
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
#include "ArrayFunctions.h"

void deleteConfig(Config& imgConfig)
{
    /*
        deletes config map for edge extraction algorithm
    */
    for (auto it = imgConfig.begin(); it != imgConfig.end(); ++it)
        delete (it->second);
}

int main()
{

    const char FILE_PATH[] = "../../CppImageProcessingLibrary/images/SteilkurveGRAY_pgm.pgm";
    auto imProcessor = std::make_unique<ImageProcessor>(std::make_unique<Sobel>());
    auto picture = imProcessor->readImage(FILE_PATH);

    // edge extraction using Sobel
    Config sobelConfig;
    sobelConfig["sobel_threshold"] =  new TypedImageProperty<float>("sobel_threshold", 100.0);
    sobelConfig["gradient_only"] = new TypedImageProperty<bool>("gradient_only", false);
    auto sobel_pic = imProcessor->processImage(picture.get(), sobelConfig);
    imProcessor->writeImageAsPGM(sobel_pic.get(), "../../CppImageProcessingLibrary/out/SteilkurveGRAY_pgm_out.pgm");

    // edge extraction using sobel with rank calculation
    Config rangConfig;
    rangConfig["rang_threshold"] = new TypedImageProperty<float>("threshold_value", 7.0);
    rangConfig["surrounding"] = new TypedImageProperty<std::uint8_t>("surrounding", 3);
    imProcessor = std::make_unique<ImageProcessor>(std::make_unique<RangEdgeDetection>());
    auto output_pic = imProcessor->processImage(picture.get(), rangConfig);
    imProcessor->writeImageAsPGM(output_pic.get(), "../../CppImageProcessingLibrary/out/SteilkurveGRAY_pgm_rangbild_out.pgm");


    // delete algorithm configuration
    deleteConfig(sobelConfig);
    deleteConfig(rangConfig);

    std::cout << "Press any key to exit." << std::endl;
    std::cin.get();
    return 0;
}

