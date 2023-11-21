#pragma once
#include <vector>
#include <functional>
#include "utils.h"
#include "Image.h"
#include "Framebuffer.h"

struct VertexProperty
{
    enum class eInterpMode
    {
        Near,
        Linear
    };
    enum class eSemantic
    {
        None, 
        Position,
        Normal,
        TexCoord
    };
    enum class eType
    {
        Vec2Float,
        Vec3Float,
        Vec4Float,
        Vec2Int,
        Vec3Int,
        Vec4Int
    };

    union
    {
        Vec2 vec2FValue;
        Vec3 vec3FValue;
        Vec4 vec4FValue;
        Vec2 vec2IValue;
        Vec3 vec3IValue;
        Vec4 vec4IValue;
        Color colorValue;
    };
    eType type = eType::Vec4Float;
    eSemantic semantic = eSemantic::None;
    eInterpMode interpolate = eInterpMode::Near;
};

struct InputVertex 
{
    std::vector<VertexProperty> properties;
};

using PerPixelFunction = std::function<void(const InputVertex& vert, Color& outcolor, void* userData)>;

class IRasterizer
{
public:
    virtual ~IRasterizer() {};

    void setFramebuffer(Framebuffer& framebuffer);
    const Framebuffer& getFramebuffer() const;

    void setPerPixelFunction(const PerPixelFunction& func);
    const PerPixelFunction& getPerPixelFunction() const;
    
    virtual void drawLine(Vec3 start, Vec3 end, Color color) = 0;
    virtual void drawTriangle(Vec3 v0, Vec3 v1, Vec3 v2, Color color) = 0;

    virtual void drawLine(InputVertex start, InputVertex end, Color color) = 0;
    virtual void drawTriangle(InputVertex v0, InputVertex v1, InputVertex v2, Color color) = 0;
protected:
    Framebuffer m_framebuffer;
    PerPixelFunction m_function;
};

class DefaultRasterizer : public IRasterizer
{
public:
    void drawLine(Vec3 start, Vec3 end, Color color) override;
    void drawTriangle(Vec3 v0, Vec3 v1, Vec3 v2, Color color) override;

    void drawLine(InputVertex start, InputVertex end, Color color) override;
    void drawTriangle(InputVertex v0, InputVertex v1, InputVertex v2, Color color) override;
protected:
    void drawLineAlongX(Vec2i xRange, int y, Vec2 zRange, Color color);
};
