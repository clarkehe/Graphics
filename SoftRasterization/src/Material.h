#ifndef Material_h
#define Material_h

namespace SoftRender 
{
	struct Material
	{
		Color ambient;	//在环境光照下这个物体反射得是什么颜色，通常这是和物体颜色相同的颜色
		Color diffuse;	//在漫反射光照下物体的颜色。（和环境光照一样）
		Color specular;	//镜面光照对物体的颜色影响

		float shininess;//反光度，镜面高光的散射/半径

		Material()
		{
			shininess = 0;
		}

		Material(const Color& _ambient, const Color& _diffuse, const Color& _specular, const float _shininess):
			ambient(_ambient), diffuse(_diffuse), specular(_specular), shininess(_shininess)
			{}
	};
}

#endif
