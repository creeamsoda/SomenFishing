# include <Siv3D.hpp>
# include "Somen.h"


Somen::Somen(double positionX, double positionY){
    somenPos.x = positionX;
    somenPos.y = positionY;
    //state = flowing;
}

void Somen::Flow(){
    somenPos.x += veloX*Scene::DeltaTime();
    somenPos.y += veloY*Scene::DeltaTime();
    //pos.x += veloX;
    //pos.y += veloY;
}

void Somen::VelocitySet(double velocityX,double velocityY){
    veloX = velocityX;
    veloY = velocityY;
}

Circle Somen::ColliderGet(){
    Circle collider{somenPos, colliderRadius};
    return collider;
}

void Somen::Draw(Texture tex){
    tex.drawAt(somenPos);
}

