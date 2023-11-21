#pragma once
#include <memory>
#include "Image.h"

class Sampler2D
{
public:
    enum class eInterpMode
    {
        Linear,
        Near
    };
    Sampler2D(std::shared_ptr<Image>& image);
    Sampler2D(std::shared_ptr<Image>& image, eInterpMode mode);

    void setInterpolationMode(eInterpMode mode);
    void setImage(std::shared_ptr<Image>& image);
    const std::shared_ptr<Image>& getImage() const;

    Color getColor(const Vec2 coords) const;
    void setColor(const Vec2 coords, const Color color);
private:
    std::shared_ptr<Image> m_image;
    eInterpMode m_interpMode;
};