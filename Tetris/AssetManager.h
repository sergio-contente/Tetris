#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>

#include "SFML/Graphics.hpp"

class AssetManager
{
public:
    void LoadTexture(const std::string& name, const std::string& filename);
    sf::Texture& GetTexture(const std::string& name);

    void LoadFont(const std::string& name, const std::string& filename);
    sf::Font& GetFont(const std::string& name);

private:
    std::map<std::string, sf::Texture> _textures;
    std::map<std::string, sf::Font> _fonts;
};

#endif // ASSETMANAGER_H
