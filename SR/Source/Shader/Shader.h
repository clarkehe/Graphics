#pragma once

// Simple Lambert Model
Color Shader_SimpleLambert(VertexOut& inVertex, Mesh& mesh, Light& light, Vec3f& camPos);

//Blinn Phong Model
Color Shader_BlinnPhong(VertexOut& inVertex, Mesh& mesh, Light& light, Vec3f& camPos);

// Phong Model
Color Shader_Phong(VertexOut& inVertex, Mesh& mesh, Light& light, Vec3f& camPos);

