
#include <iostream>
#include <memory>
#include <cstring>
#include <algorithm>
#include "Picture.h"
#include "Sobel.h"
#include "ImageAlgorithmStrategy.h"
#include "ImageConfig.h"
#include "ImageProcessor.h"
#include "RangEdgeDetection.h"

int main()
{

    const char FILE_PATH[] = "../../CppImageProcessingLibrary/images/SteilkurveGRAY_pgm.pgm";
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
    rangConfig["rang_threshold"] = std::make_unique<TypedImageProperty<float>>("threshold_value", 7.0f);
    rangConfig["surrounding"] = std::make_unique<TypedImageProperty<std::uint8_t>>("surrounding", 3);
    ImageProcessor imProcessor2(std::make_unique<RangEdgeDetection>());
    auto output_pic = imProcessor2.processImage(picture, rangConfig);
    imProcessor.writeImageAsPGM(output_pic, "../../CppImageProcessingLibrary/out/SteilkurveGRAY_pgm_rangbild_out.pgm");

    std::cout << "Press any key to exit." << std::endl;
    std::cin.get();
    return 0;
}

