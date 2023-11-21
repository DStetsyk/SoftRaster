#include "ImageSampler.h"

Sampler2D::Sampler2D(std::shared_ptr<Image>& image)
    : m_image(image)
    , m_interpMode(eInterpMode::Near)
{}

Sampler2D::Sampler2D(std::shared_ptr<Image>& image, eInterpMode mode)
    : m_image(image)
    , m_interpMode(mode)
{}

void Sampler2D::setInterpolationMode(eInterpMode mode)
{
    m_interpMode = mode;
}

void Sampler2D::setImage(std::shared_ptr<Image>& image)
{
    m_image = image;
}

const std::shared_ptr<Image>& Sampler2D::getImage() const
{
    return m_image;
}

Color Sampler2D::getColor(const Vec2 coords) const
{
    Vec2i res = m_image->getImageResolution();
    Vec2i pixelCoords;
    switch (m_interpMode)
    {
    case eInterpMode::Near:
        pixelCoords = { (int)((float)res.x * coords.x), (int)((float)res.y * coords.y) };

        return m_image->getPixelColor(pixelCoords);

    case eInterpMode::Linear:
        float xStep = 1.0f / res.x;
        float yStep = 1.0f / res.y;

        int xLeft = (int)(coords.x / xStep);
        int yTop = (int)(coords.y / yStep);

        /*
        * Get the colors of 4 nearest pixels 
        *   | 0 | 1 | 
        *   | 2 | 3 |  
        */
        Color pixels[4];
        for (int i = 0; i < 4; ++i)
        {
            int x = (i % 2 == 0) ? xLeft : xLeft + 1;
            int y = (i / 2 == 0) ? yTop : yTop + 1;
            pixelCoords = { x, y };
            pixels[i] = m_image->getPixelColor(pixelCoords);
        }

        float xT = (coords.x - xStep * xLeft) / xStep;
        float yT = (coords.y - yStep * yTop) / yStep;

        Color xTopInterp = linearInterp(pixels[0], pixels[1], xT);
        Color xBotInterp = linearInterp(pixels[2], pixels[3], xT);
        Color ret = linearInterp(xTopInterp, xBotInterp, yT);
        return ret;
    }
}

void Sampler2D::setColor(const Vec2 coords, const Color color)
{
    Vec2i res = m_image->getImageResolution();
    Vec2i pixelCoords{ (int)((float)res.x * coords.x), (int)((float)res.y * coords.y) };

    m_image->setPixelColor(pixelCoords, color);
}
