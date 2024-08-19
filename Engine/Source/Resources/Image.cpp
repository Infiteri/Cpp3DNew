#include "Image.h"
#include "Core/Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Core
{
    Image::Image(const std::string &imagePath)
    {
        path = imagePath;
        width = height = channels = 0;
        valid = false; // Note; no point in setting it to false

        data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data)
        {
            valid = false;
            CE_WARN("Image invalid: %s.", imagePath.c_str());
        }

        valid = true;
    }

    Image::~Image()
    {
    }

    void Image::FreeData()
    {
        if (!data)
            return;

        stbi_image_free(data);
    }
}