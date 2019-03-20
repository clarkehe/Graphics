//
//  Matrix2x2.hpp
//  Math
//
//  Created by Clarke on 2018/12/20.
//  Copyright © 2018 Clarke. All rights reserved.
//

#ifndef Matrix2x2_hpp
#define Matrix2x2_hpp

class Matrix2x2
{
public:
    union
    {
        struct
        {
            double m00, m01;
            double m10, m11;
        };
        
        struct
        {
            double r0[2];
            double r1[2];
        };
        
        double m[4];
    };
    
public:
    explicit Matrix2x2()
    {
        memset(m, 0x00, sizeof(m));
    }
    
    explicit Matrix2x2(double m00, double m01,
                       double m10, double m11)
    {
        this->m00 = m00;
        this->m01 = m01;
        this->m10 = m10;
        this->m11 = m11;
    }
    
    explicit Matrix2x2(double r0[2], double r1[2])
    {
        memcpy(this->r0, r0, sizeof(this->r0));
        memcpy(this->r1, r1, sizeof(this->r1));
    }
    
    explicit Matrix2x2(double m[4])
    {
        memcpy(this->m, m, sizeof(this->m));
    }
    
    void SetIndentity()
    {
        memset(m, 0x00, sizeof(m));
        this->m00 = this->m11 = 1;
    }
    
    //    this * m
    Matrix2x2 operator*(const Matrix2x2 &m) const
    {
        Matrix2x2 mm;
        
        mm.m00 = this->m00 * m.m00 + this->m01 * m.m10;
        mm.m10 = this->m10 * m.m00 + this->m11 * m.m10;
        
        mm.m01 = this->m00 * m.m01 + this->m01 * m.m11;
        mm.m11 = this->m10 * m.m01 + this->m11 * m.m11;
        
        return mm;
    }
    
    //    this * v2
    Vector2D operator*(const Vector2D &v) const
    {
        Vector2D vv;
        
        vv.x = this->m00 * v.x + this->m01 * v.y;
        vv.y = this->m10 * v.x + this->m11 * v.y;
        
        return vv;
    }
    
    Matrix2x2 Transpose() const
    {
        Matrix2x2 m;
        
        m.m00 = this->m00;
        m.m01 = this->m10;
        
        m.m10 = this->m01;
        m.m11 = this->m11;
        
        return m;
    }
    
    Matrix2x2 Inverse() const
    {
        const double a = this->m00;
        const double b = this->m01;
        const double c = this->m10;
        const double d = this->m11;
        
        Matrix2x2 m;
        
        double delta = a*d - c*b;
        if (delta < 0)
            delta *= -1.0;
        assert( delta > 0.00001f);
        
        m.m00 = d / (a*d - c*b);
        m.m10 = c / (c*b - a*d);
        
        m.m01 = b / (c*b - a*d);
        m.m11 = a / (a*d - c*b);
        
        return m;
    }
};

inline std::ostream & operator << (std::ostream &out, const Matrix2x2 &m)
{
    out << m.m00 << "," << m.m01 << std::endl;
    out << m.m10 << "," << m.m11 << std::endl;
    return out;
}

#endif /* Matrix2x2_hpp */
