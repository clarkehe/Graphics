//
//  Color.hpp
//  Math
//
//  Created by Clarke on 2019/3/20.
//

#ifndef Color_hpp
#define Color_hpp

class Color
{
public:
    union
    {
        struct
        {
            float r, g, b, a;
        };
        
        struct
        {
            float R, G, B, A;
        };
    };
    
    explicit Color()
    {
        this->r = 1.0;
        this->g = 1.0;
        this->b = 1.0;
        this->a = 1.0;
    }
    
    explicit Color(float r, float g, float b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = 1.0;
    }
    
    explicit Color(float r, float g, float b, float a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
    
    Color operator+(const Color &c) const
    {
        return Color(this->r + c.r,
                     this->g + c.g,
                     this->b + c.b,
                     this->a + c.a);
    }
    
    
    Color operator*(const Color &c) const
    {
        return Color(this->r * c.r,
                     this->g * c.g,
                     this->b * c.b,
                     this->a * c.a);
    }
    
    Color operator*(float d) const
    {
        return Color(this->r * d,
                     this->g * d,
                     this->b * d,
                     this->a * d);
    }
    
    Color operator*(double d) const
    {
        return Color(this->r * d,
                     this->g * d,
                     this->b * d,
                     this->a * d);
    }
};

#endif /* Color_hpp */
