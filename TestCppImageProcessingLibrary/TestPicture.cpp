#include "pch.h"
#include "Picture.h"

TEST(TestPicture, TestDefaultConstructor) {
	/*
		Test Default Constructor = Empty image
		Empty PicturePGM: height=0, width=0, max_value=0;
	*/

	const PicturePGM emptyPicture;
	EXPECT_EQ(emptyPicture.getHeight(), 0);
	EXPECT_EQ(emptyPicture.getWidth(), 0);
	EXPECT_EQ(emptyPicture.getSize(), 0);
	EXPECT_EQ(emptyPicture.getMaxValue(), 0);
}

TEST(TestPicture, TestConstructor) {
	/*
		Test Constructor with arguments
	*/

	const uint32_t height = 9, width = 9;
	const uint32_t size = height * width;
	const uint8_t max_value = 128;
	const std::vector<std::vector<float>> examplePictureMap{ height, std::vector<float>{width} };
	
	PicturePGM emptyPicture{height, width, size, max_value, examplePictureMap};
	EXPECT_EQ(emptyPicture.getHeight(), height);
	EXPECT_EQ(emptyPicture.getWidth(), width);
	EXPECT_EQ(emptyPicture.getSize(), size);
	EXPECT_EQ(emptyPicture.getMaxValue(), max_value);
}

TEST(TestPicture, TestCopyConstructor)
{
	/*
		Test Copy Constructor
	*/

	const uint32_t height = 9, width = 9;
	const uint32_t size = height * width;
	const uint8_t max_value = 128;

	const std::vector<std::vector<float>> examplePictureMap( height, std::vector<float>(width));
	PicturePGM newPicture{ height, width, size, max_value, examplePictureMap };
	PicturePGM otherPicture{ newPicture };
	EXPECT_EQ(otherPicture.getHeight(), newPicture.getHeight());
	EXPECT_EQ(otherPicture.getWidth(), newPicture.getWidth());
	EXPECT_EQ(otherPicture.getMaxValue(), newPicture.getMaxValue());
	EXPECT_EQ(otherPicture.getSize(), newPicture.getSize());

	for(uint32_t r=0; r<otherPicture.getHeight(); ++r)
		for(uint32_t c=0; c<otherPicture.getWidth(); ++c)
			EXPECT_EQ(otherPicture.get(r, c), newPicture.get(r, c));
}

TEST(TestPicture, TestMoveConstructor)
{
	/*
		Test Move Constructor
	*/

	const uint32_t height = 9, width = 9;
	const uint32_t size = height * width;
	const uint8_t max_value = 0;
	const std::vector<std::vector<float>>examplePictureMap(height, std::vector<float>(width));


	PicturePGM newPicture(PicturePGM{ height, width, size, max_value, examplePictureMap });
	EXPECT_EQ(newPicture.getHeight(), height);
	EXPECT_EQ(newPicture.getWidth(), width);
	EXPECT_EQ(newPicture.getSize(), size);
	EXPECT_EQ(newPicture.getMaxValue(), max_value);
	
	for (uint32_t r = 0; r < newPicture.getHeight(); ++r)
		for (uint32_t c = 0; c < newPicture.getWidth(); ++c)
			EXPECT_EQ(examplePictureMap[r][c], newPicture.get(r, c));
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
	const std::vector<std::vector<float>> examplePictureMap( height, std::vector<float>(width));


	PicturePGM pic1;
	PicturePGM pic2 { height, width, size, max_value, examplePictureMap };
	pic1 = pic2;

	EXPECT_EQ(pic1.getHeight(), pic2.getHeight());
	EXPECT_EQ(pic1.getWidth(), pic2.getWidth());
	EXPECT_EQ(pic1.getSize(), pic2.getSize());
	EXPECT_EQ(pic1.getMaxValue(), pic2.getMaxValue());

	for (uint32_t r = 0; r < pic1.getHeight(); ++r)
		for (uint32_t c = 0; c < pic1.getWidth(); ++c)
			EXPECT_EQ(pic1.get(r, c), pic2.get(r, c));
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

	const std::vector<std::vector<float>> examplePictureMapPic1(height, std::vector<float>(width, max_valuePic1));
	const std::vector<std::vector<float>> examplePictureMapPic2(height, std::vector<float>(width, max_valuePic2));

	PicturePGM pic1{ height, width, size, max_valuePic1, examplePictureMapPic1 };
	PicturePGM pic2{ height, width, size, max_valuePic2, examplePictureMapPic2 };
	pic1 = pic2;

	EXPECT_EQ(pic1.getHeight(), pic2.getHeight());
	EXPECT_EQ(pic1.getWidth(), pic2.getWidth());
	EXPECT_EQ(pic1.getSize(), pic2.getSize());
	EXPECT_EQ(pic1.getMaxValue(), pic2.getMaxValue());

	for (uint32_t r = 0; r < pic1.getHeight(); ++r)
		for (uint32_t c = 0; c < pic1.getWidth(); ++c)
			EXPECT_EQ(pic1.get(r,c), pic2.get(r,c));
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
	const std::vector<std::vector<float>>examplePictureMapPic2(height, std::vector<float>(width, max_value));

	PicturePGM pic1;
	PicturePGM pic2{ height, width, size, max_value, examplePictureMapPic2 };

	pic1 = std::move(pic2);
	EXPECT_EQ(pic1.getHeight(), height);
	EXPECT_EQ(pic1.getWidth(), width);
	EXPECT_EQ(pic1.getSize(), size);
	EXPECT_EQ(pic1.getMaxValue(), max_value);

	for (uint32_t r = 0; r < pic1.getHeight(); ++r)
		for (uint32_t c = 0; c < pic1.getWidth(); ++c)
			EXPECT_EQ(pic1.get(r,c), examplePictureMapPic2[r][c]);

	EXPECT_EQ(pic2.getHeight(), 0);
	EXPECT_EQ(pic2.getWidth(), 0);
	EXPECT_EQ(pic2.getSize(), 0);
	EXPECT_EQ(pic2.getMaxValue(), 0);
	EXPECT_TRUE(pic2.isEmpty());
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
	const std::vector<std::vector<float>> examplePictureMapPic(height, std::vector<float>(width, max_value));

	PicturePGM pic{ height, width, size, max_value, examplePictureMapPic };
	pic.make_padding();
	EXPECT_EQ(pic.getHeight(), height+padding*2);
	EXPECT_EQ(pic.getWidth(), width+padding*2);
	EXPECT_EQ(pic.getSize(), (height+padding*2) * (width+padding*2));
	EXPECT_EQ(pic.getMaxValue(), max_value);

	// check if first col and last col has 0 padding
	for (uint32_t r = 0; r < pic.getHeight(); ++r)
	{
		EXPECT_EQ(pic.get(r,0), 0);
		EXPECT_EQ(pic.get(r, pic.getWidth()-padding), 0);
	}

	// check if first row and last row has 0 padding
	for (uint32_t c = 0; c < pic.getWidth(); ++c)
	{
		EXPECT_EQ(pic.get(0, c), 0);
		EXPECT_EQ(pic.get(pic.getHeight()-padding, c), 0);
	}

	// check if original image values are unmodified
	for (uint32_t r = padding; r < pic.getHeight() - padding; ++r)
		for (uint32_t c = padding; c < pic.getWidth() - padding; ++c)
			EXPECT_EQ(pic.get(r, c), max_value);
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
	const std::vector<std::vector<float>> examplePictureMapPic(height, std::vector<float>(width, max_value));

	PicturePGM pic{ height, width, size, max_value, examplePictureMapPic };
	pic.make_padding(7);

	EXPECT_EQ(pic.getHeight(), height + padding * 2);
	EXPECT_EQ(pic.getWidth(), width + padding * 2);
	EXPECT_EQ(pic.getSize(), (height + padding * 2) * (width + padding * 2));
	EXPECT_EQ(pic.getMaxValue(), max_value);

	// check if first col and last col has 0 padding
	for (uint32_t r = 0; r < pic.getHeight(); ++r)
	{
		for (uint32_t c = 0; c < padding; ++c)
		{
			EXPECT_EQ(pic.get(r,c), 0);
			EXPECT_EQ(pic.get(r, pic.getWidth() - padding + c), 0);
		}
	}

	// check if first row and last row has 0 padding
	for (uint32_t c = 0; c < pic.getHeight(); ++c)
	{
		for (uint32_t r = 0; r < padding; ++r)
		{
			EXPECT_EQ(pic.get(r,c), 0);
			EXPECT_EQ(pic.get(pic.getWidth() - padding + r, c), 0);
		}
	}

	// check if original image values are unmodified
	for (uint32_t r = padding; r < pic.getHeight() - padding; ++r)
		for (uint32_t c = padding; c < pic.getWidth() - padding; ++c)
			EXPECT_EQ(pic.get(r,c), max_value);
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
	const std::vector<std::vector<float>> examplePictureMapPic(height, std::vector<float>(width, max_value));

	PicturePGM pic{ height, width, size, max_value, examplePictureMapPic };
	pic.make_padding(illegalPadding);

	// check if picture is unmodified because of illegal padding argument
	EXPECT_EQ(pic.getHeight(), height);
	EXPECT_EQ(pic.getWidth(), width);
	EXPECT_EQ(pic.getSize(), height * width);
	EXPECT_EQ(pic.getMaxValue(), max_value);
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

	std::vector<std::vector<float>> examplePictureMapPic(height, std::vector<float>(width, 0.0f));

	for (uint32_t r = 1; r <= oldHeight; ++r)
		for (uint32_t c = 1; c <= oldWidth; ++c)
			examplePictureMapPic[r][c] = max_value;

	PicturePGM pic{ height, width, size, max_value, examplePictureMapPic };

	// remove the padding
	pic.removePadding();

	EXPECT_EQ(pic.getHeight(), oldHeight);
	EXPECT_EQ(pic.getWidth(), oldWidth);
	EXPECT_EQ(pic.getSize(), oldSize);
	EXPECT_EQ(pic.getMaxValue(), max_value);
	for(uint32_t r = 0; r < oldHeight; ++r)
		for(uint32_t c = 0; c < oldWidth; ++c)
			EXPECT_EQ(pic.get(r,c), max_value);
}

TEST(TestPicture, TestRemovePaddingNoDefaultPadding)
{
	/*
		Test remove padding around image
		pic.removePadding(7);
		picture padding should be removed according to
		the padding size passed as argument into the method (padding size = 7)
		Example:
		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 8 8 0 0 0 0 0 0 0			8 8
		0 0 0 0 0 0 0 8 8 0 0 0 0 0 0 0			8 8
		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		height=oldHeight, width=oldWidth
		size = height * width
	*/
	const uint32_t oldHeight = 2, oldWidth = 2;
	const uint32_t oldSize = oldHeight * oldWidth;
	const uint32_t height = 16, width = 16;
	const uint32_t size = height * width;
	const uint8_t max_value = 8;
	const uint8_t paddingSize = 7;

	std::vector<std::vector<float>> examplePictureMapPic(height, std::vector<float>(width, 0.0f));

	for (uint32_t r = 7; r <= 8; ++r)
		for (uint32_t c = 7; c <= 8; ++c)
			examplePictureMapPic[r][c] = max_value;

	PicturePGM pic{ height, width, size, max_value, examplePictureMapPic };

	// remove the padding
	pic.removePadding(paddingSize);

	EXPECT_EQ(pic.getHeight(), oldHeight);
	EXPECT_EQ(pic.getWidth(), oldWidth);
	EXPECT_EQ(pic.getSize(), oldSize);
	EXPECT_EQ(pic.getMaxValue(), max_value);
	for (uint32_t r = 0; r < oldHeight; ++r)
		for (uint32_t c = 0; c < oldWidth; ++c)
			EXPECT_EQ(pic.get(r,c), max_value);
}

TEST(TestPicture, TestRemovePaddingIllegalPaddingSize)
{
	/*
		Test remove padding around image with illegal padding size
		pic.removePadding(10);
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
	const uint8_t paddingSize = 10;

	std::vector<std::vector<float>> examplePictureMapPic(height, std::vector<float>(width, 0.0f));

	for (uint32_t r = 1; r <= oldHeight; ++r)
		for (uint32_t c = 1; c <= oldWidth; ++c)
			examplePictureMapPic[r][c] = max_value;

	PicturePGM pic{ height, width, size, max_value, examplePictureMapPic };

	// remove padding should not change picture because padding size is illegal
	pic.removePadding(paddingSize);

	EXPECT_EQ(pic.getHeight(), height);
	EXPECT_EQ(pic.getWidth(), width);
	EXPECT_EQ(pic.getSize(), size);
	EXPECT_EQ(pic.getMaxValue(), max_value);
	for (uint32_t r = 0; r < height; ++r)
		EXPECT_EQ(pic.get(r,0), 0);
	for (uint32_t r = 0; r < height; ++r)
		EXPECT_EQ(pic.get(r,3), 0);
	for (uint32_t c = 0; c < width; ++c)
		EXPECT_EQ(pic.get(0, c), 0);
	for (uint32_t c = 0; c < width; ++c)
		EXPECT_EQ(pic.get(3,c), 0);
	for (uint32_t r = 1; r < oldHeight; ++r)
		for (uint32_t c = 1; c < oldWidth; ++c)
			EXPECT_EQ(pic.get(r,c), max_value);
}