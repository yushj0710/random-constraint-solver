/**
 * @file bv.hpp
 * @brief A header file defining the BitVector class for arbitrary bitwidth
 * bitwise and arithmetic operations.
 *
 * This class provides a BitVector implementation with a fixed bitwidth that
 * supports various bitwise and arithmetic operations. The bitwidth is specified
 * during construction and cannot exceed the maximum value of uint32_t. The
 * class is optimized for memory usage.
 */

#ifndef BV_HPP
#define BV_HPP

#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>

/**
 * @class BitVector
 * @brief A class representing a bit vector with a fixed bitwidth.
 *
 * The BitVector class supports various bitwise and arithmetic operations. The
 * bitwidth is specified during construction and cannot exceed the maximum value
 * of uint32_t.
 */
class BitVector {
 public:
  /**
   * @brief Constructs a BitVector with the specified bitwidth.
   * @param bitwidth The bitwidth of the BitVector.
   * @throws std::invalid_argument if the bitwidth exceeds the maximum value of
   * uint32_t.
   */
  explicit BitVector(uint32_t bitwidth);

  /**
   * @brief Constructs a BitVector with the specified bitwidth and initializes
   * it with a uint64_t value.
   * @param bitwidth The bitwidth of the BitVector.
   * @param value The uint64_t value to initialize the BitVector with.
   * @throws std::invalid_argument if the bitwidth exceeds the maximum value of
   * uint32_t.
   */
  BitVector(uint32_t bitwidth, uint64_t value);

  /**
   * @brief Constructs a BitVector with the specified bitwidth and initializes
   * it with an int64_t value.
   * @param bitwidth The bitwidth of the BitVector.
   * @param value The int64_t value to initialize the BitVector with.
   * @throws std::invalid_argument if the bitwidth exceeds the maximum value of
   * uint32_t.
   */
  BitVector(uint32_t bitwidth, int64_t value);

  /**
   * @brief Constructs a BitVector with the specified bitwidth and initializes
   * it to the minimum value.
   * @tparam Signed Specifies whether the BitVector is signed or unsigned.
   * @param bitwidth The bitwidth of the BitVector.
   * @throws std::invalid_argument if the bitwidth exceeds the maximum value of
   * uint32_t.
   */
  template <bool Signed>
  static BitVector min_value(uint32_t bitwidth);

  /**
   * @brief Constructs a BitVector with the specified bitwidth and initializes
   * it to the maximum value.
   * @tparam Signed Specifies whether the BitVector is signed or unsigned.
   * @param bitwidth The bitwidth of the BitVector.
   * @throws std::invalid_argument if the bitwidth exceeds the maximum value of
   * uint32_t.
   */
  template <bool Signed>
  static BitVector max_value(uint32_t bitwidth);

  /**
   * @brief Copy constructor.
   * @param other The other BitVector to copy from.
   */
  BitVector(const BitVector& other);

  /**
   * @brief Copy assignment operator.
   * @param other The other BitVector to copy from.
   * @return A reference to this BitVector after the copy operation.
   */
  BitVector& operator=(const BitVector& other);

  /**
   * @brief Move constructor.
   * @param other The other BitVector to move from.
   */
  BitVector(BitVector&& other) noexcept;

  /**
   * @brief Move assignment operator.
   * @param other The other BitVector to move from.
   * @return A reference to this BitVector after the move operation.
   */
  BitVector& operator=(BitVector&& other) noexcept;

  /**
   * @brief Destructor.
   */
  ~BitVector();

  /**
   * @brief Bitwise AND operator.
   * @param other The other BitVector to perform the operation with.
   * @return A new BitVector resulting from the bitwise AND operation.
   * @throws std::invalid_argument if the bitwidths of the two BitVectors do not
   * match.
   */
  BitVector operator&(const BitVector& other) const;

  unsigned int get_size() const;

  /**
   * @brief Gets the bitwidth of the BitVector.
   * @return The bitwidth of the BitVector.
   */
  uint32_t get_bitwidth() const;

  /**
   * @brief Bitwise OR operator.
   * @param other The other BitVector to perform the operation with.
   * @return A new BitVector resulting from the bitwise OR operation.
   * @throws std::invalid_argument if the bitwidths of the two BitVectors do not
   * match.
   */
  BitVector operator|(const BitVector& other) const;

  /**
   * @brief Bitwise XOR operator.
   * @param other The other BitVector to perform the operation with.
   * @return A new BitVector resulting from the bitwise XOR operation.
   * @throws std::invalid_argument if the bitwidths of the two BitVectors do not
   * match.
   */
  BitVector operator^(const BitVector& other) const;

  /**
   * @brief Bitwise NOT operator.
   * @return A new BitVector resulting from the bitwise NOT operation.
   */
  BitVector operator~() const;

  /**
   * @brief Unary negation operator.
   * @return A new BitVector representing the negation of this BitVector.
   */
  BitVector operator-() const;

  /**
   * @brief Addition operator.
   * @param other The other BitVector to perform the operation with.
   * @return A new BitVector resulting from the addition operation.
   * @throws std::invalid_argument if the bitwidths of the two BitVectors do not
   * match.
   */
  BitVector operator+(const BitVector& other) const;

  /**
   * @brief Subtraction operator.
   * @param other The other BitVector to perform the operation with.
   * @return A new BitVector resulting from the subtraction operation.
   * @throws std::invalid_argument if the bitwidths of the two BitVectors do not
   * match.
   */
  BitVector operator-(const BitVector& other) const;

  /**
   * @brief In-place addition operator.
   * @param other The other BitVector to add.
   * @return A reference to this BitVector after the addition.
   */
  BitVector& operator+=(const BitVector& other);

  /**
   * @brief In-place subtraction operator.
   * @param other The other BitVector to subtract.
   * @return A reference to this BitVector after the subtraction.
   */
  BitVector& operator-=(const BitVector& other);

  /**
   * @brief In-place multiplication operator.
   * @param other The other BitVector to multiply.
   * @return A reference to this BitVector after the multiplication.
   */
  BitVector& operator*=(const BitVector& other);

  /**
   * @brief In-place bitwise OR operator.
   * @param other The other BitVector to perform the operation with.
   * @return A reference to this BitVector after the bitwise OR operation.
   * @throws std::invalid_argument if the bitwidths of the two BitVectors do not
   * match.
   */
  BitVector& operator|=(const BitVector& other);

  /**
   * @brief In-place bitwise AND operator.
   * @param other The other BitVector to perform the operation with.
   * @return A reference to this BitVector after the bitwise AND operation.
   * @throws std::invalid_argument if the bitwidths of the two BitVectors do not
   * match.
   */
  BitVector& operator&=(const BitVector& other);

  /**
   * @brief In-place bitwise XOR operator.
   * @param other The other BitVector to perform the operation with.
   * @return A reference to this BitVector after the bitwise XOR operation.
   * @throws std::invalid_argument if the bitwidths of the two BitVectors do not
   * match.
   */
  BitVector& operator^=(const BitVector& other);

  /**
   * @brief Sets the bit at the specified position.
   * @param pos The position of the bit to set.
   * @param value The value to set the bit to (true for 1, false for 0).
   * @throws std::out_of_range if the position is out of range.
   */
  void set_bit(uint32_t pos, bool value);

  /**
   * @brief Gets the value of the bit at the specified position.
   * @param pos The position of the bit to get.
   * @return The value of the bit at the specified position.
   * @throws std::out_of_range if the position is out of range.
   */
  bool get_bit(uint32_t pos) const;

  /**
   * @brief Sets the bit at the specified position to 1.
   * @param pos The position of the bit to set.
   * @throws std::out_of_range if the position is out of range.
   */
  void bit_on(uint32_t pos);

  /**
   * @brief Sets the bit at the specified position to 0.
   * @param pos The position of the bit to clear.
   * @throws std::out_of_range if the position is out of range.
   */
  void bit_off(uint32_t pos);

  /**
   * @brief Flips the bit at the specified position.
   * @param pos The position of the bit to flip.
   * @throws std::out_of_range if the position is out of range.
   */
  void flip(uint32_t pos);

  /**
   * @brief Clears all bits in the BitVector.
   */
  void reset();

  /**
   * @brief Prints the BitVector to the standard output.
   */
  void print() const;

  /**
   * @brief Multiplication operator.
   * @param other The other BitVector to perform the operation with.
   * @return A new BitVector resulting from the multiplication operation.
   * @throws std::invalid_argument if the bitwidths of the two BitVectors do not
   * match.
   */
  BitVector operator*(const BitVector& other) const;

  /**
   * @brief Modulus operation.
   * @tparam Signed Specifies whether the modulus is signed or unsigned.
   * @param other The other BitVector to perform the operation with.
   * @return A new BitVector resulting from the modulus operation.
   * @throws std::invalid_argument if the bitwidths of the two BitVectors do not
   * match or if division by zero occurs.
   */
  template <bool Signed>
  BitVector mod_by(const BitVector& other) const;

  /**
   * @brief Divides this BitVector by another BitVector and returns the quotient
   * and remainder.
   * @tparam Signed Specifies whether the division is signed or unsigned.
   * @param other The other BitVector to divide by.
   * @return A pair containing the quotient and remainder.
   * @throws std::invalid_argument if the bitwidths of the two BitVectors do not
   * match or if division by zero occurs.
   */
  template <bool Signed>
  std::pair<BitVector, BitVector> div_by(const BitVector& other) const;

  /**
   * @brief Checks if the BitVector is zero.
   * @return True if the BitVector is zero, false otherwise.
   */
  bool is_zero() const;

  /**
   * @brief Checks if the BitVector represents a power of two.
   * @return True if the BitVector is a power of two, false otherwise.
   */
  bool is_power_of_two() const;

  /**
   * @brief Less than comparison.
   * @tparam Signed Specifies whether the comparison is signed or unsigned.
   * @param other The other BitVector to compare with.
   * @return True if this BitVector is less than the other BitVector, false
   * otherwise.
   * @throws std::invalid_argument if the bitwidths of the two BitVectors do not
   * match.
   */
  template <bool Signed>
  bool lt(const BitVector& other) const;

  /**
   * @brief Less than or equal to comparison.
   * @tparam Signed Specifies whether the comparison is signed or unsigned.
   * @param other The other BitVector to compare with.
   * @return True if this BitVector is less than or equal to the other
   * BitVector, false otherwise.
   * @throws std::invalid_argument if the bitwidths of the two BitVectors do not
   * match.
   */
  template <bool Signed>
  bool le(const BitVector& other) const;

  /**
   * @brief Greater than comparison.
   * @tparam Signed Specifies whether the comparison is signed or unsigned.
   * @param other The other BitVector to compare with.
   * @return True if this BitVector is greater than the other BitVector, false
   * otherwise.
   * @throws std::invalid_argument if the bitwidths of the two BitVectors do not
   * match.
   */
  template <bool Signed>
  bool gt(const BitVector& other) const;

  /**
   * @brief Greater than or equal to comparison.
   * @tparam Signed Specifies whether the comparison is signed or unsigned.
   * @param other The other BitVector to compare with.
   * @return True if this BitVector is greater than or equal to the other
   * BitVector, false otherwise.
   * @throws std::invalid_argument if the bitwidths of the two BitVectors do not
   * match.
   */
  template <bool Signed>
  bool ge(const BitVector& other) const;

  /**
   * @brief Equality operator.
   * @param other The other BitVector to compare with.
   * @return True if the two BitVectors are equal, false otherwise.
   * @throws std::invalid_argument if the bitwidths of the two BitVectors do not
   * match.
   */
  bool operator==(const BitVector& other) const;

  /**
   * @brief Inequality operator.
   * @param other The other BitVector to compare with.
   * @return True if the two BitVectors are not equal, false otherwise.
   * @throws std::invalid_argument if the bitwidths of the two BitVectors do not
   * match.
   */
  bool operator!=(const BitVector& other) const;

  /**
   * @brief Left shift operator.
   * @param shift The BitVector representing the number of positions to shift.
   * @return A new BitVector resulting from the left shift operation.
   */
  BitVector operator<<(const BitVector& shift) const;

  /**
   * @brief Right shift operator.
   * @param shift The BitVector representing the number of positions to shift.
   * @return A new BitVector resulting from the right shift operation.
   */
  BitVector operator>>(const BitVector& shift) const;

  /**
   * @brief Left shift operator.
   * @param shift The number of positions to shift.
   * @return A new BitVector resulting from the left shift operation.
   */
  BitVector operator<<(uint32_t shift) const;

  /**
   * @brief Right shift operator.
   * @param shift The number of positions to shift.
   * @return A new BitVector resulting from the right shift operation.
   */
  BitVector operator>>(uint32_t shift) const;

  /**
   * @brief Arithmetic right shift operator.
   * @param shift The BitVector representing the number of positions to shift.
   * @return A new BitVector resulting from the arithmetic right shift
   * operation.
   */
  BitVector ashr(const BitVector& shift) const;

  /**
   * @brief Arithmetic right shift operator.
   * @param shift The number of positions to shift.
   * @return A new BitVector resulting from the arithmetic right shift
   * operation.
   */
  BitVector ashr(uint32_t shift) const;

  /**
   * @brief Left shift assignment operator.
   * @param shift The BitVector representing the number of positions to shift.
   * @return A reference to this BitVector after the left shift operation.
   */
  BitVector& operator<<=(const BitVector& shift);

  /**
   * @brief Right shift assignment operator.
   * @param shift The BitVector representing the number of positions to shift.
   * @return A reference to this BitVector after the right shift operation.
   */
  BitVector& operator>>=(const BitVector& shift);

  /**
   * @brief Left shift assignment operator.
   * @param shift The number of positions to shift.
   * @return A reference to this BitVector after the left shift operation.
   */
  BitVector& operator<<=(uint32_t shift);

  /**
   * @brief Right shift assignment operator.
   * @param shift The number of positions to shift.
   * @return A reference to this BitVector after the right shift operation.
   */
  BitVector& operator>>=(uint32_t shift);

  /**
   * @brief Arithmetic right shift assignment operator.
   * @param shift The BitVector representing the number of positions to shift.
   * @return A reference to this BitVector after the arithmetic right shift
   * operation.
   */
  BitVector& ashr_assign(const BitVector& shift);

  /**
   * @brief Arithmetic right shift assignment operator.
   * @param shift The number of positions to shift.
   * @return A reference to this BitVector after the arithmetic right shift
   * operation.
   */
  BitVector& ashr_assign(uint32_t shift);

  /**
   * @brief Computes the hash value of the BitVector.
   * @return The hash value of the BitVector.
   */
  std::size_t hash() const;

  /**
   * @brief Prefix increment operator.
   * @return A reference to this BitVector after the increment operation.
   */
  BitVector& operator++();

  /**
   * @brief Prefix decrement operator.
   * @return A reference to this BitVector after the decrement operation.
   */
  BitVector& operator--();

  /**
   * @brief Postfix increment operator.
   * @return A new BitVector before the increment operation.
   */
  BitVector operator++(int);

  /**
   * @brief Postfix decrement operator.
   * @return A new BitVector before the decrement operation.
   */
  BitVector operator--(int);

  /**
   * @brief Computes the power of the BitVector raised to an exponent using fast
   * exponentiation.
   * @param exponent The BitVector representing the exponent to raise the
   * BitVector to.
   * @return A new BitVector representing the result.
   */
  BitVector fast_power(const BitVector& exponent) const;

  /**
   * @brief Zero extends the BitVector by a specified incremental width.
   * @param increment The incremental width to extend by.
   * @return A new BitVector with the extended bitwidth.
   * @throws std::invalid_argument if the new bitwidth exceeds the maximum value
   * of uint32_t.
   */
  BitVector zero_extend_by(uint32_t increment) const;

  /**
   * @brief Sign extends the BitVector by a specified incremental width.
   * @param increment The incremental width to extend by.
   * @return A new BitVector with the extended bitwidth.
   * @throws std::invalid_argument if the new bitwidth exceeds the maximum value
   * of uint32_t.
   */
  BitVector sign_extend_by(uint32_t increment) const;

  /**
   * @brief Computes the size of the BitVector array with the given bitwidth.
   * @param bitwidth The bitwidth of the BitVector.
   * @return The size of the BitVector array.
   */
  static uint32_t get_size(uint32_t bitwidth);

  /**
   * @brief Computes the mask for the BitVector with the given bitwidth.
   * @param bitwidth The bitwidth of the BitVector.
   * @return The mask for the BitVector.
   */
  static uint32_t get_mask(uint32_t bitwidth);

  /**
   * @brief Converts the BitVector to a uint64_t value.
   * @return A pair containing the uint64_t value of the BitVector and a boolean
   * indicating success.
   */
  std::pair<uint64_t, bool> to_uint64() const;

  /**
   * @brief Converts the BitVector to an int64_t value.
   * @return A pair containing the int64_t value of the BitVector and a boolean
   * indicating success.
   */
  std::pair<int64_t, bool> to_int64() const;

  /**
   * @brief Negates the BitVector in-place.
   * @return A reference to this BitVector after the negation.
   */
  BitVector& negate();

  /**
   * @brief Returns the hexadecimal string representation of the BitVector.
   * @return A string representing the BitVector in hexadecimal format.
   */
  std::string to_string() const;

  /**
   * @brief Outputs the BitVector to an ostream.
   * @param os The ostream to output to.
   * @param bv The BitVector to output.
   * @return The ostream after outputting the BitVector.
   */
  friend std::ostream& operator<<(std::ostream& os, const BitVector& bv);

 private:
  uint32_t*
      bits;  // Raw pointer to the array containing bitwidth, mask, and bits

  /**
   * @brief Checks if the bitwidths of two BitVectors match.
   * @param other The other BitVector to check against.
   * @throws std::invalid_argument if the bitwidths do not match.
   */
  void check_bitwidth(const BitVector& other) const;

  /**
   * @brief Applies the mask to the BitVector.
   */
  void apply_mask();
};

#endif  // BV_HPP