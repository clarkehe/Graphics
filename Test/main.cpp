//
//  main.cpp
//  Test
//
//  Created by Clarke on 2018/12/18.
//  Copyright © 2018 Clarke. All rights reserved.
//

#include <iostream>
#include "../Math/Vector2D.hpp"
#include "../Math/Vector3D.hpp"
#include "../Math/Vector4D.hpp"
#include "../Math/Matrix4x4.hpp"
#include "../Math/Matrix2x2.hpp"

int main(int argc, const char * argv[])
{
    Vector3D v;
    v.x = 1;
    v.y = 2;
    v.z = 3;
    
    Vector4D v1(v, 1);
    Vector4D v2(v, 0);
    
    Matrix4x4 t = Matrix4x4::Translate(Vector3D(3, 2, 1));
    
    auto v11 = t * v1;
    auto v21 = t * v2;
    
    std::cout << "point, v1: " << v1 << "v11: " << v11 << std::endl ;
    std::cout << "vector, v2: " << v2 << "v21: " << v21 << std::endl ;
    
    Vector3D x(1, 0, 0);
    Vector3D y(0, 1, 0);
    Vector3D z(0, 0, 1);
    Matrix4x4 r = Matrix4x4::RotateAroundX(-3.1415926f * 0.5f); //90 degree
    
    auto x1 = r * x;
    auto y1 = r * y;
    auto z1 = r * z;
    
    std::cout << "x: " << x << "x1: " << x1 << std::endl ;
    std::cout << "y: " << y << "y1: " << y1 << std::endl ;
    std::cout << "z: " << z << "z1: " << z1 << std::endl ;
    
    Matrix2x2 m(1, 3, 2, 7);
    Matrix2x2 mi = m.Inverse();
    Matrix2x2 i = m * mi;
    
    std::cout << "m:" << m;
    std::cout << "mi:" << mi;
    std::cout << "i:" << i;
    
    return 0;
}
