//#ifndef SOMEN_H
#pragma once
#define SOMEN_H
#include <Siv3D.hpp>

class Somen
{
private:
    double veloX;
    double veloY;
    uint8 somenState;
    static constexpr double colliderRadius = 50;
    
public:
    uint32 index;
    Vec2 somenPos;
    static const uint8 flowing = 1;
    static const uint8 caught = 2;
    static const uint8 robbed = 3;
    static const uint8 gotten = 4;
    Somen(uint32 somenIndex,double positionX,double positionY);
    void VelocitySet(double velocityX,double velocityY);
    void Flow();
    Circle ColliderGet();
    void ChangeState(uint8 nextState);
    uint8 GetState();
    void Draw(Texture tex);
};

//#endif

