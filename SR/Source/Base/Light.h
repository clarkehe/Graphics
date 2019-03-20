#pragma once

namespace SoftRender
{
	struct Light
    {
		Vec3f worldPos;
		Vec3f viewPos;

		Color ambient;
		Color diffuse;
		Color specular;

		Light()
        :worldPos(Vec3f(1.0, 1.0, 1.0))
        {}

		Light(const Vec3f& _pos,
              const Color& _ambient,
              const Color& _diffuse,
              const Color& _specular)
        :worldPos(_pos), ambient(_ambient), diffuse(_diffuse), specular(_specular){}
	};
}
