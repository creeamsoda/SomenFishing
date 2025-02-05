#include "Generater.h"
#include <Siv3D.hpp>

Generater::Generater(){
    frequency = 1;
    passedTime = 0;
    m_somenIndex = 0;
    somenArray = {};
    genePosYMax = 400;
    genePosYMin = 100;
}
void Generater::Generate(){
    passedTime += Scene::DeltaTime();
    if(passedTime > frequency){
        somenArray << Somen(m_somenIndex, 0, static_cast<double>(Random(genePosYMin,genePosYMax)));
        passedTime -= frequency;
        m_somenIndex ++;

        for(int32 i=0;i<somenArray.size();i++){
            auto somenInArray = somenArray[i];
            std::cout << U"{}個目,index{},state{}"_fmt(i,somenInArray.index,somenInArray.GetState()) << std::endl;
        }
    }
}

Array<Somen> Generater::SomenArrayGet(){
    return somenArray;
}
//# endif


