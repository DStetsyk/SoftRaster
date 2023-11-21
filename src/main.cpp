#include "ImageResource.h"
#include "MeshResource.h"

int main()
{
    std::shared_ptr<IResourceLoader> imgLoader = std::make_shared<ImageLoader>();
    Resources::getInstance().addResourceLoader(imgLoader);
    std::shared_ptr<IResourceLoader> modelLoader = std::make_shared<MeshLoader>();
    Resources::getInstance().addResourceLoader(modelLoader);

    ResourceHandle img("input.png");
    Resources::getInstance().loadResource(img);
    return 0;
}