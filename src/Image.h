#pragma once
#include "utils.h"

class Image
{
public:
    enum class eImageFormat
    {
        Y = 1,      // Monochrome, 8 bits
        YA,         // Two component image, 8 btis per component
        RGB,
        RGBA
    };

    Image();
    Image(Vec2i res, eImageFormat format);
    Image(Vec2i res, eImageFormat format, void* data);
    ~Image();

    // Allocate data for image 
    // works only if m_data == nullptr
    bool createIamge(); 

    void clear();

    Color getPixelColor(Vec2i coords);
    void setPixelColor(Vec2i coords, Color color);

    void* getImageData() const;
    eImageFormat getImageFormat() const;
    Vec2i getImageResolution() const;

protected:
    // Restrict changing image properties after creation 
    // This setters available only for loader
    void setImageFormat(eImageFormat format);
    void setImageData(void* data);
    void setImageResolution(Vec2i res);

private:
    static int getFormatBytesPerPixel(eImageFormat format);
    static eImageFormat getFormatForNumOfComponents(int compNum);

    Vec2i m_resolution;
    eImageFormat m_format;
    unsigned char* m_data;
    bool m_ownData;

    friend class ImageLoader;
};

