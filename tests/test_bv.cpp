#include <gtest/gtest.h>

#include "utils/bv.hpp"

TEST(BitVectorTest, Constructor) {
  BitVector bv(32);
  EXPECT_EQ(bv.get_size(), 1);
  EXPECT_TRUE(bv.is_zero());
}

TEST(BitVectorTest, ConstructorWithValue) {
  BitVector bv(32, static_cast<uint64_t>(0x12345678));
  EXPECT_EQ(bv.get_bitwidth(), 32);
  BitVector expected(32);
  expected.set_bit(3, true);
  expected.set_bit(4, true);
  expected.set_bit(5, true);
  expected.set_bit(6, true);
  expected.set_bit(9, true);
  expected.set_bit(10, true);
  expected.set_bit(12, true);
  expected.set_bit(14, true);
  expected.set_bit(18, true);
  expected.set_bit(20, true);
  expected.set_bit(21, true);
  expected.set_bit(25, true);
  expected.set_bit(28, true);
  EXPECT_TRUE(bv == expected);
}

TEST(BitVectorTest, ConstructorWithSignedValue) {
  BitVector bv(32, static_cast<int64_t>(-1));
  EXPECT_EQ(bv.get_bitwidth(), 32);
  BitVector expected(32, static_cast<uint64_t>(-1));
  EXPECT_TRUE(bv == expected);

  BitVector bv2(32, static_cast<int64_t>(-12345678));
  BitVector expected2(32);
  std::set<int> offs = {0, 2, 3, 6, 8, 13, 14, 18, 19, 20, 21, 23};
  for (int i = 0; i < 32; ++i) {
    if (!offs.count(i)) {
      expected2.set_bit(i, true);
    }
  }
  EXPECT_TRUE(bv2 == expected2);
}

TEST(BitVectorTest, SetAndGetBit) {
  BitVector bv(32);
  bv.set_bit(0, true);
  EXPECT_TRUE(bv.get_bit(0));
  bv.set_bit(0, false);
  EXPECT_FALSE(bv.get_bit(0));
}

TEST(BitVectorTest, FlipBit) {
  BitVector bv(32);
  bv.flip(0);
  EXPECT_TRUE(bv.get_bit(0));
  bv.flip(0);
  EXPECT_FALSE(bv.get_bit(0));

  bv.flip(31);
  EXPECT_TRUE(bv.get_bit(31));
  bv.flip(31);
  EXPECT_FALSE(bv.get_bit(31));
}

TEST(BitVectorTest, BitwiseOperations) {
  BitVector bv1(32, static_cast<uint64_t>(1));
  BitVector bv2(32, static_cast<uint64_t>(2));
  BitVector bv_and = bv1 & bv2;
  BitVector bv_or = bv1 | bv2;
  BitVector bv_xor = bv1 ^ bv2;
  EXPECT_TRUE(bv_and.is_zero());
  BitVector expected_or(32, static_cast<uint64_t>(3));
  EXPECT_TRUE(bv_or == expected_or);
  BitVector expected_xor(32, static_cast<uint64_t>(3));
  EXPECT_TRUE(bv_xor == expected_xor);
}

TEST(BitVectorTest, InPlaceBitwiseOperations) {
  BitVector bv1(32, static_cast<uint64_t>(1));
  BitVector bv2(32, static_cast<uint64_t>(2));

  bv1 |= bv2;
  BitVector expected_or(32, static_cast<uint64_t>(3));
  EXPECT_TRUE(bv1 == expected_or);

  bv1 &= bv2;
  BitVector expected_and(32, static_cast<uint64_t>(2));
  EXPECT_TRUE(bv1 == expected_and);

  bv1 ^= bv2;
  BitVector expected_xor(32);
  EXPECT_TRUE(bv1 == expected_xor);
}

TEST(BitVectorTest, ArithmeticOperations) {
  BitVector bv1(32, static_cast<uint64_t>(1));
  BitVector bv2(32, static_cast<uint64_t>(2));
  BitVector bv_add = bv1 + bv2;
  BitVector bv_sub = bv2 - bv1;
  BitVector expected_add(32, static_cast<uint64_t>(3));
  EXPECT_TRUE(bv_add == expected_add);
  BitVector expected_sub(32, static_cast<uint64_t>(1));
  EXPECT_TRUE(bv_sub == expected_sub);
}

TEST(BitVectorTest, InPlaceArithmeticOperations) {
  BitVector bv1(32, static_cast<uint64_t>(1));
  BitVector bv2(32, static_cast<uint64_t>(2));

  bv1 += bv2;
  EXPECT_TRUE(bv1 == BitVector(32, static_cast<uint64_t>(3)));

  bv1 -= bv2;
  EXPECT_TRUE(bv1 == BitVector(32, static_cast<uint64_t>(1)));

  bv1 -= bv2;
  EXPECT_TRUE(bv1 == BitVector(32, static_cast<uint64_t>(-1)));

  bv1 *= bv2;
  EXPECT_TRUE(bv1 == BitVector(32, static_cast<uint64_t>(-2)));

  bv1.reset();
  bv1.set_bit(0, true);
  bv2.set_bit(0, false);
  bv2.set_bit(1, true);
  bv1 = bv1.mod_by<true>(bv2);  // Specify true for signed modulus
  BitVector expected_mod(32, static_cast<int64_t>(1));
  EXPECT_TRUE(bv1 == expected_mod);
}

TEST(BitVectorTest, UnsignedDivision) {
  BitVector bv1(32, static_cast<uint64_t>(10));
  BitVector bv2(32, static_cast<uint64_t>(3));
  auto [quotient, remainder] = bv1.div_by<false>(bv2);
  EXPECT_EQ(quotient.get_bitwidth(), 32);
  EXPECT_EQ(remainder.get_bitwidth(), 32);
  EXPECT_TRUE(quotient == BitVector(32, static_cast<uint64_t>(3)));
  EXPECT_TRUE(remainder == BitVector(32, static_cast<uint64_t>(1)));
}

TEST(BitVectorTest, SignedDivision) {
  BitVector bv1(32, static_cast<int64_t>(-10));
  BitVector bv2(32, static_cast<int64_t>(3));
  auto [quotient, remainder] = bv1.div_by<true>(bv2);
  EXPECT_EQ(quotient.get_bitwidth(), 32);
  EXPECT_EQ(remainder.get_bitwidth(), 32);
  EXPECT_TRUE(quotient == BitVector(32, static_cast<int64_t>(-3)));
  EXPECT_TRUE(remainder == BitVector(32, static_cast<int64_t>(-1)));
}

TEST(BitVectorTest, UnsignedModulus) {
  BitVector bv1(32, static_cast<uint64_t>(10));
  BitVector bv2(32, static_cast<uint64_t>(3));
  BitVector result = bv1.mod_by<false>(bv2);
  EXPECT_EQ(result.get_bitwidth(), 32);
  EXPECT_TRUE(result == BitVector(32, static_cast<uint64_t>(1)));
}

TEST(BitVectorTest, SignedModulus) {
  BitVector bv1(32, static_cast<int64_t>(-10));
  BitVector bv2(32, static_cast<int64_t>(3));
  BitVector result = bv1.mod_by<true>(bv2);
  EXPECT_EQ(result.get_bitwidth(), 32);
  EXPECT_TRUE(result == BitVector(32, static_cast<int64_t>(-1)));
}

TEST(BitVectorTest, IsPowerOfTwo) {
  BitVector bv1(32);
  bv1.set_bit(0, true);
  EXPECT_TRUE(bv1.is_power_of_two());
  bv1.set_bit(1, true);
  EXPECT_FALSE(bv1.is_power_of_two());
}

TEST(BitVectorTest, FastPower) {
  BitVector bv(32, static_cast<uint64_t>(1));
  BitVector exp(16, static_cast<uint64_t>(8));
  BitVector result = bv.fast_power(exp);  // 1^8 = 1
  EXPECT_TRUE(result == BitVector(32, static_cast<uint64_t>(1)));

  bv.set_bit(1, true);          // bv = 3
  exp.set_bit(0, true);         // exp = 9
  result = bv.fast_power(exp);  // 3^9 = 19683
  EXPECT_TRUE(result == BitVector(32, static_cast<uint64_t>(19683)));
}

TEST(BitVectorTest, ZeroExtendBy) {
  BitVector bv(8);
  bv.set_bit(0, true);
  bv.set_bit(7, true);  // bv = 10000001
  BitVector extended = bv.zero_extend_by(8);
  BitVector expected(16);
  expected.set_bit(0, true);
  expected.set_bit(7, true);
  EXPECT_EQ(extended.get_bitwidth(), 16);
  EXPECT_TRUE(extended == expected);
}

TEST(BitVectorTest, SignExtendBy) {
  BitVector bv = BitVector::min_value<true>(8);
  BitVector extended = bv.sign_extend_by(8);
  BitVector expected(16, 0xFF80ULL);
  EXPECT_EQ(extended.get_bitwidth(), 16);
  EXPECT_TRUE(extended == expected);

  bv = BitVector::max_value<true>(8);
  extended = bv.sign_extend_by(8);
  expected = BitVector(16, static_cast<int64_t>(127));
  EXPECT_EQ(extended.get_bitwidth(), 16);
  EXPECT_TRUE(extended == expected);
}

TEST(BitVectorTest, ComputeSize) {
  EXPECT_EQ(BitVector::get_size(32), 1);
  EXPECT_EQ(BitVector::get_size(64), 2);
  EXPECT_EQ(BitVector::get_size(1), 1);
  EXPECT_EQ(BitVector::get_size(33), 2);
}

TEST(BitVectorTest, GetMask) {
  EXPECT_EQ(BitVector::get_mask(32), 0xFFFFFFFF);
  EXPECT_EQ(BitVector::get_mask(31), 0x7FFFFFFF);
  EXPECT_EQ(BitVector::get_mask(1), 0x1);
}

TEST(BitVectorTest, ToUint64CornerCases) {
  BitVector bv1(65, 0xFFFFFFFFFFFFFFFF);
  auto [value1, success1] = bv1.to_uint64();
  EXPECT_TRUE(success1);
  EXPECT_EQ(value1, std::numeric_limits<uint64_t>::max());

  BitVector bv2(64, 0xFFFFFFFFFFFFFFFF);
  auto [value2, success2] = bv2.to_uint64();
  EXPECT_TRUE(success2);
  EXPECT_EQ(value2, 0xFFFFFFFFFFFFFFFF);
}

TEST(BitVectorTest, ToInt64CornerCases) {
  BitVector bv1(65, static_cast<int64_t>(-1));
  auto [value1, success1] = bv1.to_int64();
  EXPECT_TRUE(success1);
  EXPECT_EQ(value1, -1);

  BitVector bv2(64, static_cast<int64_t>(-1));
  auto [value2, success2] = bv2.to_int64();
  EXPECT_TRUE(success2);
  EXPECT_EQ(value2, -1);

  BitVector bv3(33, static_cast<int64_t>(-128));
  auto [value3, success3] = bv3.to_int64();
  EXPECT_TRUE(success3);
  EXPECT_EQ(value3, -128);
}

TEST(BitVectorTest, OverflowCases) {
  BitVector bv1(32, static_cast<uint64_t>(0xFFFFFFFF));
  BitVector bv2(32, static_cast<uint64_t>(1));
  BitVector result = bv1 + bv2;
  EXPECT_TRUE(result.is_zero());

  BitVector bv3(32, static_cast<uint64_t>(0));
  BitVector bv4(32, static_cast<uint64_t>(1));
  result = bv3 - bv4;
  BitVector expected(32, static_cast<uint64_t>(-1));
  EXPECT_TRUE(result == expected);
}

TEST(BitVectorTest, MinValue) {
  BitVector bv1 = BitVector::min_value<true>(32);
  BitVector expected1(32);
  expected1.set_bit(31, true);
  EXPECT_EQ(bv1.get_bitwidth(), 32);
  EXPECT_TRUE(bv1 == expected1);

  BitVector bv2 = BitVector::min_value<false>(32);
  BitVector expected2(32);
  EXPECT_EQ(bv2.get_bitwidth(), 32);
  EXPECT_TRUE(bv2 == expected2);
}

TEST(BitVectorTest, MaxValue) {
  BitVector bv1 = BitVector::max_value<true>(32);
  BitVector expected1(32);
  for (uint32_t i = 0; i < 31; ++i) {
    expected1.set_bit(i, true);
  }
  EXPECT_EQ(bv1.get_bitwidth(), 32);
  EXPECT_TRUE(bv1 == expected1);

  BitVector bv2 = BitVector::max_value<false>(32);
  BitVector expected2(32);
  for (uint32_t i = 0; i < 32; ++i) {
    expected2.set_bit(i, true);
  }
  EXPECT_EQ(bv2.get_bitwidth(), 32);
  EXPECT_TRUE(bv2 == expected2);
}

TEST(BitVectorTest, UnaryNegation) {
  BitVector bv1(100, static_cast<int64_t>(1));
  BitVector neg_bv1 = -bv1;
  BitVector expected(100, static_cast<int64_t>(-1));
  EXPECT_EQ(neg_bv1.get_bitwidth(), 100);
  EXPECT_TRUE(neg_bv1 == expected);

  BitVector bv2(100, static_cast<int64_t>(-1));
  BitVector neg_bv2 = -bv2;
  BitVector expected2(100, static_cast<int64_t>(1));
  EXPECT_EQ(neg_bv2.get_bitwidth(), 100);
  EXPECT_TRUE(neg_bv2 == expected2);
}

TEST(BitVectorTest, InPlaceNegation) {
  BitVector bv1(100, static_cast<int64_t>(1));
  bv1.negate();
  BitVector expected(100, static_cast<int64_t>(-1));
  EXPECT_EQ(bv1.get_bitwidth(), 100);
  EXPECT_TRUE(bv1 == expected);

  BitVector bv2(100, static_cast<int64_t>(-1));
  bv2.negate();
  BitVector expected2(100, static_cast<int64_t>(1));
  EXPECT_EQ(bv2.get_bitwidth(), 100);
  EXPECT_TRUE(bv2 == expected2);
}

TEST(BitVectorTest, UnsignedComparison) {
  BitVector bv1(32, static_cast<uint64_t>(10));
  BitVector bv2(32, static_cast<uint64_t>(20));
  EXPECT_TRUE(bv1.lt<false>(bv2));
  EXPECT_TRUE(bv1.le<false>(bv2));
  EXPECT_FALSE(bv1.gt<false>(bv2));
  EXPECT_FALSE(bv1.ge<false>(bv2));
  EXPECT_FALSE(bv1 == bv2);
  EXPECT_TRUE(bv1 != bv2);

  BitVector bv3(32, static_cast<uint64_t>(10));
  EXPECT_TRUE(bv1 == bv3);
  EXPECT_FALSE(bv1 != bv3);
}

TEST(BitVectorTest, SignedComparison) {
  BitVector bv1(32, static_cast<int64_t>(-10));
  BitVector bv2(32, static_cast<int64_t>(10));
  EXPECT_TRUE(bv1.lt<true>(bv2));
  EXPECT_TRUE(bv1.le<true>(bv2));
  EXPECT_FALSE(bv1.gt<true>(bv2));
  EXPECT_FALSE(bv1.ge<true>(bv2));
  EXPECT_FALSE(bv1 == bv2);
  EXPECT_TRUE(bv1 != bv2);

  BitVector bv3(32, static_cast<int64_t>(-10));
  EXPECT_TRUE(bv1 == bv3);
  EXPECT_FALSE(bv1 != bv3);
}

TEST(BitVectorTest, WidthGreaterThan64) {
  BitVector bv(128, static_cast<uint64_t>(0xFFFFFFFFFFFFFFFF));
  BitVector expected(128);
  for (uint32_t i = 0; i < 64; ++i) {
    expected.set_bit(i, true);
  }
  EXPECT_TRUE(bv == expected);

  BitVector bv2(128, static_cast<int64_t>(-1));
  BitVector expected2(128);
  for (uint32_t i = 0; i < 128; ++i) {
    expected2.set_bit(i, true);
  }
  EXPECT_TRUE(bv2 == expected2);
}

TEST(BitVectorTest, ToString) {
  BitVector bv1(32, static_cast<uint64_t>(0x12345678));
  EXPECT_EQ(bv1.to_string(), "0x12345678");

  BitVector bv2(64, static_cast<uint64_t>(0xFFFFFFFFFFFFFFFF));
  EXPECT_EQ(bv2.to_string(), "0xffffffffffffffff");

  BitVector bv3(128, static_cast<uint64_t>(0xFFFFFFFFFFFFFFFF));
  EXPECT_EQ(bv3.to_string(), "0x00000000ffffffffffffffff");
}

TEST(BitVectorTest, OstreamOperator) {
  BitVector bv1(32, static_cast<uint64_t>(0x12345678));
  std::ostringstream oss;
  oss << bv1;
  EXPECT_EQ(oss.str(), "0x12345678");

  BitVector bv2(64, static_cast<uint64_t>(0xFFFFFFFFFFFFFFFF));
  oss.str("");
  oss << bv2;
  EXPECT_EQ(oss.str(), "0xffffffffffffffff");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
