#ifndef Model_h
#define Model_h

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace SoftRender
{
	class Model
	{ 
	public:
		Model(std::string path, Vec3f worldPos, Material m);

	private:
		friend class Render;

		Vec3f mPos;
		Material mMaterial;
        std::vector<Mesh> mMeshes;

	private:
		std::string mPath;
		std::string mDirectory;

        void loadModel(std::string path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
		unsigned int TextureFromFile(Texture& texture, const std::string& path, const std::string& directory);
	};
}

#endif
