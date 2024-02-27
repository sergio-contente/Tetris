#include "AssetManager.h"

void AssetManager::LoadTexture(const std::string& name, const std::string& filename)
{
    sf::Texture texture;
    if (texture.loadFromFile(filename))
    {
        this->_textures[name] = texture;
    }
}

sf::Texture& AssetManager::GetTexture(const std::string& name)
{
    return this->_textures.at(name);
}

void AssetManager::LoadFont(const std::string& name, const std::string& filename)
{
    sf::Font font;
    if (font.loadFromFile(filename))
    {
        this->_fonts[name] = font;
    }
}

sf::Font& AssetManager::GetFont(const std::string& name)
{
    return this->_fonts.at(name);
}
