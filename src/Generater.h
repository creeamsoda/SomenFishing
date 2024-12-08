//
//  Generater.h
//  empty
//
//  Created by 栗栖想太 on 2024/12/06.
//

# ifndef GENERATER_H
//#pragma once
#define GENERATER_H
#include "Somen.h"
#include <Siv3D.hpp>

class Generater
{
private:
    double frequency;
    double passedTime;
    int32 genePosYMax;
    int32 genePosYMin;
public:
    Array<Somen> somenArray;
    Generater();
    void Generate();
    Array<Somen> SomenArrayGet();
};


# endif

