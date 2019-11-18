#include "stdafx.h"
#include "../MipMap/MipMap.h"

static bool IsPowerOfTwo(const int size)
{
    assert(size > 0);
    
    unsigned int v0 = (unsigned int)size;
    unsigned int v1 = v0 - 1u;
    unsigned int c  = v0 & v1;
    return (c == 0u);
}

static int NearestPowerOfTwo(const int size)
{
    assert(size > 0);
    
    unsigned int v = (unsigned int)size;
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;    // next power of 2
    
    unsigned int x = v >> 1; // previous power of 2
    return (v - size) > (size - x) ? x : v;
}

int GetMaxMipMaps(const int width, const int height)
{
    if (!IsPowerOfTwo(width) || !IsPowerOfTwo(height))
        return 1;
    
    const int widthLevel  = (int)log2(width);
    const int heightLevel = (int)log2(height);
    
    return widthLevel < heightLevel ? (widthLevel + 1) : (heightLevel + 1);
}

//BoxFilter
Color *GenerateMipMap(Color *source, const int width, const int height)
{
    assert(source != NULL);
    assert(IsPowerOfTwo(width));
    assert(IsPowerOfTwo(height));
    
    const int newWidth  = width >> 1;
    const int newHeight = height >> 1;
    
    const int bufferSize = sizeof(Color) * newWidth * newHeight;
    Color *buffer = (Color *)malloc(bufferSize);
    
    for (int j = 0; j < newHeight; j++)
    {
        for (int i = 0; i < newWidth; i++)
        {
            const int x = i * 2;
            const int y = j * 2;
            
            int x1 = x + 1;
            int y1 = y + 1;
            
            if (x1 >= width)
                x1 = width -1;
            
            if (y1 >= height)
                y1 = height -1;
            
            const Color A = source[y * width + x];
            const Color B = source[y * width + x1];
            const Color C = source[y1 * width + x];
            const Color D = source[y1 * width + x1];
            
            const Color color = (A + B + C + D) * 0.25;
            buffer[j * newWidth + i] = color;
        }
    }
    
    return buffer;
}

