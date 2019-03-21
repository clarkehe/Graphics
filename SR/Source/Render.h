#pragma once
#include "Common.h"
#include "Model.h" 

class Render
{
public:
    int width, height;
    Color *frameBuffer;
    std::vector<float> depthBuffer;

    Mat4f mModelMatrix;
    Mat4f mProjMatrix;
    Mat4f mViewMatrix;

    Mat4f mvMat, mvpMat, nmvMat;
    DrawMode currentMode;

private:
    Light mLight;
    Vec3f mCameraPos;

public:
    Render (int w, int h) :
        width (w),
        height (h),
        depthBuffer (w * h, std::numeric_limits<float>::max ())
    {
        frameBuffer = new Color[w *h];
        currentMode = Textured;
    }

    void SetFrustum (float hfov, float ratio, float n, float f)
    {
        mProjMatrix = Perspective (hfov, ratio, n, f);
    }

    void SetCameraPos (const Vec3f &pos)
    {
        mCameraPos = pos;
        mViewMatrix = ViewMatrix (pos, Vec3f(), Vec3f(0.0f, 1.0f, 0.0f));
    }

    void SetLight (const Light& _light)
    {
        mLight = _light;
    }

    void DrawModel (Model &model);

private:
    void SetPixel (int x, int y, const Color &color, float depth);
    void Rasterization(Mesh&mesh, VertexOut& v0, VertexOut& v1, VertexOut& v2);
    void DrawMesh (Mesh&mesh, Vertex &v0, Vertex &v1, Vertex &v2);
    void DrawLine (const Vec3f &p0, const Vec3f &p1, const Color &color);

    void DrawTriangle(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3, const Color& color);
    void FillTriangle(Mesh& mesh, Vertex& v1, Vertex& v2, Vertex& v3);
    void DrawLineInternal (int x0, int y0, int x1, int y1, const Color &color, bool steep);
};

