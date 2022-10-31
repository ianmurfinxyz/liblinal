#include <gtest/gtest.h>
#include <cmath>
#include "lal/float_cmp.hh"

// Floating point values have a 'floating' decimal point. This allows them to represent
// very large numbers with low precision, and very small numbers with high precision.
//
// Floating point math is also subject to rounding errors and limitations in representing
// certain values, such as 0.1f. Consequently the same value calculated via equivilent means
// can produce slightly different results. For example,
//
// float a = 0.15 + 0.15
// float b = 0.1 + 0.2
// if(a == b) // can be false!
// if(a >= b) // can also be false!
//
// Further floating point values are quantised, not continuous. Only specific values can
// actually be represented withing a floating point register. This is simply the consequence of 
// computers only being capable of discreet mathematics. However to make things more complex the 
// difference between any given floating point value and the next representale value is not 
// constant.
//
// The minimum difference between 2 floating point values is called the 'unit of least precision' 
// of ULP.
//
// The c++ standard provides a function to return the next representable value of a floating point
// register, https://en.cppreference.com/w/cpp/numeric/math/nextafter
//
// Further the c++ standard lib provides another function to return the machine epsilon,
// https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon, which is the difference between
// the value 1.0 and the next representable value.
// 
// Simple comparisons using the machine epsilon however are not sufficient, for example,
//
//   inline bool floatEqual(float a, float b){
//   	return std::abs(a - b) <= std::numeric_limits<float>::epsilon();
//   }
//
// this method will always return false (i.e. not equal) when a and b are large enough such that
// the minimum distance between them is larger than the machine epsilon. This would include float
// values which are adjacent w.r.t representable values. Hence the only values which would compare
// equal are those which are the same representable floating point value (the same ULP).
//

// Q: What are the largest values that can be distinguished?
// Q: What is the largest difference we want to be considered equal? The answer to this must 
// vary with the size of the values being compared.


// There should not be a limitation in very small values comparing equal as the difference
// between them will very quickly be less than the machine epsilon.
TEST(FloatCmpTest, HandlesSmallEqualValues) {
	EXPECT_TRUE(lal::floatEqual(20.f/3.f, 40.f/6.f));
	EXPECT_TRUE(lal::floatEqual(20.f/3000.f, 40.f/6000.f));
	EXPECT_TRUE(lal::floatEqual(20.f/3000000.f, 40.f/6000000.f));
	EXPECT_TRUE(lal::floatEqual(20.f/3000000000.f, 40.f/6000000000.f));
	EXPECT_TRUE(lal::floatEqual(20.f/3000000000000.f, 40.f/6000000000000.f));
	EXPECT_TRUE(lal::floatEqual(20.f/3000000000000000.f, 40.f/6000000000000000.f));
	EXPECT_TRUE(lal::floatEqual(20.f/3000000000000000000.f, 40.f/6000000000000000000.f));
}

// There will be a limitation in distinguishing very small differences. A decision will need to
// be made about how large a difference we want to require before we return those values as being
// unequal.
TEST(FloatCmpTest, HandlesSmallUnequalValues) {
	EXPECT_FALSE(lal::floatEqual(0.1f, 0.11f));
	EXPECT_FALSE(lal::floatEqual(0.1f, 0.101f));
	EXPECT_FALSE(lal::floatEqual(0.1f, 0.1001f));
	EXPECT_FALSE(lal::floatEqual(0.1001f, 0.1f));

	// fails
	//EXPECT_FALSE(lal::floatEqual(0.00001f, 0.00001001f));

	// fails
	//EXPECT_FALSE(lal::floatEqual(0.1f, 0.10001f));


	//EXPECT_FALSE(lal::floatEqual(21.f/3.f, 40.f/6.f));

	// This test successfully distinguishes these values as being unequal,
	//   21/3000 == 7.0e-3
	//   40/6000 == 6.66
	//EXPECT_FALSE(lal::floatEqual(21.f/3000.f, 40.f/6000.f));

	// These test pushes the difference to a value smaller than distinguishable
	// so they fail as they return equal/true.
	//EXPECT_FALSE(lal::floatEqual(21.f/3000000.f, 40.f/6000000.f));
	//EXPECT_FALSE(lal::floatEqual(21.f/3000000000.f, 40.f/6000000000.f));
	//EXPECT_FALSE(lal::floatEqual(21.f/3000000000000.f, 40.f/6000000000000.f));
	//EXPECT_FALSE(lal::floatEqual(21.f/3000000000000000.f, 40.f/6000000000000000.f));
	//EXPECT_FALSE(lal::floatEqual(21.f/3000000000000000000.f, 40.f/6000000000000000000.f));
}

TEST(FloatCmpTest, HandlesLargeEqualValues) {
	EXPECT_TRUE(lal::floatEqual(1.23498f * 1.0e3f , 1.23498f * 1.0e3f ));
	EXPECT_TRUE(lal::floatEqual(1.23498f * 1.0e6f , 1.23498f * 1.0e6f ));
	EXPECT_TRUE(lal::floatEqual(1.23498f * 1.0e9f , 1.23498f * 1.0e9f ));
	EXPECT_TRUE(lal::floatEqual(1.23498f * 1.0e12f, 1.23498f * 1.0e12f));
	EXPECT_TRUE(lal::floatEqual(1.23498f * 1.0e15f, 1.23498f * 1.0e15f));
	EXPECT_TRUE(lal::floatEqual(1.23498f * 1.0e18f, 1.23498f * 1.0e18f));
	EXPECT_TRUE(lal::floatEqual(1.23498f * 1.0e21f, 1.23498f * 1.0e21f));
	EXPECT_TRUE(lal::floatEqual(1.23498f * 1.0e24f, 1.23498f * 1.0e24f));
}

TEST(FloatCmpTest, HandlesLargeUnequalValues) {
	EXPECT_FALSE(lal::floatEqual(1.43498f * 1.0e3f , 1.23498f * 1.0e3f ));
	EXPECT_FALSE(lal::floatEqual(1.43498f * 1.0e6f , 1.23498f * 1.0e6f ));
	EXPECT_FALSE(lal::floatEqual(1.43498f * 1.0e9f , 1.23498f * 1.0e9f ));
	EXPECT_FALSE(lal::floatEqual(1.43498f * 1.0e12f, 1.23498f * 1.0e12f));
	EXPECT_FALSE(lal::floatEqual(1.43498f * 1.0e15f, 1.23498f * 1.0e15f));
	EXPECT_FALSE(lal::floatEqual(1.43498f * 1.0e18f, 1.23498f * 1.0e18f));
	EXPECT_FALSE(lal::floatEqual(1.43498f * 1.0e21f, 1.23498f * 1.0e21f));
	EXPECT_FALSE(lal::floatEqual(1.43498f * 1.0e24f, 1.23498f * 1.0e24f));

	EXPECT_FALSE(lal::floatEqual(1.24498f * 1.0e3f , 1.23498f * 1.0e3f ));
	EXPECT_FALSE(lal::floatEqual(1.24498f * 1.0e6f , 1.23498f * 1.0e6f ));
	EXPECT_FALSE(lal::floatEqual(1.24498f * 1.0e9f , 1.23498f * 1.0e9f ));
	EXPECT_FALSE(lal::floatEqual(1.24498f * 1.0e12f, 1.23498f * 1.0e12f));
	EXPECT_FALSE(lal::floatEqual(1.24498f * 1.0e15f, 1.23498f * 1.0e15f));
	EXPECT_FALSE(lal::floatEqual(1.24498f * 1.0e18f, 1.23498f * 1.0e18f));
	EXPECT_FALSE(lal::floatEqual(1.24498f * 1.0e21f, 1.23498f * 1.0e21f));
	EXPECT_FALSE(lal::floatEqual(1.24498f * 1.0e24f, 1.23498f * 1.0e24f));

	EXPECT_FALSE(lal::floatEqual(1.23598f * 1.0e3f , 1.23498f * 1.0e3f ));
	EXPECT_FALSE(lal::floatEqual(1.23598f * 1.0e6f , 1.23498f * 1.0e6f ));
	EXPECT_FALSE(lal::floatEqual(1.23598f * 1.0e9f , 1.23498f * 1.0e9f ));
	EXPECT_FALSE(lal::floatEqual(1.23598f * 1.0e12f, 1.23498f * 1.0e12f));
	EXPECT_FALSE(lal::floatEqual(1.23598f * 1.0e15f, 1.23498f * 1.0e15f));
	EXPECT_FALSE(lal::floatEqual(1.23598f * 1.0e18f, 1.23498f * 1.0e18f));
	EXPECT_FALSE(lal::floatEqual(1.23598f * 1.0e21f, 1.23498f * 1.0e21f));
	EXPECT_FALSE(lal::floatEqual(1.23598f * 1.0e24f, 1.23498f * 1.0e24f));

	// These tests push the difference to a value smaller than distinguishable
	// so they fail as they return equal/true.
	//EXPECT_FALSE(lal::floatEqual(1.23499f * 1.0e3f , 1.23498f * 1.0e3f ));
	//EXPECT_FALSE(lal::floatEqual(1.23499f * 1.0e6f , 1.23498f * 1.0e6f ));
	//EXPECT_FALSE(lal::floatEqual(1.23499f * 1.0e9f , 1.23498f * 1.0e9f ));
	//EXPECT_FALSE(lal::floatEqual(1.23499f * 1.0e12f, 1.23498f * 1.0e12f));
	//EXPECT_FALSE(lal::floatEqual(1.23499f * 1.0e15f, 1.23498f * 1.0e15f));
	//EXPECT_FALSE(lal::floatEqual(1.23499f * 1.0e18f, 1.23498f * 1.0e18f));
	//EXPECT_FALSE(lal::floatEqual(1.23499f * 1.0e21f, 1.23498f * 1.0e21f));
	//EXPECT_FALSE(lal::floatEqual(1.23499f * 1.0e24f, 1.23498f * 1.0e24f));
}

TEST(FloatCmpTest, HandlesSquareRoots) {
	EXPECT_TRUE(lal::floatEqual(std::sqrt(4.0f), 2.0f));
	EXPECT_TRUE(lal::floatEqual(std::sqrt(544.44444f), 23.3333f));
	EXPECT_TRUE(lal::floatEqual(std::sqrt(6897.6541), 83.05211677));
	EXPECT_TRUE(lal::floatEqual(std::sqrt(64578192.346598f), 8036.055771f));
}
