#include "stdafx.h"
#include "Render.h"

static bool OnSameSide(Vec3f a, Vec3f b, Vec3f c, Vec3f p)
{
    return (b-a).Cross(c-a).Dot((b-a).Cross(p-a)) >= 0;
}

static bool PointInTrangleBySameSide(Vec3f a, Vec3f b, Vec3f c, Vec3f p)
{
    return OnSameSide(a, b, c, p) &&
    OnSameSide(b, c, a, p)&&
    OnSameSide(c, a, b, p);
}

//advanced Rasterization from http://forum.devmaster.net/t/advanced-rasterization/6145/1
//http://blackpawn.com/texts/pointinpoly/default.html
static bool PointInTrangleByCentroid(Vec3f a, Vec3f b, Vec3f c, Vec3f p)
{
    Vec3f v0 = c - a ;
    Vec3f v1 = b - a ;
    Vec3f v2 = p - a ;
    
    float dot00 = v0.Dot(v0) ;
    float dot01 = v0.Dot(v1) ;
    float dot02 = v0.Dot(v2) ;
    float dot11 = v1.Dot(v1) ;
    float dot12 = v1.Dot(v2) ;
    
    float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01) ;
    
    float u = (dot11 * dot02 - dot01 * dot12) * inverDeno ;
    if (u < 0 || u > 1) // if u out of range, return directly
    {
        return false ;
    }
    
    float v = (dot00 * dot12 - dot01 * dot02) * inverDeno ;
    if (v < 0 || v > 1) // if v out of range, return directly
    {
        return false ;
    }
    
    return u + v <= 1 ;
}

// note that the result of edge function could be represent as area as well.
static inline float calculateArea (const Vec3f &p0, const Vec3f &p1, const Vec3f &p2)
{
    return ((p2.x - p0.x) * (p1.y - p0.y) - (p2.y - p0.y) * (p1.x - p0.x));
}

static inline bool zTest(float z, float zbuff)
{
    return z < zbuff;
}

static inline void Interpolate (const VertexOut &v0, const VertexOut &v1, const VertexOut &v2, VertexOut &v, Vec3f &w)
{
    float area = calculateArea (v0.projPos, v1.projPos, v2.projPos);

    w.x = calculateArea (v1.projPos, v2.projPos, v.projPos) / area;
    w.y = calculateArea (v2.projPos, v0.projPos, v.projPos) / area;
    w.z = calculateArea (v0.projPos, v1.projPos, v.projPos) / area;

    v.derivZ = v0.derivZ*w.x + v1.derivZ*w.y + v2.derivZ*w.z;
    v.projPos.z = 1/v.derivZ;// 1/v.derivZ=观察空间的Z坐标

    v.uv = (v0.uv * w.x + v1.uv * w.y + v2.uv * w.z) * (1/v.derivZ);
    v.color = (v0.color*w.x + v1.color*w.y + v2.color*w.z)/v.derivZ;

    v.normal = (v0.normal*w.x + v1.normal*w.y + v2.normal*w.z);
    v.worldPos = (v0.worldPos*w.x + v1.worldPos*w.y + v2.worldPos*w.z);
    v.viewPos = (v0.viewPos*w.x + v1.viewPos * w.y + v2.viewPos*w.z);
    v.viewPos = v.viewPos * (1.0/v.derivZ);
}

void Render::SetPixel (int x, int y, const Color &color, float depth = 0)
{
    frameBuffer[x + y * width] = color;
    depthBuffer[x+y*width] = depth;
}

void Render::DrawLine (const Vec3f &p0, const Vec3f &p1, const Color &color)
{
    int start, end;
    if (p0.x < p1.x)
    {
        start = (int)std::ceill (p0.x);
        end   = (int)std::floor (p1.x);
    }
    else
    {
        start = (int)std::ceill (p1.x);
        end  = (int)std::floor (p0.x);
    }

    int y0 = (int)std::floor (p0.y), y1 = (int)std::floor(p1.y);
    int startY = std::min(y0, y1);
    int endY   = std::max(y0, y1);

    if (start == end || std::abs(p0.x - p1.x) < 0.5f)
    {
        for (int y=startY; y<=endY; y++)
        {
            int dx = start;
            int dy = y;
            SetPixel(dx, dy, color);
        }
        return;
    }

    double r0[2] = { p0.x , p1.x};
    double r1[2] = { 1, 1};
    
    Matrix2x2 m(r0, r1);
    m = m.Transpose();
    m = m.Inverse();
    
    const Vector2D y(p0.y, p1.y);

    //y = a * x + b
    const Vector2D res = m * y;
    float a = res.x;
    float b = res.y;

#ifdef _DEBUG
    {
        float tem = ( p0.y - p1.y) / (p0.x - p1.x);
        assert(std::abs(tem - a ) < 0.0001f);
    }
#endif // _DEBUG
    
    for (int x=start; x <= end; x++)
    {
        int dx = x;
        int dy = a * x + b;

        assert(dy <= endY + 1);
        SetPixel(dx, dy, color);
    }
} // bresenham line algorithm

void Render::DrawTriangle (const VertexOut &v0, const VertexOut &v1, const VertexOut &v2, const Color &color)
{
    DrawLine (v0.projPos, v1.projPos, color);
    DrawLine (v1.projPos, v2.projPos, color);
    DrawLine (v0.projPos, v2.projPos, color);
}

void Render::Rasterization(Mesh& mesh, VertexOut& v1, VertexOut& v2, VertexOut& v3)
{
    // Bounding rectangle
    const int minx = min3(v1.screenPos.x, v2.screenPos.x, v3.screenPos.x);
    const int maxx = max3(v1.screenPos.x, v2.screenPos.x, v3.screenPos.x);

    const int miny = min3(v1.screenPos.y, v2.screenPos.y, v3.screenPos.y);
    const int maxy = max3(v1.screenPos.y, v2.screenPos.y, v3.screenPos.y);

    Vec3f weight;
    for(int y = miny; y <= maxy; y++)
    {
        for(int x = minx; x <= maxx; x++)
        {
            VertexOut v;
            v.projPos = Vec3f(x+0.5, y+0.5, 0);

            //???
            if (!PointInTrangleBySameSide(v1.projPos, v2.projPos, v3.projPos, v.projPos)) continue;

            //??
            Interpolate(v1, v2, v3, v, weight);

            if (zTest(v.projPos.z, depthBuffer[x+y*width]))
            {
                if (mesh.diffuseTextures.size() > 0 && mesh.specularTextures.size() > 0)
                    SetPixel(x, y, PixelShader(v, mesh, mLight, mCameraPos), v.projPos.z);
                else
                    SetPixel(x, y,  PixelShader(v, mLight, mCameraPos, mCurMaterial), v.projPos.z);
            }
        }
    }
}

void Render::DrawMesh(Mesh& mesh, Vertex& v0, Vertex& v1, Vertex&v2)
{
    VertexOut outVertex[3];
    
    VertexShader(mModelMatrix, mViewMatrix, mProjMatrix, v0, outVertex[0]);
    VertexShader(mModelMatrix, mViewMatrix, mProjMatrix, v1, outVertex[1]);
    VertexShader(mModelMatrix, mViewMatrix, mProjMatrix, v2, outVertex[2]);

    for(int i=0; i<3; i++)
    {
        auto &v = outVertex[i];

        //三角形三个顶点：有一个被Clip就return？
        if (Clip(v)) return;

        //
        v.screenPos = Ndc2Screen (v.projPos, width, height);
        
        //???
        v.projPos.x = v.screenPos.x;
        v.projPos.y = v.screenPos.y;
    }

    //
    if (BackFaceCulling (outVertex[0].viewPos,outVertex[1].viewPos,outVertex[2].viewPos))
        return;

    if (currentMode == Textured)
        Rasterization(mesh, outVertex[0], outVertex[1], outVertex[2]);
    else
        DrawTriangle (outVertex[0], outVertex[1], outVertex[2], Color(0, 1.0f, 0, 0 ));
}

void Render::DrawModel (Model &model)
{
    mModelMatrix = ModelMatrix(model.mPos);
    mvMat = mModelMatrix * mViewMatrix;
    mvpMat = mvMat * mProjMatrix;
    nmvMat = mvMat.Inverse().Transpose ();

    // we need light position(in view space) in pixel shader???
    mLight.viewPos = MultPointMatrix(mLight.worldPos, mvMat);
    
    for (int i = 0; i < model.mMeshes.size(); i++)
    {
        Mesh &mesh = model.mMeshes[i];
        
        std::cout << "\nStart render mesh:" << i << std::endl;
        std::cout << "Ambient texture count:"   << mesh.ambientTextures.size() << std::endl;
        std::cout << "Diffuse texture count:"   << mesh.diffuseTextures.size() << std::endl;
        std::cout << "Specular texture count: " << mesh.specularTextures.size() << std::endl;

        for (int j = 0; j < mesh.indices.size(); j+=3)
            DrawMesh(mesh, mesh.vertices[mesh.indices[j]], mesh.vertices[mesh.indices[j+1]], mesh.vertices[mesh.indices[j+2]]);
    }
}
