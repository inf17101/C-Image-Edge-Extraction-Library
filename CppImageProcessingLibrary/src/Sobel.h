#pragma once
#include "ImageAlgorithmStrategy.h"
#include "Picture.h"
#include <memory>

class Sobel : public ImageAlgorithmStrategy
{
    bool isConfigValid(Config& c, float& threshold_value, bool& gradient_only);
    public:
        Sobel() = default;
        ~Sobel() = default;
        virtual std::unique_ptr<PicturePGM> processImage(PicturePGM* pic, Config& c) override;

};