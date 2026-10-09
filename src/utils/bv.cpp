#include "utils/bv.hpp"

#include <algorithm>
#include <bitset>
#include <functional>
#include <iostream>
#include <sstream>

BitVector::BitVector(uint32_t bitwidth) {
  bits = new uint32_t[get_size(bitwidth) + 2];
  bits[0] = bitwidth;
  bits[1] = get_mask(bitwidth);
  std::fill(bits + 2, bits + get_size() + 2, 0);
}

BitVector::BitVector(uint32_t bitwidth, uint64_t value) : BitVector(bitwidth) {
  bits[2] = static_cast<uint32_t>(value);
  if (bitwidth > 32) {
    bits[3] = static_cast<uint32_t>(value >> 32);
  }
}

BitVector::BitVector(uint32_t bitwidth, int64_t value)
    : BitVector(bitwidth, static_cast<uint64_t>(value)) {
  if (value < 0) {
    for (uint32_t i = 3 + (bitwidth > 32); i < get_size() + 2; ++i) {
      bits[i] = ~0U;
    }
    apply_mask();
  }
}

BitVector::BitVector(const BitVector& other) {
  bits = new uint32_t[other.get_size() + 2];
  std::copy(other.bits, other.bits + other.get_size() + 2, bits);
}

BitVector& BitVector::operator=(const BitVector& other) {
  if (this != &other) {
    delete[] bits;
    bits = new uint32_t[other.get_size() + 2];
    std::copy(other.bits, other.bits + other.get_size() + 2, bits);
  }
  return *this;
}

BitVector::BitVector(BitVector&& other) noexcept : bits(other.bits) {
  other.bits = nullptr;
}

BitVector& BitVector::operator=(BitVector&& other) noexcept {
  if (this != &other) {
    delete[] bits;
    bits = other.bits;
    other.bits = nullptr;
  }
  return *this;
}

BitVector::~BitVector() { delete[] bits; }

BitVector BitVector::operator&(const BitVector& other) const {
  check_bitwidth(other);
  BitVector result(bits[0]);
  for (uint32_t i = 0, j = 2; i < get_size(); ++i, ++j) {
    result.bits[j] = bits[j] & other.bits[j];
  }
  result.apply_mask();
  return result;
}

unsigned int BitVector::get_size() const { return get_size(bits[0]); }

BitVector BitVector::operator|(const BitVector& other) const {
  check_bitwidth(other);
  BitVector result(bits[0]);
  for (uint32_t i = 0; i < get_size(); ++i) {
    result.bits[i + 2] = bits[i + 2] | other.bits[i + 2];
  }
  result.apply_mask();
  return result;
}

BitVector BitVector::operator^(const BitVector& other) const {
  check_bitwidth(other);
  BitVector result(bits[0]);
  for (uint32_t i = 0; i < get_size(); ++i) {
    result.bits[i + 2] = bits[i + 2] ^ other.bits[i + 2];
  }
  result.apply_mask();
  return result;
}

BitVector BitVector::operator~() const {
  BitVector result(bits[0]);
  for (uint32_t i = 0; i < get_size(); ++i) {
    result.bits[i + 2] = ~bits[i + 2];
  }
  result.apply_mask();
  return result;
}

BitVector BitVector::operator+(const BitVector& other) const {
  check_bitwidth(other);
  BitVector result(bits[0]);
  uint64_t carry = 0;
  for (uint32_t i = 0; i < get_size(); ++i) {
    uint64_t sum =
        static_cast<uint64_t>(bits[i + 2]) + other.bits[i + 2] + carry;
    result.bits[i + 2] = static_cast<uint32_t>(sum);
    carry = sum >> 32;
  }
  result.apply_mask();
  return result;
}

BitVector BitVector::operator-(const BitVector& other) const {
  check_bitwidth(other);
  BitVector result(bits[0]);
  uint64_t borrow = 0;
  for (uint32_t i = 0; i < get_size(); ++i) {
    uint64_t diff =
        static_cast<uint64_t>(bits[i + 2]) - other.bits[i + 2] - borrow;
    result.bits[i + 2] = static_cast<uint32_t>(diff);
    borrow = (diff >> 32) & 1;
  }
  result.apply_mask();
  return result;
}

BitVector BitVector::operator-() const { return ++(~(*this)); }

void BitVector::bit_on(uint32_t pos) {
  if (pos >= bits[0]) {
    throw std::out_of_range("Bit position out of range");
  }
  uint32_t index = pos / 32 + 2;
  uint32_t bit = pos % 32;
  bits[index] |= (1U << bit);
}

void BitVector::bit_off(uint32_t pos) {
  if (pos >= bits[0]) {
    throw std::out_of_range("Bit position out of range");
  }
  uint32_t index = pos / 32 + 2;
  uint32_t bit = pos % 32;
  bits[index] &= ~(1U << bit);
}
void BitVector::set_bit(uint32_t pos, bool value) {
  if (value) {
    bit_on(pos);
  } else {
    bit_off(pos);
  }
}

void BitVector::flip(uint32_t pos) {
  if (pos >= bits[0]) {
    throw std::out_of_range("Bit position out of range");
  }
  uint32_t index = pos / 32 + 2;
  uint32_t bit = pos % 32;
  bits[index] ^= (1U << bit);
}

bool BitVector::is_zero() const {
  for (uint32_t i = 2; i < get_size() + 2; ++i) {
    if (bits[i] != 0) {
      return false;
    }
  }
  return true;
}
bool BitVector::get_bit(uint32_t pos) const {
  if (pos >= bits[0]) {
    throw std::out_of_range("Bit position out of range");
  }
  uint32_t index = pos / 32 + 2;
  uint32_t bit = pos % 32;
  return (bits[index] & (1U << bit)) != 0;
}

std::string BitVector::to_string() const {
  std::ostringstream oss;
  oss << "0x";
  for (int i = get_size() + 1; i >= 2; --i) {
    oss << std::hex << bits[i];
  }
  return oss.str();
}

BitVector BitVector::operator*(const BitVector& other) const {
  check_bitwidth(other);
  BitVector result(bits[0]);
  BitVector multiplicand = *this;
  BitVector multiplier = other;

  while (!multiplier.is_zero()) {
    if (multiplier.get_bit(0)) {
      result += multiplicand;
    }
    multiplicand <<= 1;
    multiplier >>= 1;
  }

  result.apply_mask();
  return result;
}

BitVector BitVector::operator<<(const BitVector& shift) const {
  auto [shift_value, success] = shift.to_uint64();
  if (!success) {
    throw std::invalid_argument("Shift value is too large");
  }
  return operator<<(shift_value);
}

BitVector BitVector::operator<<(uint32_t shift) const {
  if (shift == 0) return *this;
  BitVector result(bits[0]);
  if (shift >= bits[0]) return result;
  uint32_t word_shift = shift / 32;
  uint32_t bit_shift = shift % 32;
  for (uint32_t i = get_size() + 1; i >= word_shift + 2; --i) {
    result.bits[i] = bits[i - word_shift] << bit_shift;
    if (i > word_shift + 2 && bit_shift > 0) {
      result.bits[i] |= bits[i - word_shift - 1] >> (32 - bit_shift);
    }
  }
  result.apply_mask();
  return result;
}

BitVector BitVector::operator>>(const BitVector& shift) const {
  auto [shift_value, success] = shift.to_uint64();
  if (!success) {
    throw std::invalid_argument("Shift value is too large");
  }
  return operator>>(shift_value);
}

BitVector BitVector::operator>>(uint32_t shift) const {
  if (shift == 0) return *this;
  BitVector result(bits[0]);
  if (shift >= bits[0]) return result;
  uint32_t word_shift = shift / 32;
  uint32_t bit_shift = shift % 32;
  for (uint32_t i = 2; i < get_size() + 2 - word_shift; ++i) {
    result.bits[i] = bits[i + word_shift] >> bit_shift;
    if (i + word_shift + 1 < get_size() + 2 && bit_shift > 0) {
      result.bits[i] |= bits[i + word_shift + 1] << (32 - bit_shift);
    }
  }
  result.apply_mask();
  return result;
}

BitVector BitVector::ashr(const BitVector& shift) const {
  auto [shift_value, success] = shift.to_uint64();
  if (!success) {
    throw std::invalid_argument("Shift value is too large");
  }
  return ashr(shift_value);
}

BitVector BitVector::ashr(uint32_t shift) const {
  BitVector result(bits[0]);
  uint32_t word_shift = shift / 32;
  uint32_t bit_shift = shift % 32;
  for (uint32_t i = 2; i < get_size() + 2 - word_shift; ++i) {
    result.bits[i] = bits[i + word_shift] >> bit_shift;
    if (i + word_shift + 1 < get_size() + 2 && bit_shift > 0) {
      result.bits[i] |= bits[i + word_shift + 1] << (32 - bit_shift);
    }
  }
  if (get_bit(bits[0] - 1)) {
    for (uint32_t i = get_size() + 2 - word_shift; i < get_size() + 2; ++i) {
      result.bits[i] = ~0U;
    }
    if (bit_shift > 0) {
      result.bits[get_size() + 2 - word_shift - 1] |=
          ~((1U << (32 - bit_shift)) - 1);
    }
  }
  result.apply_mask();
  return result;
}

BitVector& BitVector::operator<<=(const BitVector& shift) {
  auto [shift_value, success] = shift.to_uint64();
  if (!success) {
    throw std::invalid_argument("Shift value is too large");
  }
  return operator<<=(shift_value);
}

BitVector& BitVector::operator>>=(const BitVector& shift) {
  auto [shift_value, success] = shift.to_uint64();
  if (!success) {
    throw std::invalid_argument("Shift value is too large");
  }
  return operator>>=(shift_value);
}

BitVector& BitVector::operator<<=(uint32_t shift) {
  if (shift >= get_bitwidth()) {
    reset();
    return *this;
  }
  uint32_t word_shift = shift / 32;
  uint32_t bit_shift = shift % 32;
  if (word_shift > 0) {
    for (uint32_t i = get_size() + 1; i >= word_shift + 2; --i) {
      bits[i] = bits[i - word_shift];
    }
    std::fill(bits + 2, bits + word_shift + 2, 0);
  }
  if (bit_shift > 0) {
    for (uint32_t i = get_size() + 1; i >= 2; --i) {
      bits[i] <<= bit_shift;
      if (i > 2) {
        bits[i] |= bits[i - 1] >> (32 - bit_shift);
      }
    }
  }
  apply_mask();
  return *this;
}

BitVector& BitVector::operator>>=(uint32_t shift) {
  uint32_t word_shift = shift / 32;
  uint32_t bit_shift = shift % 32;
  if (word_shift > 0) {
    for (uint32_t i = 2; i < get_size() + 2 - word_shift; ++i) {
      bits[i] = bits[i + word_shift];
    }
    std::fill(bits + get_size() + 2 - word_shift, bits + get_size() + 2, 0);
  }
  if (bit_shift > 0) {
    for (uint32_t i = 2; i < get_size() + 2; ++i) {
      bits[i] >>= bit_shift;
      if (i + 1 < get_size() + 2) {
        bits[i] |= bits[i + 1] << (32 - bit_shift);
      }
    }
  }
  apply_mask();
  return *this;
}

BitVector& BitVector::ashr_assign(const BitVector& shift) {
  auto [shift_value, success] = shift.to_uint64();
  if (!success) {
    throw std::invalid_argument("Shift value is too large");
  }
  return ashr_assign(shift_value);
}

BitVector& BitVector::ashr_assign(uint32_t shift) {
  if (shift >= get_bitwidth()) {
    if (get_bit(bits[0] - 1)) {
      std::fill(bits + 2, bits + get_size() + 2, ~0U);
    } else {
      reset();
    }
    return *this;
  }
  uint32_t word_shift = shift / 32;
  uint32_t bit_shift = shift % 32;
  if (word_shift > 0) {
    for (uint32_t i = 2; i < get_size() + 2 - word_shift; ++i) {
      bits[i] = bits[i + word_shift];
    }
    std::fill(bits + get_size() + 2 - word_shift, bits + get_size() + 2, 0);
  }
  if (bit_shift > 0) {
    for (uint32_t i = 2; i < get_size() + 2; ++i) {
      bits[i] >>= bit_shift;
      if (i + 1 < get_size() + 2) {
        bits[i] |= bits[i + 1] << (32 - bit_shift);
      }
    }
  }
  if (get_bit(bits[0] - 1)) {
    for (uint32_t i = get_size() + 2 - word_shift; i < get_size() + 2; ++i) {
      bits[i] = ~0U;
    }
    if (bit_shift > 0) {
      bits[get_size() + 2 - word_shift - 1] |= ~((1U << (32 - bit_shift)) - 1);
    }
  }
  apply_mask();
  return *this;
}

BitVector& BitVector::operator+=(const BitVector& other) {
  check_bitwidth(other);
  uint64_t carry = 0;
  for (uint32_t i = 0; i < get_size(); ++i) {
    uint64_t sum =
        static_cast<uint64_t>(bits[i + 2]) + other.bits[i + 2] + carry;
    bits[i + 2] = static_cast<uint32_t>(sum);
    carry = sum >> 32;
  }
  apply_mask();
  return *this;
}

BitVector& BitVector::operator-=(const BitVector& other) {
  check_bitwidth(other);
  uint64_t borrow = 0;
  for (uint32_t i = 0; i < get_size(); ++i) {
    uint64_t diff =
        static_cast<uint64_t>(bits[i + 2]) - other.bits[i + 2] - borrow;
    bits[i + 2] = static_cast<uint32_t>(diff);
    borrow = (diff >> 32) & 1;
  }
  apply_mask();
  return *this;
}

BitVector& BitVector::operator*=(const BitVector& other) {
  check_bitwidth(other);
  BitVector result(bits[0]);
  BitVector multiplicand = *this;
  BitVector multiplier = other;

  while (!multiplier.is_zero()) {
    if (multiplier.get_bit(0)) {
      result += multiplicand;
    }
    multiplicand <<= 1;
    multiplier >>= 1;
  }

  *this = std::move(result);
  apply_mask();
  return *this;
}

BitVector& BitVector::operator|=(const BitVector& other) {
  check_bitwidth(other);
  for (uint32_t i = 0; i < get_size(); ++i) {
    bits[i + 2] |= other.bits[i + 2];
  }
  apply_mask();
  return *this;
}

BitVector& BitVector::operator&=(const BitVector& other) {
  check_bitwidth(other);
  for (uint32_t i = 0; i < get_size(); ++i) {
    bits[i + 2] &= other.bits[i + 2];
  }
  apply_mask();
  return *this;
}

BitVector& BitVector::operator^=(const BitVector& other) {
  check_bitwidth(other);
  for (uint32_t i = 0; i < get_size(); ++i) {
    bits[i + 2] ^= other.bits[i + 2];
  }
  apply_mask();
  return *this;
}

std::size_t BitVector::hash() const {
  std::size_t seed = 0;
  for (uint32_t i = 0; i < get_size() + 2; ++i) {
    seed ^=
        std::hash<uint32_t>{}(bits[i]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
  return seed;
}

void BitVector::check_bitwidth(const BitVector& other) const {
  if (bits[0] != other.bits[0]) {
    throw std::invalid_argument("Bitwidths do not match");
  }
}

void BitVector::apply_mask() { bits[get_size(bits[0]) + 1] &= bits[1]; }

uint32_t BitVector::get_bitwidth() const { return bits[0]; }

BitVector& BitVector::operator++() {
  uint64_t carry = 1;
  for (uint32_t i = 0; i < get_size(); ++i) {
    uint64_t sum = static_cast<uint64_t>(bits[i + 2]) + carry;
    bits[i + 2] = static_cast<uint32_t>(sum);
    carry = sum >> 32;
    if (carry == 0) break;
  }
  apply_mask();
  return *this;
}

BitVector BitVector::operator++(int) {
  BitVector temp = *this;
  ++(*this);
  return temp;
}

BitVector& BitVector::operator--() {
  uint64_t borrow = 1;
  for (uint32_t i = 0; i < get_size(); ++i) {
    uint64_t diff = static_cast<uint64_t>(bits[i + 2]) - borrow;
    bits[i + 2] = static_cast<uint32_t>(diff);
    borrow = (diff >> 32) & 1;
    if (borrow == 0) break;
  }
  apply_mask();
  return *this;
}

BitVector BitVector::operator--(int) {
  BitVector temp = *this;
  --(*this);
  return temp;
}

bool BitVector::is_power_of_two() const {
  if (is_zero()) return false;
  bool found_one = false;
  for (uint32_t i = 2; i < get_size() + 2; ++i) {
    if (bits[i] == 0) continue;
    if (found_one) return false;
    if ((bits[i] & (bits[i] - 1)) != 0) return false;
    found_one = true;
  }
  return true;
}

BitVector BitVector::fast_power(const BitVector& exponent) const {
  BitVector base = *this;
  BitVector result(get_bitwidth());
  result.set_bit(0, true);  // Initialize result to 1

  BitVector exp = exponent;
  while (!exp.is_zero()) {
    if (exp.get_bit(0)) {
      result *= base;
    }
    base *= base;
    exp >>= 1;
  }

  return result;
}

BitVector BitVector::zero_extend_by(uint32_t increment) const {
  uint32_t new_bitwidth = bits[0] + increment;
  if (new_bitwidth < bits[0]) {
    throw std::invalid_argument(
        "New bitwidth exceeds the maximum value of uint32_t");
  }
  BitVector result(new_bitwidth);
  std::copy(bits + 2, bits + get_size() + 2, result.bits + 2);
  return result;
}

BitVector BitVector::sign_extend_by(uint32_t increment) const {
  uint32_t new_bitwidth = bits[0] + increment;
  if (new_bitwidth < bits[0]) {
    throw std::invalid_argument(
        "New bitwidth exceeds the maximum value of uint32_t");
  }
  BitVector result(new_bitwidth);
  std::copy(bits + 2, bits + get_size() + 2, result.bits + 2);
  if (get_bit(bits[0] - 1)) {
    result.bits[get_size() + 1] |= ~bits[1];
    std::fill(result.bits + get_size() + 2, result.bits + result.get_size() + 2,
              ~0U);
    result.apply_mask();
  }
  return result;
}

uint32_t BitVector::get_size(uint32_t bitwidth) { return (bitwidth + 31) / 32; }

uint32_t BitVector::get_mask(uint32_t bitwidth) {
  return (bitwidth % 32 == 0) ? ~0U : (1U << (bitwidth % 32)) - 1;
}

void BitVector::reset() { std::fill(bits + 2, bits + get_size() + 2, 0); }

template <bool Signed>
BitVector BitVector::mod_by(const BitVector& other) const {
  return div_by<Signed>(other).second;
}

// Explicit instantiation of template methods
template BitVector BitVector::mod_by<true>(const BitVector& other) const;
template BitVector BitVector::mod_by<false>(const BitVector& other) const;

template <bool Signed>
std::pair<BitVector, BitVector> BitVector::div_by(
    const BitVector& other) const {
  check_bitwidth(other);
  if (other.is_zero()) {
    throw std::invalid_argument("Division by zero");
  }
  BitVector quotient(bits[0]);
  BitVector remainder(bits[0]);
  bool is_negative = false;
  BitVector dividend = *this;
  BitVector divisor = other;
  if constexpr (Signed) {
    is_negative = (get_bit(bits[0] - 1) != other.get_bit(bits[0] - 1));
    if (get_bit(bits[0] - 1)) {
      dividend = -dividend;
    }
    if (other.get_bit(bits[0] - 1)) {
      divisor = -divisor;
    }
  }
  for (int i = bits[0] - 1; i >= 0; --i) {
    remainder <<= 1;
    remainder.set_bit(0, dividend.get_bit(i));
    if (!remainder.template lt<true>(divisor)) {
      remainder -= divisor;
      quotient.set_bit(i, true);
    }
  }
  if (Signed && is_negative) {
    quotient = -quotient;
  }
  quotient.apply_mask();
  remainder.apply_mask();
  return {quotient, remainder};
}

template <bool Signed>
bool BitVector::lt(const BitVector& other) const {
  check_bitwidth(other);
  if constexpr (Signed) {
    bool this_negative = get_bit(bits[0] - 1);
    bool other_negative = other.get_bit(bits[0] - 1);
    if (this_negative != other_negative) {
      return this_negative;
    }
  }
  for (int i = get_size() + 1; i >= 2; --i) {
    if (bits[i] != other.bits[i]) {
      return bits[i] < other.bits[i];
    }
  }
  return false;
}

template <bool Signed>
bool BitVector::le(const BitVector& other) const {
  return !this->template gt<Signed>(other);
}

template <bool Signed>
bool BitVector::gt(const BitVector& other) const {
  return other.template lt<Signed>(*this);
}

template <bool Signed>
bool BitVector::ge(const BitVector& other) const {
  return !this->template lt<Signed>(other);
}

// Explicit instantiation of template methods
template std::pair<BitVector, BitVector> BitVector::div_by<true>(
    const BitVector& other) const;
template std::pair<BitVector, BitVector> BitVector::div_by<false>(
    const BitVector& other) const;
template bool BitVector::lt<true>(const BitVector& other) const;
template bool BitVector::lt<false>(const BitVector& other) const;
template bool BitVector::le<true>(const BitVector& other) const;
template bool BitVector::le<false>(const BitVector& other) const;
template bool BitVector::gt<true>(const BitVector& other) const;
template bool BitVector::gt<false>(const BitVector& other) const;
template bool BitVector::ge<true>(const BitVector& other) const;
template bool BitVector::ge<false>(const BitVector& other) const;

std::pair<uint64_t, bool> BitVector::to_uint64() const {
  uint64_t value = bits[2];
  if (bits[0] > 32) {
    value |= static_cast<uint64_t>(bits[3]) << 32;
  }
  if (bits[0] > 64) {
    for (uint32_t i = 4; i < get_size() + 2; ++i) {
      if (bits[i] != 0) {
        return {0, false};
      }
    }
  }
  return {value, true};
}

std::pair<int64_t, bool> BitVector::to_int64() const {
  uint64_t unsigned_value = bits[2];
  if (bits[0] > 32) {
    unsigned_value |= static_cast<uint64_t>(bits[3]) << 32;
  }
  if (bits[0] > 64) {
    bool is_negative = get_bit(bits[0] - 1);
    // if the number is negative, the bits beyond the bitwidth should be all 1
    if (is_negative) {
      for (uint32_t i = 4; i < get_size() + 1; ++i) {
        if (bits[i] != ~0U) {
          return {0, false};
        }
      }
      // the last work should be equal to the mask
      if (bits[get_size() + 1] != bits[1]) {
        return {0, false};
      }
    } else {
      // if the number is positive, the bits beyond the bitwidth should be all 0
      for (uint32_t i = 4; i < get_size() + 2; ++i) {
        if (bits[i] != 0) {
          return {0, false};
        }
      }
    }
  }
  int64_t value = static_cast<int64_t>(unsigned_value);
  if (get_bit(bits[0] - 1)) {
    // Set the bits beyond the bitwidth to 1
    value |= static_cast<uint64_t>(~bits[1]) << 32;
  }
  return {value, true};
}
template <bool Signed>
BitVector BitVector::min_value(uint32_t bitwidth) {
  BitVector result(bitwidth);
  if constexpr (Signed) {
    result.bit_on(bitwidth - 1);  // Set the highest bit to 1
  }
  return result;
}

template <bool Signed>
BitVector BitVector::max_value(uint32_t bitwidth) {
  BitVector result(bitwidth, static_cast<int64_t>(-1));
  if constexpr (Signed) {
    result.bit_off(bitwidth - 1);  // Set the highest bit to 0
  }
  return result;
}

// Explicit instantiation of template methods
template BitVector BitVector::min_value<true>(uint32_t bitwidth);
template BitVector BitVector::min_value<false>(uint32_t bitwidth);
template BitVector BitVector::max_value<true>(uint32_t bitwidth);
template BitVector BitVector::max_value<false>(uint32_t bitwidth);

BitVector& BitVector::negate() {
  *this = -(*this);
  return *this;
}

bool BitVector::operator==(const BitVector& other) const {
  check_bitwidth(other);
  for (uint32_t i = 0; i < get_size() + 2; ++i) {
    if (bits[i] != other.bits[i]) {
      return false;
    }
  }
  return true;
}

bool BitVector::operator!=(const BitVector& other) const {
  return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const BitVector& bv) {
  os << bv.to_string();
  return os;
}
