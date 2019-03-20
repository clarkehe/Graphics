#pragma once

namespace SoftRender 
{
	struct Material
	{
		Color ambient;
		Color diffuse;
		Color specular;
		float shininess;

		Material()
		{
			shininess = 0;
		}

		Material(const Color& _ambient, const Color& _diffuse, const Color& _specular, const float _shininess):
			ambient(_ambient), diffuse(_diffuse), specular(_specular), shininess(_shininess)
			{}
	};
}
