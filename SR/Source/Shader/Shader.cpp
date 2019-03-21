#include "stdafx.h"
#include "../Shader/Shader.h"
#include "Common.h"

//https://www.alanzucconi.com/2015/06/10/a-gentle-introduction-to-shaders-in-unity3d/
//https://blog.lujun.co/2019/03/16/unity_light_model/
//https://blog.csdn.net/v_xchen_v/article/details/79431266
//https://www.jianshu.com/p/4cad24dd6dcd

Color Shader_SimpleLambert(VertexOut& inVertex, Mesh& mesh, Light& light, Vec3f& camPos)
{
    Vec3f lightDir = light.pos - inVertex.worldPos;
    float diff = (float)inVertex.normal.Dot(lightDir.Normalize());
    
#if 0
    diff = std::max(0.f, diff);
#else
    //https://blog.lujun.co/2019/03/16/unity_light_model/
    // half Lambert
    diff = diff * 0.5 + 0.5;
#endif
    
    Color albedo(1.0f, 0.3f, 0.5f, 1.0f);
    if (mesh.diffuseTextures.size() > 0)
        albedo = TextureMap(mesh.diffuseTextures[0], inVertex.uv);
    
    const Color c = albedo * light.color * diff * light.atten;
    return c;
}

Color Shader_BlinnPhong(VertexOut& inVertex, Mesh& mesh, Light& light, Vec3f& camPos)
{
    const Vec3f lightDir = light.pos - inVertex.worldPos;
    const Vec3f viewDir  = camPos - inVertex.worldPos;
    
    //Half Vector
    const Vec3f h = (lightDir + viewDir).Normalize();
    float nh = (float)inVertex.normal.Dot(h);
    nh = std::max(0.f, nh);
    
    Color speculaColor(1.0f, 0.3f, 0.5f, 1.0f);
    if (mesh.specularTextures.size() > 0)
        speculaColor = TextureMap(mesh.specularTextures[0], inVertex.uv);
    
    const float Specular = 0.5f;    //[0, 1]
    const float Gloss    = 1.0f;    //specular intensity
    const float spec     = std::pow(nh, Specular * 128.0f) * Gloss;
    
    Color c = Shader_SimpleLambert(inVertex, mesh, light, camPos);
    c = c + (light.color * speculaColor * spec * light.atten);
    return c;
}

// Phong Model
Color Shader_Phong(VertexOut& inVertex, Mesh& mesh, Light& light, Vec3f& camPos)
{
    const Vec3f lightDir = light.pos - inVertex.worldPos;
    const Vec3f viewDir  = camPos - inVertex.worldPos;

    //Reflection Vector
    float ldDot = lightDir.Dot(inVertex.normal);
    ldDot = std::max(0.f, ldDot);
    ldDot *= 2;
    const Vec3f relDir = (inVertex.normal * ldDot - lightDir).Normalize();
    
    float nh = (float)viewDir.Dot(relDir);
    nh = std::max(0.f, nh);
    
    Color speculaColor(1.0f, 0.3f, 0.5f, 1.0f);
    if (mesh.specularTextures.size() > 0)
        speculaColor = TextureMap(mesh.specularTextures[0], inVertex.uv);
    
    const float Specular = 0.5f;    //[0, 1]
    const float Gloss    = 1.0f;    //specular intensity
    const float spec     = std::pow(nh, Specular * 128.0f) * Gloss;
    
    Color c = Shader_SimpleLambert(inVertex, mesh, light, camPos);
    c = c + (light.color * speculaColor * spec * light.atten);
    return c;
}
