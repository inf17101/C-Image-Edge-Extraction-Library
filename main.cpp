
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

    const char FILE_PATH[] = "images/SteilkurveGRAY_pgm.pgm";
    auto imProcessor = std::make_unique<ImageProcessor>(std::make_unique<Sobel>());
    //const char FILE_PATH[] = "images/TestSimple9x9.pgm";
    //auto imProcessor = std::make_unique<ImageProcessor>(std::make_unique<RangEdgeDetection>());
    auto picture = imProcessor->readImage(FILE_PATH);

    Config c;
    //c["rang_threshold"] = new TypedImageProperty<float>("threshold_value", 7.0);
    //c["surrounding"] = new TypedImageProperty<std::uint8_t>("surrounding", 3);
    c["sobel_threshold"] =  new TypedImageProperty<float>("sobel_threshold", 100.0);
    c["gradient_only"] = new TypedImageProperty<bool>("gradient_only", false);
    auto sobel_pic = imProcessor->processImage(picture.get(), c);

    imProcessor->writeImageAsPGM(sobel_pic.get(), "out/SteilkurveGRAY_pgm_out.pgm");

    
    //auto output_pic = imProcessor->processImage(picture.get(), c);
    //imProcessor->writeImageAsPGM(output_pic.get(), "out/SteilkurveGRAY_pgm_rangbild_out2.pgm");
    //output_pic->printPic();

    for(auto it = c.begin(); it != c.end(); ++it)
        delete (it->second);

    return 0;
}

