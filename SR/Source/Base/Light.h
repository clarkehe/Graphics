#pragma once

namespace SoftRender
{
	struct Light
    {
		Vec3f pos;
		Color color;
        float atten;
        
        Light()
        {
            atten = 1.0f;
        }
    };
}
