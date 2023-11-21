#pragma once
#include "Resource.h"
#include "ImageResource.h"
#include "Geometry.h"
#include "Material.h"
#include <assimp/scene.h>

class ModelResource : public IResource
{
public:
    struct Mesh
    {
        Geometry geometry;
        Material material;
    };
    using ModelData = std::vector<Mesh>;
    using ModelExtra = std::vector<std::shared_ptr<ImageResource>>;

    void* getResourceData() const override;
    void* getResourceExtra() const override;
    
private:
    std::vector<Mesh> m_meshes;
    std::vector<std::shared_ptr<ImageResource>> m_textures;

    friend class MeshLoader;
};

class MeshLoader : public IResourceLoader
{
public:
    std::vector<std::string> getLoaderSupportedExtentions() const override;
    std::shared_ptr<IResource> loadResource(const ResourceHandle& handle) override;

    bool supportSaving() const override;
    void saveResource(const std::shared_ptr<IResource>& resource, const std::string& filename) const override;
private:
    
    void loadModel();
    void processNode(aiNode* node, const aiScene* scene);
    ModelResource::Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::string m_filename;
    std::shared_ptr<IResource> m_resource;
};
