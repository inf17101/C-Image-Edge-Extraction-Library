#include "pch.h"
#include "ArrayFunctions.h"

TEST(TestArrayFunctions, TestCreateAndDelete2dArray)
{
	float** map = nullptr;
	auto result = create2dArray<float, uint32_t>(&map, 9, 9);
	EXPECT_TRUE(result);
	EXPECT_NE(map, nullptr);

	delete2dArray(&map, 9);
	EXPECT_EQ(map, nullptr);
}