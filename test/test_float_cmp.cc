#include <gtest/gtest.h>
#include "lal/float_cmp.hh"

TEST(FloatCmpTest, HandlesSmallValues) {
	EXPECT_TRUE(lal::floatEqual(0.000012, 0.000012));
}
