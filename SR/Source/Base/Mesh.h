#pragma once
#include <vector>
#include "Vertex.h"
#include "Texture.h"

namespace SoftRender
{
	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		std::vector<Texture> ambientTextures;
		std::vector<Texture> diffuseTextures;
		std::vector<Texture> specularTextures;

		Mesh(std::vector<Vertex>& _vertices, 
			std::vector<unsigned int>& _indices,
			std::vector<Texture>& _ambientTextures,
			std::vector<Texture>& _diffuseTextures,
			std::vector<Texture>& _specularTextures)

		{
			vertices = _vertices;
			indices = _indices;
			ambientTextures = _ambientTextures;
			diffuseTextures = _diffuseTextures;
			specularTextures = _specularTextures;
		}
	};
}
