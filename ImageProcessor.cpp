#include "ImageProcessor.h"
#include <cstring>
#include <fstream>

bool ImageProcessor::checkPaddingOption(uint8_t padding_size)
{
    if(padding_size < 2 || padding_size > 7)
    {
        std::cerr << "illegal padding size. Should be: 7 <= padding_size >= 2" << std::endl;
        return false;
    }
    return true;
}

std::unique_ptr<PicturePGM> ImageProcessor::readImage(const char* filename, uint8_t padding_size)
{
    if(!checkPaddingOption(padding_size)) return std::make_unique<PicturePGM>();

    FILE *fhandle = fopen(filename, "rb");
    if (!fhandle)
    {
        std::cerr << "File could not be opened." << std::endl;
        return std::make_unique<PicturePGM>();
    }
    std::cout << "Opened file " << filename << " correctly!" << std::endl;

    char line[75];
    if (fgets(line, sizeof(line), fhandle) == NULL) return std::make_unique<PicturePGM>();

    if(strncmp(line, "P5",2) != 0) return std::make_unique<PicturePGM>();

    if(fgets(line, sizeof(line), fhandle) == NULL) return std::make_unique<PicturePGM>();

    std::unique_ptr<PicturePGM> inputPicture = std::make_unique<PicturePGM>();
    inputPicture->width = strtoul(strtok(line, " "), NULL, 10);
    inputPicture->height = strtoul(strtok(NULL, " "), NULL, 10);
    inputPicture->size = inputPicture->height * inputPicture -> width;
    
    if(fgets(line, sizeof(line), fhandle) == NULL) return std::make_unique<PicturePGM>();
    inputPicture->max_value = (uint8_t) strtoul(line, NULL, 10);

    inputPicture->height += padding_size; //because of zero padding
    inputPicture->width += padding_size;

    auto success = inputPicture->reinitWithValue(0.0, inputPicture->width, inputPicture->height);
    if(!success) return std::make_unique<PicturePGM>();

    for (uint32_t i=1; i<inputPicture->height-1; ++i)
        for(uint32_t j=1; j<inputPicture->width-1; ++j)
            inputPicture->map[i][j] = static_cast<float> (fgetc(fhandle));

    fclose(fhandle);

    return inputPicture;

}

std::unique_ptr<PicturePGM> ImageProcessor::processImage(PicturePGM* pic, Config& c)
{
    return algorithm_->processImage(pic,c);
}

int8_t ImageProcessor::writeImageAsPGM(PicturePGM* pic, const char* FILE_PATH)
{
    std::ofstream of(FILE_PATH, std::ios::out
                                | std::ios_base::binary
                                | std::ios_base::trunc);
    if(!of)
        return -1;
    
    of << "P5\n";
    of << pic->width << " " << pic->height << "\n";

    /*
        must be converted to unsigned int otherwise
        image software cannot open the result image properly
    */
    of << static_cast<unsigned int>(pic->max_value) << "\n";

    if(of.bad())
        return -1;

    for(uint32_t i=0; i<pic->height; ++i)
        for(uint32_t j=0; j<pic->width; ++j)
            of.put(pic->map[i][j]);
    
    if(of.bad())
        return -1;
    of.close();

    return 0;

}