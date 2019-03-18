//
//  Quaternion.cpp
//  Math
//
//  Created by Clarke on 2018/12/21.
//  Copyright © 2018 Clarke. All rights reserved.
//

#include "Quaternion.hpp"

Quaternion::Quaternion()
{
    x = y = z = 0;
    w = 1;
}

Quaternion::Quaternion(double x, double y, double z, double w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

void Quaternion::SetIndentity()
{
    x = y = z = 0;
    w = 1;
}
