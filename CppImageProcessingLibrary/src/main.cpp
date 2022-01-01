
#include <iostream>
#include <memory>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "Picture.h"
#include "Sobel.h"
#include "ImageAlgorithmStrategy.h"
#include "ImageConfig.h"
#include "ImageProcessor.h"
#include "RangEdgeDetection.h"

std::tuple<std::string, std::uint32_t, std::uint32_t, std::uint16_t> readHeader(std::ifstream& inputFile)
{
    std::string header{ "" };
    std::uint32_t width{ 0 };
    std::uint32_t height{ 0 };
    std::uint16_t maxValue{ 0 };
    inputFile >> header >> width >> height >> maxValue;
    return { header, height, width, maxValue };
}

PicturePGM readPicture(const std::string& filename)
{
    std::ifstream inputFile{filename };
    if (!inputFile.is_open()) { return PicturePGM{}; }
    const auto [header, height, width, maxValue] = readHeader(inputFile);
    const std::uint32_t size = height * width;
    std::vector<std::vector<float>> pictureMap(height, std::vector<float>(width));
    unsigned char pixelValue;
    for (std::uint32_t row{ 0 }; row < height; ++row)
    {
        for (std::uint32_t col{ 0 }; col < width && inputFile >> pixelValue; ++col)
        {
            pictureMap[row][col] = static_cast<float>(pixelValue);
        }
    }

    return PicturePGM{height, width, size, static_cast<std::uint8_t>(maxValue), pictureMap};
}

int main()
{
    const static std::string FILE_PATH = "../../CppImageProcessingLibrary/images/SteilkurveGRAY_pgm.pgm";
    ImageProcessor imProcessor(std::make_unique<Sobel>());
    auto picture = imProcessor.readImage(FILE_PATH);

    // edge extraction using Sobel
    Config sobelConfig;
    sobelConfig["sobel_threshold"] =  std::make_unique<TypedImageProperty<float>>("sobel_threshold", 100.0f);
    sobelConfig["gradient_only"] = std::make_unique<TypedImageProperty<bool>>("gradient_only", false);
    auto sobel_pic = imProcessor.processImage(picture, sobelConfig);
    imProcessor.writeImageAsPGM(sobel_pic, "../../CppImageProcessingLibrary/out/SteilkurveGRAY_pgm_out.pgm");

    // edge extraction using sobel with rank calculation
    Config rangConfig;
    rangConfig["rang_threshold"] = std::make_unique<TypedImageProperty<float>>("threshold_value", 4.0f);
    rangConfig["surrounding"] = std::make_unique<TypedImageProperty<std::uint8_t>>("surrounding", 3);
    ImageProcessor imProcessor2(std::make_unique<RangEdgeDetection>());
    auto output_pic = imProcessor2.processImage(picture, rangConfig);
    imProcessor.writeImageAsPGM(output_pic, "../../CppImageProcessingLibrary/out/SteilkurveGRAY_pgm_rangbild_out.pgm");

    auto result = imProcessor.readImage("../../CppImageProcessingLibrary/images/TestSimple9x9.pgm");
    result.print();

    std::cout << "Press any key to exit." << std::endl;
    std::cin.get();
    return 0;
}

