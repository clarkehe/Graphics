#ifndef Model_h
#define Model_h
#include "Common.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
 
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
		vector<Mesh> mMeshes;

	private:
		std::string mPath;
		std::string mDirectory;

		void loadModel(string path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);

		vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
		unsigned int TextureFromFile(Texture& texture, const string& path, const string& directory);
	};
}

#endif
