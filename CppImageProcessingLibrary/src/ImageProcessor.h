#pragma once

#include "ImageAlgorithmStrategy.h"
#include <memory>

class ImageProcessor
{
    std::unique_ptr<ImageAlgorithmStrategy> algorithm_;

    bool checkPaddingOption(uint8_t padding_size);
    public:
        ImageProcessor(std::unique_ptr<ImageAlgorithmStrategy> a) : algorithm_(std::move(a)) {}
        std::unique_ptr<PicturePGM> readImage(const char* filename, uint8_t padding_size=2);
        std::unique_ptr<PicturePGM> processImage(PicturePGM* pic, Config& c);
        int8_t writeImageAsPGM(PicturePGM* pic, const char* FILE_PATH);
        ImageProcessor(const ImageProcessor& ) = delete;
        ImageProcessor& operator=(ImageProcessor& ) = delete;
};
