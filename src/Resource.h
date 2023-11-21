#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>

class ResourceHandle
{
public:
    ResourceHandle();
    ResourceHandle(const std::string& name);
    std::string getResourceName() const;
private:
    std::string m_name;
};

class IResource
{
public:
    virtual ~IResource();
    virtual ResourceHandle getResourceHandle() const;
    virtual void* getResourceData() const = 0;
    virtual void* getResourceExtra() const = 0;
private:
    ResourceHandle m_handle;

    friend class IResoruceLoader;
};

class IResourceLoader
{
public:
    virtual ~IResourceLoader();
    virtual std::vector<std::string> getLoaderSupportedExtentions() const = 0;
    virtual std::shared_ptr<IResource> loadResource(const ResourceHandle& handle) = 0;

    virtual bool supportSaving() const = 0;
    virtual void saveResource(const std::shared_ptr<IResource>& resource, const std::string& filename) const = 0;
};

class Resources
{
public:
    static Resources& getInstance();

    void addResourceLoader(const std::shared_ptr<IResourceLoader>& loader);
    std::shared_ptr<IResource> loadResource(const ResourceHandle& handle);

    void saveResource(const std::shared_ptr<IResource>& handle, const std::string& filename);
private:
    Resources();

    std::map<std::string, std::shared_ptr<IResourceLoader>> m_loaderMap;
};
