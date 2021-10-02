#pragma once
#include <memory>
#include "Picture.h"
#include "ImageConfig.h"
#include <vector>
#include <map>

using Config= std::map<std::string, ImageProperty*>;

class ImageAlgorithmStrategy
{
    public:
        virtual ~ImageAlgorithmStrategy() = default;
        virtual std::unique_ptr<PicturePGM> processImage(PicturePGM* pic, Config& c) = 0;
};
