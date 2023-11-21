#include "Rasterizer.h"

void defaultPerPixelFunction(const InputVertex& vert, Color& outcolor, void* userData)
{
    return;
}

void IRasterizer::setFramebuffer(Framebuffer& framebuffer)
{
    m_framebuffer = framebuffer;
}

const Framebuffer& IRasterizer::getFramebuffer() const
{
    return m_framebuffer;
}

void IRasterizer::setPerPixelFunction(const PerPixelFunction& func)
{
    m_function = func;
}

const PerPixelFunction& IRasterizer::getPerPixelFunction() const
{
    return m_function;
}

void DefaultRasterizer::drawLine(Vec3 start, Vec3 end, Color color)
{
    bool transpose = abs(end.y - start.y) > abs(end.x - start.x) ? true : false;
    if (transpose)
    {
        std::swap(start.x, start.y);
        std::swap(end.x, end.y);
    }
    if (start.x > end.x)
    {
        std::swap(start, end);
    }

    Vec2i res = m_framebuffer.colorAttachment->getImageResolution();
    Vec2i absStart = { start.x * res.x, start.y * res.y };
    Vec2i absEnd = { end.x * res.x, end.y * res.y };

    int dx = absEnd.x - absStart.x;
    int dy = absEnd.y - absStart.y;
    for (int x = absStart.x; x < absEnd.x; ++x)
    {
        float interpCoef = (float)(x - absStart.x) / dx;
        //int y = (int)(absStart.y + dy * interpCoef);
        int y = linearInterp(absStart.y, absEnd.y, interpCoef);
        unsigned char z = linearInterp(start.z, end.z, interpCoef) * 255;

        Vec2i pixelCoords = transpose ? Vec2i{ y, x } : Vec2i{ x, y };
        float depthValue = m_framebuffer.depthAttachment->getPixelColor(pixelCoords).r;
        if (depthValue > z)
        {
            InputVertex empty;
            m_function(empty, color, nullptr);
            m_framebuffer.colorAttachment->setPixelColor(pixelCoords, color);
            m_framebuffer.depthAttachment->setPixelColor(pixelCoords, { z, 0, 0, 0 });
        }
    }
}

void DefaultRasterizer::drawTriangle(Vec3 v0, Vec3 v1, Vec3 v2, Color color)
{
    if (v0.y > v1.y)
    { std::swap(v0, v1); }
    if (v0.y > v2.y)
    { std::swap(v0, v2); }
    if (v1.y > v2.y)
    { std::swap(v1, v2); }

    Vec2i res = m_framebuffer.colorAttachment->getImageResolution();
    Vec2i v0i = { v0.x * res.x, v0.y * res.y };
    Vec2i v1i = { v1.x * res.x, v1.y * res.y };
    Vec2i v2i = { v2.x * res.x, v2.y * res.y };

    int dy = v2i.y - v0i.y;
    Vec2i betaStart = v0i, betaEnd = v1i;
    float betaStartZ = v0.z, betaEndZ = v1.z;
    for (int y = v0i.y; y < v2i.y; ++y)
    {
        if (y >= v1.y)
        {
            betaStart = v1i;
            betaEnd = v2i;
            betaStartZ = v1.z;
            betaEndZ = v2.z;
        }
        int x0 = v0i.x + (v2i.x - v0i.x) * ((float)(y - v0i.y) / dy);
        int x1 = betaStart.x + (betaEnd.x - betaStart.x) * ((float)(y - betaStart.y) / (betaEnd.y - betaStart.y));
        float z0 = v0.z + (v2.z - v0.z) * ((float)(y - v0i.y) / dy);
        float z1 = betaStartZ + (betaEndZ - betaStartZ) * ((float)(y - betaStart.y) / (betaEnd.y - betaStart.y));
        if (x0 > x1)
        {
            std::swap(x0, x1);
        }
        drawLineAlongX({ x0, x1 }, y, { z0, z1 }, color);
    }
}

void DefaultRasterizer::drawLineAlongX(Vec2i xRange, int y, Vec2 zRange, Color color)
{
    if (xRange.x > xRange.y)
    { 
        std::swap(xRange.x, xRange.y); 
        std::swap(zRange.x, zRange.y);
    }
    
    float dx = (xRange.y - xRange.x);
    for (int x = xRange.x; x < xRange.y; ++x)
    {
        float interpCoef = (x - xRange.y) / dx;
        unsigned char z = linearInterp(zRange.x, zRange.y, interpCoef);
        if (m_framebuffer.depthAttachment->getPixelColor({ x, y }).r > z)
        {
            InputVertex empty;
            m_function(empty, color, nullptr);
            m_framebuffer.depthAttachment->setPixelColor({ x, y }, color);
        }
    }
}

void DefaultRasterizer::drawLine(InputVertex start, InputVertex end, Color color)
{}

void DefaultRasterizer::drawTriangle(InputVertex v0, InputVertex v1, InputVertex v2, Color color)
{}
