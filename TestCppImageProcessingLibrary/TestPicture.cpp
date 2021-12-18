#include "pch.h"
#include "Picture.h"
#include "ArrayFunctions.h"

TEST(TestPicture, TestDefaultConstructor) {
	/*
		Test Default Constructor = Empty image
		Empty PicturePGM: height=0, width=0, max_value=0, map=nullptr;
	*/

	PicturePGM emptyPicture;
	EXPECT_EQ(emptyPicture.height, 0);
	EXPECT_EQ(emptyPicture.width, 0);
	EXPECT_EQ(emptyPicture.size, 0);
	EXPECT_EQ(emptyPicture.max_value, 0);
	EXPECT_EQ(emptyPicture.map, nullptr);
}

TEST(TestPicture, TestConstructor) {
	/*
		Test Constructor with arguments
	*/

	const uint32_t height = 9, width = 9;
	const uint32_t size = height * width;
	const uint8_t max_value = 128;
	float** examplePictureMap = nullptr;
	auto result = create2dArray<float, uint32_t>(&examplePictureMap, height, width);
	if (!result)
		EXPECT_EQ(1, 0);
	
	PicturePGM emptyPicture{height, width, size, max_value, examplePictureMap};
	EXPECT_EQ(emptyPicture.height, height);
	EXPECT_EQ(emptyPicture.width, width);
	EXPECT_EQ(emptyPicture.size, size);
	EXPECT_EQ(emptyPicture.max_value, max_value);
	EXPECT_NE(emptyPicture.map, nullptr);
}

TEST(TestPicture, TestCopyConstructor)
{
	/*
		Test Copy Constructor
	*/

	const uint32_t height = 9, width = 9;
	const uint32_t size = height * width;
	const uint8_t max_value = 128;

	float** examplePictureMap = nullptr;
	auto result = create2dArray<float, uint32_t>(&examplePictureMap, height, width);
	if (!result)
		EXPECT_EQ(1, 0);

	PicturePGM newPicture{ height, width, size, max_value, examplePictureMap };
	PicturePGM otherPicture{ newPicture };
	EXPECT_EQ(otherPicture.height, newPicture.height);
	EXPECT_EQ(otherPicture.width, newPicture.width);
	EXPECT_EQ(otherPicture.max_value, newPicture.max_value);
	EXPECT_EQ(otherPicture.size, newPicture.size);

	for(uint32_t r=0; r<otherPicture.height; ++r)
		for(uint32_t c=0; c<otherPicture.width; ++c)
			EXPECT_EQ(otherPicture.map[r][c], newPicture.map[r][c]);
}

TEST(TestPicture, TestMoveConstructor)
{
	/*
		Test Move Constructor
	*/

	const uint32_t height = 9, width = 9;
	const uint32_t size = height * width;
	const uint8_t max_value = 0;

	float** examplePictureMap = nullptr;
	auto result = create2dArray<float, uint32_t>(&examplePictureMap, height, width);
	if (!result)
		EXPECT_EQ(1, 0);

	PicturePGM newPicture(PicturePGM{ height, width, size, max_value, examplePictureMap });
	EXPECT_EQ(newPicture.height, height);
	EXPECT_EQ(newPicture.width, width);
	EXPECT_EQ(newPicture.size, size);
	EXPECT_EQ(newPicture.max_value, max_value);
	EXPECT_NE(newPicture.map, nullptr);
	
	for (uint32_t r = 0; r < newPicture.height; ++r)
		for (uint32_t c = 0; c < newPicture.width; ++c)
			EXPECT_EQ(examplePictureMap[r][c], newPicture.map[r][c]);
}

TEST(TestPicture, TestCopyAssignmentWithDifferentSize)
{
	/*
		Test Copy Assignment,
		like pic1 = pic2;
		pic1 has different height, width and size than pic2
	*/
	const uint32_t height = 9, width = 9;
	const uint32_t size = height * width;
	const uint8_t max_value = 0;

	float** examplePictureMap = nullptr;
	auto result = create2dArray<float, uint32_t>(&examplePictureMap, height, width);
	if (!result)
		EXPECT_EQ(1, 0);

	PicturePGM pic1;
	PicturePGM pic2 { height, width, size, max_value, examplePictureMap };
	pic1 = pic2;

	EXPECT_EQ(pic1.height, pic2.height);
	EXPECT_EQ(pic1.width, pic2.width);
	EXPECT_EQ(pic1.size, pic2.size);
	EXPECT_EQ(pic1.max_value, pic2.max_value);
	EXPECT_NE(pic1.map, nullptr);

	for (uint32_t r = 0; r < pic1.height; ++r)
		for (uint32_t c = 0; c < pic1.width; ++c)
			EXPECT_EQ(pic1.map[r][c], pic2.map[r][c]);
}

TEST(TestPicture, TestCopyAssignmentWithSameSize)
{
	/*
		Test Copy Assignment,
		like pic1 = pic2;
		üic1 and pic2 have the same size
	*/
	const uint32_t height = 9, width = 9;
	const uint32_t size = height * width;
	const uint8_t max_valuePic1 = 0;
	const uint8_t max_valuePic2 = 8;

	float** examplePictureMapPic1 = nullptr;
	auto result = create2dArray<float, uint32_t>(&examplePictureMapPic1, height, width);
	if (!result)
		EXPECT_EQ(1, 0);

	float** examplePictureMapPic2 = nullptr;
	result = create2dArray<float, uint32_t>(&examplePictureMapPic2, height, width);
	if (!result)
		EXPECT_EQ(1, 0);
	for (uint32_t r = 0; r < height; ++r)
		for (uint32_t c = 0; c < width; ++c)
			examplePictureMapPic2[r][c] = max_valuePic2;

	PicturePGM pic1{ height, width, size, max_valuePic1, examplePictureMapPic1 };
	PicturePGM pic2{ height, width, size, max_valuePic2, examplePictureMapPic2};
	pic1 = pic2;

	EXPECT_EQ(pic1.height, pic2.height);
	EXPECT_EQ(pic1.width, pic2.width);
	EXPECT_EQ(pic1.size, pic2.size);
	EXPECT_EQ(pic1.max_value, pic2.max_value);
	EXPECT_NE(pic1.map, nullptr);

	for (uint32_t r = 0; r < pic1.height; ++r)
		for (uint32_t c = 0; c < pic1.width; ++c)
			EXPECT_EQ(pic1.map[r][c], pic2.map[r][c]);
}

TEST(TestPicture, TestMoveAssignment)
{
	/*
		Test Move Assignment
		pic1 = std::move(pic2);
		pic2 should be set to valid state
	*/
	const uint32_t height = 9, width = 9;
	const uint32_t size = height * width;
	const uint8_t max_value = 8;

	float** examplePictureMapPic2 = nullptr;
	auto result = create2dArray<float, uint32_t>(&examplePictureMapPic2, height, width);
	if (!result)
		EXPECT_EQ(1, 0);
	for (uint32_t r = 0; r < height; ++r)
		for (uint32_t c = 0; c < width; ++c)
			examplePictureMapPic2[r][c] = max_value;

	PicturePGM pic1;
	PicturePGM pic2{ height, width, size, max_value, examplePictureMapPic2 };

	pic1 = std::move(pic2);
	EXPECT_EQ(pic1.height, height);
	EXPECT_EQ(pic1.width, width);
	EXPECT_EQ(pic1.size, size);
	EXPECT_EQ(pic1.max_value, max_value);
	EXPECT_NE(pic1.map, nullptr);

	for (uint32_t r = 0; r < pic1.height; ++r)
		for (uint32_t c = 0; c < pic1.width; ++c)
			EXPECT_EQ(pic1.map[r][c], examplePictureMapPic2[r][c]);

	EXPECT_EQ(pic2.height, 0);
	EXPECT_EQ(pic2.width, 0);
	EXPECT_EQ(pic2.size, 0);
	EXPECT_EQ(pic2.max_value, 0);
	EXPECT_EQ(pic2.map, nullptr);
}

TEST(TestPicture, TestMakePaddingDefault)
{
	/*
		Test Make Padding around image with default 1 pixel frame
		pic.make_padding();
		picture should be extended with padding with default frame with of 1 pixel
		Example:
					0 0 0 0
		 8 8		0 8 8 0
		 8 8		0 8 8 0
					0 0 0 0
		height += 2, width += 2
		size = (height+2) * (width+2)
	*/

	const uint32_t height = 2, width = 2;
	const uint32_t size = height * width;
	const uint8_t max_value = 8;
	const uint8_t padding = 1;

	float** examplePictureMapPic = nullptr;
	auto result = create2dArray<float, uint32_t>(&examplePictureMapPic, height, width);
	if (!result)
		EXPECT_EQ(1, 0);
	for (uint32_t r = 0; r < height; ++r)
		for (uint32_t c = 0; c < width; ++c)
			examplePictureMapPic[r][c] = max_value;

	PicturePGM pic{ height, width, size, max_value, examplePictureMapPic };
	pic.make_padding();
	EXPECT_EQ(pic.height, height+padding*2);
	EXPECT_EQ(pic.width, width+padding*2);
	EXPECT_EQ(pic.size, (height+padding*2) * (width+padding*2));
	EXPECT_EQ(pic.max_value, max_value);
	EXPECT_NE(pic.map, nullptr);

	// check if first col and last col has 0 padding
	for (uint32_t r = 0; r < pic.height; ++r)
	{
		EXPECT_EQ(pic.map[r][0], 0);
		EXPECT_EQ(pic.map[r][pic.width-padding], 0);
	}

	// check if first row and last row has 0 padding
	for (uint32_t c = 0; c < pic.width; ++c)
	{
		EXPECT_EQ(pic.map[0][c], 0);
		EXPECT_EQ(pic.map[pic.height-padding][c], 0);
	}

	// check if original image values are unmodified
	for (uint32_t r = padding; r < pic.height - padding; ++r)
		for (uint32_t c = padding; c < pic.width - padding; ++c)
			EXPECT_EQ(pic.map[r][c], max_value);
}

TEST(TestPicture, TestMakePaddingWith7PixelPadding)
{
	/*
		Test Make Padding around image with 7 pixel frame
		pic.make_padding(7);
		picture should be extended with padding with default frame with of 7 pixel
		Example:
					0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
					0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
					0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
					0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
					0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
					0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
					0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
		 8 8		0 0 0 0 0 0 0 8 8 0 0 0 0 0 0 0
		 8 8		0 0 0 0 0 0 0 8 8 0 0 0 0 0 0 0
					0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
					0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
					0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
					0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
					0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
					0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
					0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		height += 14, width += 14
		size = (height+14) * (width+14)
	*/

	const uint32_t height = 2, width = 2;
	const uint32_t size = height * width;
	const uint8_t max_value = 8;
	const uint8_t padding = 7;

	float** examplePictureMapPic = nullptr;
	auto result = create2dArray<float, uint32_t>(&examplePictureMapPic, height, width);
	if (!result)
		EXPECT_EQ(1, 0);
	for (uint32_t r = 0; r < height; ++r)
		for (uint32_t c = 0; c < width; ++c)
			examplePictureMapPic[r][c] = max_value;

	PicturePGM pic{ height, width, size, max_value, examplePictureMapPic };
	pic.make_padding(7);

	EXPECT_EQ(pic.height, height + padding * 2);
	EXPECT_EQ(pic.width, width + padding * 2);
	EXPECT_EQ(pic.size, (height + padding * 2) * (width + padding * 2));
	EXPECT_EQ(pic.max_value, max_value);
	EXPECT_NE(pic.map, nullptr);

	// check if first col and last col has 0 padding
	for (uint32_t r = 0; r < pic.height; ++r)
	{
		for (uint32_t c = 0; c < padding; ++c)
		{
			EXPECT_EQ(pic.map[r][c], 0);
			EXPECT_EQ(pic.map[r][pic.width - padding + c], 0);
		}
	}

	// check if first row and last row has 0 padding
	for (uint32_t c = 0; c < pic.height; ++c)
	{
		for (uint32_t r = 0; r < padding; ++r)
		{
			EXPECT_EQ(pic.map[r][c], 0);
			EXPECT_EQ(pic.map[pic.width - padding + r][c], 0);
		}
	}

	// check if original image values are unmodified
	for (uint32_t r = padding; r < pic.height - padding; ++r)
		for (uint32_t c = padding; c < pic.width - padding; ++c)
			EXPECT_EQ(pic.map[r][c], max_value);
}

TEST(TestPicture, TestMakePaddingWithIllegalPaddingOption)
{
	/*
		Test Make Padding around image with illegal padding argument
		pic.make_padding(8);
		picture should not be extended and still remains the same
		Example:
		 8 8	8 8
		 8 8	8 8
		height=oldHeight, width=oldWidth
		size = height * width
	*/

	const uint32_t height = 2, width = 2;
	const uint32_t size = height * width;
	const uint8_t max_value = 8;
	const uint8_t illegalPadding = 8;

	float** examplePictureMapPic = nullptr;
	auto result = create2dArray<float, uint32_t>(&examplePictureMapPic, height, width);
	if (!result)
		EXPECT_EQ(1, 0);
	for (uint32_t r = 0; r < height; ++r)
		for (uint32_t c = 0; c < width; ++c)
			examplePictureMapPic[r][c] = max_value;

	PicturePGM pic{ height, width, size, max_value, examplePictureMapPic };
	pic.make_padding(illegalPadding);

	// check if picture is unmodified because of illegal padding argument
	EXPECT_EQ(pic.height, height);
	EXPECT_EQ(pic.width, width);
	EXPECT_EQ(pic.size, height * width);
	EXPECT_EQ(pic.max_value, max_value);
	EXPECT_NE(pic.map, nullptr);

}

TEST(TestPicture, TestRemovePadding)
{
	/*
		Test remove padding around image
		pic.removePadding();
		picture padding should be removed according to 
		the padding size passed as argument into the method (default 1)
		Example:
		0 0 0 0
		0 8 8 0		8 8 
		0 8 8 0		8 8
		0 0 0 0
		height=oldHeight, width=oldWidth
		size = height * width
	*/
	const uint32_t oldHeight = 2, oldWidth = 2;
	const uint32_t oldSize = oldHeight * oldWidth;
	const uint32_t height = 4, width = 4;
	const uint32_t size = height * width;
	const uint8_t max_value = 8;

	float** examplePictureMapPic = nullptr;
	auto result = create2dArray<float, uint32_t>(&examplePictureMapPic, height, width);
	if (!result)
		EXPECT_EQ(1, 0);
	for (uint32_t r = 0; r < height; ++r)
		for (uint32_t c = 0; c < width; ++c)
			examplePictureMapPic[r][c] = 0;

	for (uint32_t r = 1; r <= oldHeight; ++r)
		for (uint32_t c = 1; c <= oldWidth; ++c)
			examplePictureMapPic[r][c] = max_value;

	PicturePGM pic{ height, width, size, max_value, examplePictureMapPic };

	// remove the padding
	pic.removePadding();

	EXPECT_EQ(pic.height, oldHeight);
	EXPECT_EQ(pic.width, oldWidth);
	EXPECT_EQ(pic.size, oldSize);
	EXPECT_EQ(pic.max_value, max_value);
	EXPECT_NE(pic.map, nullptr);
	for(uint32_t r = 0; r < oldHeight; ++r)
		for(uint32_t c = 0; c < oldWidth; ++c)
			EXPECT_EQ(pic.map[r][c], max_value);

}