//
//  Generater.cpp
//  empty
//
//  Created by 栗栖想太 on 2024/12/06.
//

//# ifndef GENERATER_H
//#pragma once
//#define GENERATER_H
/*
class Generater
{
private:
    int m_num;
    //int m_num2;
public:
    Generater(int a){
        m_num = a;
    };
};
*/

#include "Generater.h"
#include <Siv3D.hpp>

Generater::Generater(){
    frequency = 1;
    passedTime = 0;
    somenArray = {};
    genePosYMax = 400;
    genePosYMin = 100;
}
void Generater::Generate(){
    passedTime += Scene::DeltaTime();
    if(passedTime > frequency){
        somenArray << Somen(0, static_cast<double>(Random(genePosYMin,genePosYMax)));
        passedTime -= frequency;
    }
}

Array<Somen> Generater::SomenArrayGet(){
    return somenArray;
}
//# endif


