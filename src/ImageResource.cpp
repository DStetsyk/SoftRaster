#include "ImageResource.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <filesystem>
#include <iostream>

static const std::vector<std::string> sImageLoaderSupportedExtentions =
{
    ".png",
    ".jpg",
    ".jpeg"
};

ImageResource::~ImageResource()
{
    if (m_loaded)
    {
        stbi_image_free(m_extraData.img->getImageData());
    }
}

void* ImageResource::getResourceData() const
{
    return m_extraData.img->getImageData();
}

void* ImageResource::getResourceExtra() const 
{
    return (void*) &m_extraData;
}

std::vector<std::string> ImageLoader::getLoaderSupportedExtentions() const
{
    return sImageLoaderSupportedExtentions;
}

std::shared_ptr<IResource> ImageLoader::loadResource(const ResourceHandle& handle) 
{
    int x = 0, y = 0, n = 0;
    std::shared_ptr<ImageResource> img = std::make_shared<ImageResource>();
    unsigned char* data = stbi_load(handle.getResourceName().c_str(), &x, &y, &n, 0);
    if (data == NULL)
    { return nullptr; }

    img->m_extraData.img = std::shared_ptr<Image>(new Image({ x, y }, 
        Image::getFormatForNumOfComponents(n),
        data));
    img->m_loaded = true;
    return img;
}

bool ImageLoader::supportSaving() const
{
    return true;
}

void ImageLoader::saveResource(const std::shared_ptr<IResource>& resource, const std::string& filename) const
{
    std::filesystem::path resourceFileName = filename;
    std::string ext = resourceFileName.extension().string();
    std::shared_ptr<Image> img = ((ImageResource::Extra*)resource->getResourceExtra())->img;
    Vec2i res = img->getImageResolution();

    if (ext == ".png")
    {
        stbi_write_png(filename.c_str(),
            res.x,
            res.y,
            (int)img->getImageFormat(),
            img->getImageData(),
            res.x * img->getFormatBytesPerPixel(img->getImageFormat()));
    }
    else if (ext == ".jpg" || ext == ".jpeg")
    {
        stbi_write_jpg(filename.c_str(),
            res.x,
            res.y,
            (int)img->getImageFormat(),
            img->getImageData(),
            100);
    }
    else
    {
        std::cerr << "Unsupported file format" << std::endl;
    }

    return;
}
