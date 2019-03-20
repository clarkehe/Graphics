#pragma once

namespace SoftRender
{
	struct Texture
	{
		int width, height;
		std::string path;
		std::string type;

		Texture()
        {
			width = 0;
			height = 0;
		}

		Texture(int _width, int _height):
			width(_width),
			height(_height)
        {}
	};
}
