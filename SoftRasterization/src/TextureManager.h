#ifndef TextureManager_h
#define TextureManager_h

#include <map>
#include <vector>
#include "Common.h"

namespace SoftRender
{
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
}

#endif