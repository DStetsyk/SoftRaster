#include "Resource.h"
#include <filesystem>

ResourceHandle::ResourceHandle()
    : m_name("")
{}

ResourceHandle::ResourceHandle(const std::string& name)
    : m_name(name)
{}

std::string ResourceHandle::getResourceName() const
{
    return m_name;
}

IResource::~IResource()
{}

ResourceHandle IResource::getResourceHandle() const
{
    return m_handle;
}

IResourceLoader::~IResourceLoader()
{}

Resources& Resources::getInstance()
{
    static Resources instance;
    return instance;
}

void Resources::addResourceLoader(const std::shared_ptr<IResourceLoader>& loader)
{
    const auto supportedFormats = loader->getLoaderSupportedExtentions();

    for (std::string ext : supportedFormats)
    {
        m_loaderMap.emplace(ext, loader);
    }
}

std::shared_ptr<IResource> Resources::loadResource(const ResourceHandle& handle)
{
    std::filesystem::path resourceFileName(handle.getResourceName());

    const auto& it = m_loaderMap.find(resourceFileName.extension().string());
    if (it != m_loaderMap.end())
    {
        const std::shared_ptr<IResourceLoader>& loader = it->second;
        return loader->loadResource(handle);
    }

    return nullptr;
}

void Resources::saveResource(const std::shared_ptr<IResource>& handle, const std::string& filename)
{
    std::filesystem::path resourceFileName = filename;

    const auto& it = m_loaderMap.find(resourceFileName.extension().string());
    if (it != m_loaderMap.end() && it->second->supportSaving())
    {
        it->second->saveResource(handle, filename);
    }
}

Resources::Resources()
{}
