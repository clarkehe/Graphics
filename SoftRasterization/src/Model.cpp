#include "Model.h"
#include <fstream>
#include <sstream>
#include "TextureManager.h"
#include <vector>
#include "Texture.h"

namespace SoftRender
{ 
	Model::Model(std::string path, Vec3f worldPos, Material m):path(path), pos(worldPos),material(m)
	{
		loadModel(path);
	}

	void Model::loadModel(string path)
	{
		Assimp::Importer import;
		const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate|aiProcess_FlipUVs|aiProcess_GenNormals);
		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
			return ;
		} 

		directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode *node, const aiScene *scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
	{
		vector<Vertex> vertices;
		vector<unsigned int> indices;

		std::vector<Texture> ambientTextures;
		std::vector<Texture> diffuseTextures;
		std::vector<Texture> specularTextures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			if (mesh->HasPositions()) {
				vertex.modelPos = Vec3f(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			}

			if (mesh->HasNormals()) {
				vertex.normal = Vec3f(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			}

			if (mesh->mTextureCoords[0]) {
				vertex.uv = Vec2f(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			}
			else
			{
				vertex.uv = Vec2f();
			}
			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			if (face.mNumIndices != 3) {
				std::cout << face.mNumIndices;
			}

			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		if(mesh->mMaterialIndex > 0)
		{
			aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];

			//物体Color贴图
			std::vector<Texture> ambientMaps  = loadMaterialTextures(mat, aiTextureType_AMBIENT, "texture_ambient");
			ambientTextures.insert(ambientTextures.end(), ambientMaps.begin(), ambientMaps.end());

			//漫反射贴图
			std::vector<Texture> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
			diffuseTextures.insert(diffuseTextures.end(), diffuseMaps.begin(), diffuseMaps.end());

			//镜面贴图
			vector<Texture> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
			specularTextures.insert(specularTextures.end(), specularMaps.begin(), specularMaps.end());
		}

		return Mesh(vertices, indices, ambientTextures, diffuseTextures, specularTextures);
	}

	vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
	{
		std::cout << "loadMaterialTextures, type: " << typeName;
		std::cout << ", count:"<< mat->GetTextureCount(type) << std::endl;

		vector<Texture> textures;
		for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			Texture texture;
			std::string path(str.C_Str());

			if (path.find("materials") != string::npos)
				path = path.substr(path.find("materials"), path.length());

			TextureFromFile(texture, path, directory);
			texture.type = typeName;
			texture.path = string(directory + "/" + path);

			if (texture.width > 0 && texture.height > 0)
			{
				textures.push_back(texture);
			}
			else
			{
				std::cout << "texture size is zero, path:" << texture.path << std::endl;
			}
		}
		return textures;
	}

	unsigned int Model::TextureFromFile(Texture& texture, const string& path, const string& directory)
	{
		string filename = directory + "/" + path;
		TextureManager::getInstance()->LoadTexture(texture, filename);
		return 0;
	}
}
