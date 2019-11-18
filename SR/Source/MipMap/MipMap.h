#pragma once

int GetMaxMipMaps(const int width, const int height);

Color *GenerateMipMap(Color *source, const int width, const int height);


