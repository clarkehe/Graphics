//
//  Vector4D.hpp
//  Math
//
//  Created by Clarke on 2018/12/25.
//  Copyright © 2018 Clarke. All rights reserved.
//

#ifndef Vector4D_hpp
#define Vector4D_hpp

class Vector4D
{
public:
    double x, y, z, w;
    
public:
    explicit Vector4D(double x, double y, double z, double w);
    explicit Vector4D(const Vector3D &v, double w);
    
    Vector3D ToVector3D() const;
};

std::ostream & operator << (std::ostream &out, const Vector4D &v);

#endif /* Vector4D_hpp */
