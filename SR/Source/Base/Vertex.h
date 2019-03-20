#pragma once

namespace SoftRender
{
	struct Vertex
    {
		Vec3f modelPos;
		Vec3f normal;
		Vec2f uv;
		Color color;
		float derivZ;
        
		Vertex()
        {
            derivZ = 0;
        }
	};

	struct VertexOut
    {
		Vec3f worldPos;
		Vec3f viewPos;
		Vec3f projPos;
        Vec2f screenPos;
		Vec3f normal;
		Vec2f uv;
		Color color;
		float derivZ;
        
		VertexOut()
        {
            derivZ = 0;
        }
	};
}

