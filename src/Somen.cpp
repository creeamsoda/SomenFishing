# include <Siv3D.hpp>
# include "Somen.h"


Somen::Somen(uint32 somenIndex, double positionX, double positionY){
    index = somenIndex;
    somenState = flowing;
    somenPos.x = positionX;
    somenPos.y = positionY;
    //state = flowing;
}

void Somen::Flow(){
    if(somenState == flowing){
        somenPos.x += veloX*Scene::DeltaTime();
        somenPos.y += veloY*Scene::DeltaTime();
    }
    if(somenState != flowing){
        //Print << U"{}中です.({},{})"_fmt(somenState,somenPos.x, somenPos.y);
    }
}

void Somen::VelocitySet(double velocityX,double velocityY){
    veloX = velocityX;
    veloY = velocityY;
}

Circle Somen::ColliderGet(){
    Circle collider{somenPos, colliderRadius};
    return collider;
}

void Somen::ChangeState(uint8 nextState){
    switch (nextState)
    {
    case flowing:
        somenState = flowing;
        break;
    case caught:
        somenState = caught;
        Print << U"caught";
        break;
    case robbed:
        somenState = robbed;
        break;
    case gotten:
        somenState = gotten;
        break;
    default:
        Print << U"changeState失敗";
        break;
    }
}

uint8 Somen::GetState(){
    return somenState;
}

void Somen::Draw(Texture tex){
    tex.scaled(0.1).drawAt(somenPos);
}

