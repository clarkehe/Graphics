#include "stdafx.h"
#include "../Shader/Shader.h"
#include "Common.h"

Color Shader_SimpleLambert(VertexOut& inVertex, Mesh& mesh, Light& light, Vec3f& camPos)
{
    Vec3f lightDir = light.pos - inVertex.worldPos;
    float diff = (float)inVertex.normal.Dot(lightDir.Normalize());
    diff = std::max(0.f, diff);
    
    Color albedo(1.0f, 0.3f, 0.5f, 1.0f);
    if (mesh.diffuseTextures.size() > 0)
        albedo = TextureMap(mesh.diffuseTextures[0], inVertex.uv);
    
    const Color c = albedo * light.color * diff * light.atten;
    return c;
}

Color Shader_BlinnPhong(VertexOut& inVertex, Mesh& mesh, Light& light, Vec3f& camPos)
{
    const Vec3f lightDir = light.pos - inVertex.worldPos;
    const Vec3f viewDir = camPos - inVertex.worldPos;
    
    const Vec3f h = (lightDir + viewDir).Normalize();
    float nh = (float)inVertex.normal.Dot(h);
    nh = std::max(0.f, nh);
    
    Color speculaColor(1.0f, 0.3f, 0.5f, 1.0f);
    if (mesh.specularTextures.size() > 0)
        speculaColor = TextureMap(mesh.specularTextures[0], inVertex.uv);
    
    const float Specular = 0.5f;    //[0, 1]
    const float Gloss    = 1.0f;   //specular intensity
    const float spec     = std::pow(nh, Specular * 128.0f) * Gloss;
    
    Color c = Shader_SimpleLambert(inVertex, mesh, light, camPos);
    c = c + (light.color * speculaColor * spec * light.atten);
    return c;
}
