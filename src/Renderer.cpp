#include "Renderer.h"

const Vec2i defaultResolution = { 800, 600 };

Renderer::Renderer()
{
    m_rasterizer = std::make_shared<DefaultRasterizer>();
    m_state.framebuffer.colorAttachment = std::make_shared<Image>(defaultResolution, Image::eImageFormat::RGBA);
    m_state.framebuffer.colorAttachment->createIamge();
    m_state.framebuffer.depthAttachment = std::make_shared<Image>(defaultResolution, Image::eImageFormat::Y);
    m_state.framebuffer.depthAttachment->createIamge();
    m_rasterizer->setFramebuffer(m_state.framebuffer);
}

Renderer::Renderer(const State& state)
    : m_state(state)
{
    if (m_state.framebuffer.colorAttachment == nullptr)
    {
        m_state.framebuffer.colorAttachment = std::make_shared<Image>(defaultResolution, Image::eImageFormat::RGBA);
        m_state.framebuffer.colorAttachment->createIamge();
    }

    if (m_state.framebuffer.depthAttachment == nullptr)
    {
        m_state.framebuffer.depthAttachment = std::make_shared<Image>(defaultResolution, Image::eImageFormat::Y);
        m_state.framebuffer.depthAttachment->createIamge();
    }
}

const Renderer::State& Renderer::getState() const
{
    return m_state;
}

void Renderer::setState(const State& state)
{
    m_state = state;
    if (m_state.framebuffer.colorAttachment != m_rasterizer->getFramebuffer().colorAttachment ||
        m_state.framebuffer.depthAttachment != m_rasterizer->getFramebuffer().depthAttachment)
    {
        m_rasterizer->setFramebuffer(m_state.framebuffer);
    }
}

void Renderer::setPrimitive(const eRenderingPrimitive primitive)
{
    m_state.primitive = primitive;
}

void Renderer::setDephtTestEnabled(bool enabled)
{
    m_state.depthTestEnabled = enabled;
}

void Renderer::addMatrixToStack(const glm::mat4& matrix)
{
    m_matrixStack.push_back(matrix);
}

void Renderer::popMatrix()
{
    m_matrixStack.pop_back();
}

void Renderer::drawGeometry(const Geometry& geometry, const Color& color)
{
    int stride = getStrideForPrimitive(m_state.primitive);
    for (int i = 0; i < geometry.indices.size(); i += stride)
    {

    }
}

void Renderer::drawGeometryWithMaterial(const Geometry& geometry, const Material& material)
{

}

int Renderer::getStrideForPrimitive(const eRenderingPrimitive primitive)
{
    switch (primitive)
    {
    case eRenderingPrimitive::Line:
        return 2;
    case eRenderingPrimitive::Triangle:
        return 3;
    }
}
