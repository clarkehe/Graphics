#pragma once
#include "Vector2.h"

namespace SoftRender
{
	//2x2
	class Matrix2
	{
	public:
		float r0[2];
		float r1[2];

	public:
		Matrix2(float r0_[2], float r1_[2])
		{
			this->r0[0] = r0_[0];
			this->r0[1] = r0_[1];

			this->r1[0] = r1_[0];
			this->r1[1] = r1_[1];
		}

		Matrix2 Inverse()
		{
			float delta = r0[0] * r1[1] - r0[1] * r1[0];
			assert(std::abs(delta) > 0.0001f);

			float a = r1[1]  / delta;
			float b = -r0[1] / delta;
			float c = -r1[0] / delta;
			float d = r0[0]  / delta;

			float r0_[2] = {a, b};
			float r1_[2] = {c, d};

			Matrix2 ret(r0_, r1_);
			return ret;
		}
	};

	//vector * matrix
	Vec2f operator * (const Vec2f &vector, const Matrix2 &matrix)
	{
		Vec2f ret;
		ret.x = vector.x * matrix.r0[0] + vector.y * matrix.r1[0];
		ret.y = vector.x * matrix.r0[1] + vector.y * matrix.r1[1];
		return ret;
	}
};

