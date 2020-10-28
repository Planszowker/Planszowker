#include "ResourceHolder.hpp"

namespace Resources {
    void ResourceHolder::Load(const enum Textures& texturesEnum, const std::string &filename) {
        auto inserted = _mResourcesMap.insert(std::make_pair(texturesEnum, std::make_unique<sf::Texture>()));
        if(!_mResourcesMap[texturesEnum]->loadFromFile(resourcePath() + filename)){
            throw std::runtime_error("ResourceHolder::Load - failed to load a resource from "
                                     + filename);
        }
        
        assert(inserted.second);
    }
    
    sf::Texture &ResourceHolder::Get(enum Textures texturesEnum) {
        auto found = _mResourcesMap.find(texturesEnum);
        assert(found != _mResourcesMap.end());
        
        return *found->second;
    }
    
    const sf::Texture &ResourceHolder::Get(enum Textures texturesEnum) const{
        auto found = _mResourcesMap.find(texturesEnum);
        assert(found != _mResourcesMap.end());
        
        return *found->second;
    }
}
