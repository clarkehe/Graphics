#pragma once

struct Texture
{
    const int static MAX_LEVEL = 10;
    int width, height;
    int mipMaps;
    std::string path;
    std::string type;
    Color *buffer[MAX_LEVEL];
    
    Texture()
    {
        width = 0;
        height = 0;
        mipMaps = 1;
        memset(buffer, 0x00, sizeof(buffer));
    }
               
    Texture(const int _width, const int _height)
    : width(_width)
    , height(_height)
    {
        mipMaps = 1;
        memset(buffer, 0x00, sizeof(buffer));
    }
    
    Color Sample(const int x, const int y, const int level)
    {
        assert(x >= 0 && x < width);
        assert(y >= 0 && y < height);
        assert(level >= 0 && level < mipMaps);
        
        int sampleLevel = level;
        if (buffer[sampleLevel] == NULL)
            sampleLevel = 0;
        
        Color *curBuff = buffer[sampleLevel];
        const int offset = y * width + x;
        return curBuff[offset];
    }
};
