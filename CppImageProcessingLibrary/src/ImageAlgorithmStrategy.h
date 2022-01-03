#pragma once
#include <memory>
#include "Picture.h"

class ImageAlgorithmStrategy
{
    public:
        virtual ~ImageAlgorithmStrategy() = default;
        virtual PicturePGM processImage(PicturePGM& pic) noexcept = 0;
};
