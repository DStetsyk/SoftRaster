#pragma once
#include <memory>
#include "ImageSampler.h"
#include "utils.h"

class Material
{
public:
    Color ambientColor = Color{ 1, 1, 1, 1 };
    Color diffuseColor = Color{ 1, 1, 1, 1 };
    Color specularColor = Color{ 1, 1, 1, 1 };

    std::shared_ptr<Sampler2D> diffuseTex = nullptr;
    std::shared_ptr<Sampler2D> specularMap = nullptr;
    std::shared_ptr<Sampler2D> normalMap = nullptr;
    float shininess = .0f;
};