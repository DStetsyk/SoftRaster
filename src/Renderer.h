#pragma once
#include <list>
#include "Geometry.h"
#include "Material.h"
#include "Framebuffer.h"
#include "utils.h"
#include "Rasterizer.h"

enum class eRenderingPrimitive
{
    Line,
    Triangle
};

class Renderer
{
public:
    struct State
    {
        eRenderingPrimitive primitive = eRenderingPrimitive::Line;
        bool depthTestEnabled = true;
        Framebuffer framebuffer;
    };

    Renderer();
    Renderer(const State& state);

    const State& getState() const;
    void setState(const State& state);

    void setPrimitive(const eRenderingPrimitive primitive);
    void setDephtTestEnabled(bool enabled);

    void addMatrixToStack(const glm::mat4& matrix);
    void popMatrix();

    void drawGeometry(const Geometry& geometry, const Color& color);
    void drawGeometryWithMaterial(const Geometry& geometry, const Material& material);
private:

    int getStrideForPrimitive(const eRenderingPrimitive primitive);

    State m_state;
    std::shared_ptr<IRasterizer> m_rasterizer;
    std::list<glm::mat4> m_matrixStack;

};