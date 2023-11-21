#pragma once
#include "Resource.h"
#include "Image.h"

class ImageResource : public IResource
{
public:
    struct Extra
    {
       std::shared_ptr<Image> img;
    };

    ~ImageResource();

    void* getResourceData() const override;
    void* getResourceExtra() const override;

private:
    Extra m_extraData;
    bool m_loaded = false;
    friend class ImageLoader;
};

class ImageLoader : public IResourceLoader
{
public:
    std::vector<std::string> getLoaderSupportedExtentions() const override;
    std::shared_ptr<IResource> loadResource(const ResourceHandle& handle) override;

    bool supportSaving() const override;
    void saveResource(const std::shared_ptr<IResource>& resource, const std::string& filename) const override;
};

