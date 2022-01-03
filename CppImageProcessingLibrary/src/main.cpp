
#include <iostream>
#include <memory>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "Picture.h"
#include "Sobel.h"
#include "ImageAlgorithmStrategy.h"
#include "ImageProcessor.h"
#include "RangEdgeDetection.h"

int main()
{
    const static std::string FILE_PATH = "../../CppImageProcessingLibrary/images/SteilkurveGRAY_pgm.pgm";
    ImageProcessor imProcessor(std::make_unique<Sobel>(100.0f, false));
    auto picture = imProcessor.readImage(FILE_PATH);
    picture.makePadding();

    // edge extraction using Sobel
    auto sobelPic = imProcessor.processImage(picture);
    imProcessor.writeImageAsPGM(sobelPic, "../../CppImageProcessingLibrary/out/SteilkurveGRAY_pgm_out.pgm");

    // edge extraction using sobel with rank calculation
    ImageProcessor imProcessor2(std::make_unique<RangEdgeDetection>(3, 5.0f));
    auto rangPic = imProcessor2.processImage(picture);
    imProcessor.writeImageAsPGM(rangPic, "../../CppImageProcessingLibrary/out/SteilkurveGRAY_pgm_rangbild_out.pgm");
    
    std::cout << "Press any key to exit." << std::endl;
    std::cin.get();
    return 0;
}

