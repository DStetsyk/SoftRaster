#pragma once
#include <memory>
#include "Image.h"

struct Framebuffer
{
    std::shared_ptr<Image> colorAttachment;
    std::shared_ptr<Image> depthAttachment;
};
