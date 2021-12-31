#pragma once
#include <memory>
#include "Picture.h"
#include "ImageConfig.h"
#include <vector>
#include <map>

using Config= std::map<std::string, std::unique_ptr<ImageProperty>>;

class ImageAlgorithmStrategy
{
    public:
        virtual ~ImageAlgorithmStrategy() = default;
        virtual PicturePGM processImage(PicturePGM& pic, Config& c) noexcept = 0;
};
