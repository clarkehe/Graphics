#include "stdafx.h"
#include "../Shader/Shader.h"
#include "Common.h"

Color Shader_SimpleLambert(VertexOut& inVertex, Mesh& mesh, Light& light, Vec3f& cameraPos)
{
    float diff = (float)inVertex.normal.Dot(light.worldDir);
    //assert(diff > 0.f);
    diff = std::max(0.f, diff);
    
    Color albedo(1.0f, 0.3f, 0.5f, 1.0f);
    if (mesh.diffuseTextures.size() > 0)
        albedo = TextureMap(mesh.diffuseTextures[0], inVertex.uv);
    
    const Color c = albedo * light.color * diff * light.atten;
    return c;
}
