#pragma once

enum DrawMode
{
    Textured, WireFrame
};

Mat4f ModelMatrix(Vec3f & translate);
Mat4f ViewMatrix(const Vec3f& look, const Vec3f& at, const Vec3f& up);
Mat4f Perspective(float radians, float ratio, float near, float far);

Vec3f operator*(const Vec3f& d, const Mat4f& m );
Vec3 MultDirMatrix(const Vec3& d, const Matrix4& m);
Vec3 MultPointMatrix(const Vec3& d, const Matrix4& m);

template<typename T>
T min3(T a, T b, T c)
{
    return std::min(a, std::min(b, c));
}

template<typename T>
T max3(T a, T b, T c)
{
    return std::max(a, std::max(b, c));
}

void SaveTexture(Color *frameBuffer, int width, int height, std::string file);
bool LoadTexture (Texture &texture, std::string file);

void VertexShader(Mat4f& model, Mat4f& view, Mat4f& proj, Vertex& inVertex, VertexOut& outVertex);
bool Clip(VertexOut& v);
Vec2f Ndc2Screen (Vec3f &pos, float width, float height);
bool BackFaceCulling (const Vec3f &p0, const Vec3f &p1, const Vec3f &p2);

Color PixelShader(VertexOut& inVertex, Texture& texture, Light& light, Vec3f& cameraPos, Material& material);
Color PixelShader(VertexOut& inVertex, Mesh& mesh, Light& light, Vec3f& cameraPos);
Color PixelShader(VertexOut& inVertex, Light& light, Vec3f& cameraPos, Material& material);

Color BilinearFilter(Texture& texture, float s, float t);
Color NearestFilter(Texture& texture, int s, int t);

Vec4f operator*(const Vec4f& d, const Mat4f& m );

