#pragma once

class TextureManager
{
public:
    static TextureManager* getInstance();
    
public:
    bool LoadTexture (Texture &texture, std::string& file);
    std::vector<Color> &getTexture(std::string& filepath);
    
private:
    std::map<std::string, std::vector<Color>> textureMaps;
};
