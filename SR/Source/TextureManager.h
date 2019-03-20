#ifndef TextureManager_h
#define TextureManager_h

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
