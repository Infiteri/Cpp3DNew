#pragma once

#include "Base.h"
#include <string>

namespace Core
{
    class CE_API Image
    {
    private:
        std::string path;
        int width;
        int height;
        int channels;
        CeU8 *data;
        bool valid;

    public:
        /// @brief Will load the image data into this class.
        /// @param imagePath The image path. (in the future, if the path ends in .ce_image (or other custom image format), it will be loaded respectively)
        Image(const std::string &imagePath);
        ~Image();

        inline std::string GetPath() { return path; };
        inline int GetWidth() { return width; };
        inline int GetHeight() { return height; };
        inline int GetChannels() { return channels; };
        inline CeU8 *GetData() { return data; };
        inline bool IsValid() { return valid; };

        void FreeData();
    };
}