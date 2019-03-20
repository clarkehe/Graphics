#include "stdafx.h"
#include "TextureManager.h"
#include "stb_image.h"
#include "stb_image_write.h"


static TextureManager * instance = NULL;

TextureManager* TextureManager::getInstance()
{
    if (!instance)
    {
        instance = new TextureManager();
    }
    return instance;
}

bool TextureManager::LoadTexture (Texture &texture, std::string& file)
{
    if(textureMaps.find(file) != textureMaps.end()) return true;

    //     N=#comp     components
    //       1           grey
    //       2           grey, alpha
    //       3           red, green, blue
    //       4           red, green, blue, alpha

    int comp;
    unsigned char* tmp = stbi_load(file.c_str(), &texture.width, &texture.height, &comp, STBI_rgb);
    if (tmp == NULL)
    {
        std::cout << "load file failed" << std::endl;
        return false;
    }
    
    std::vector<Color> data;
    data.resize (texture.width * texture.height);

    int count = 0;
    auto it = data.begin();
    for (; it != data.end(); it++)
    {
        Color &color = *it;
        color = Color(tmp[count + 0] / 255.0f, tmp[count + 1] / 255.0f, tmp[count + 2] / 255.0f );
        count += STBI_rgb;
    }

    textureMaps[file] = data;
    stbi_image_free(tmp);
    return true;
}

std::vector<Color> &TextureManager::getTexture(std::string & filepath)
{
    //if (textureMaps.find(filepath) != textureMaps.end())
    {
        return textureMaps.at(filepath);
    }
}

