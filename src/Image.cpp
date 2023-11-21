#include "Image.h"
#include <string>

Image::Image()
    : m_resolution{ 0, 0 }
    , m_format(eImageFormat::RGB)
    , m_data(nullptr)
    , m_ownData(false)
{}

Image::Image(Vec2i res, eImageFormat format)
    : m_resolution(res)
    , m_format(format)
    , m_data(nullptr)
    , m_ownData(false)
{}

Image::Image(Vec2i res, eImageFormat format, void* data)
    : m_resolution(res)
    , m_format(format)
    , m_data((unsigned char*)data)
    , m_ownData(false)
{}

Image::~Image()
{
    if (m_data && m_ownData)
    {
        delete[] m_data;
    }
}

bool Image::createIamge()
{
    if (m_data != nullptr)
    {
        return false;
    }

    size_t byteSize = m_resolution.x * m_resolution.y * getFormatBytesPerPixel(m_format);
    m_data = new unsigned char[byteSize];
    memset(m_data, 0, byteSize);
    m_ownData = true;

    return true;
}

void Image::clear()
{
    size_t byteSize = m_resolution.x * m_resolution.y * getFormatBytesPerPixel(m_format);
    memset(m_data, 0, byteSize);
}

Color Image::getPixelColor(Vec2i coords)
{
    if (coords.x >= m_resolution.x || coords.y >= m_resolution.y)
    { return { 0, 0, 0, 1 }; }

    int stride = (coords.y * m_resolution.x + coords.x) * getFormatBytesPerPixel(m_format);
    Color color{ 0, 0, 0, 1 };
    std::memcpy(&color, m_data + stride, getFormatBytesPerPixel(m_format));
    return color;
}

void Image::setPixelColor(Vec2i coords, Color color)
{
    int stride = (coords.y * m_resolution.x + coords.x) * getFormatBytesPerPixel(m_format);
    std::memcpy(m_data + stride, &color, getFormatBytesPerPixel(m_format));
}

void* Image::getImageData() const
{
    return (void*)m_data;
}

Image::eImageFormat Image::getImageFormat() const
{
    return m_format;
}

Vec2i Image::getImageResolution() const
{
    return m_resolution;
}

void Image::setImageFormat(eImageFormat format)
{
    m_format = format;
}

void Image::setImageData(void* data)
{
    m_data = (unsigned char*)data;
}

void Image::setImageResolution(Vec2i res)
{
    m_resolution = res;
}

int Image::getFormatBytesPerPixel(eImageFormat format)
{
    switch (format)
    {
    case eImageFormat::Y:
        return 1;
    case eImageFormat::YA:
        return 2;
    case eImageFormat::RGB:
        return 3;
    case eImageFormat::RGBA:
        return 4;
    }

    return 0;
}

Image::eImageFormat Image::getFormatForNumOfComponents(int compNum)
{
    return (eImageFormat)compNum;
}
