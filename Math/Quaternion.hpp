//
//  Quaternion.hpp
//  Math
//
//  Created by Clarke on 2018/12/21.
//  Copyright © 2018 Clarke. All rights reserved.
//

#ifndef Quaternion_hpp
#define Quaternion_hpp

class Quaternion
{
public:
    double x, y, z, w;
    
    explicit Quaternion();
    explicit Quaternion(double x, double y, double z, double w);
    
    void SetIndentity();
};

#endif /* Quaternion_hpp */
