#pragma once
#define PLAYER_H
#include <Siv3D.hpp>
#include "InputManager.h"
#include "Somen.h"

class Player
{
private:
    /* data */
    uint8 m_playerIndex;
    Texture aimTex;
    Texture floaterTex;
    uint8 state;
    const uint8 aiming = 1;
    const uint8 floating = 2;
    const uint8 catching = 3;
    const uint8 robbed = 4;
    const uint8 getting = 5;
    static constexpr double aimVelo = 200;
    //浮き状態の動き
    static constexpr double floaterVelo = 100;
    Vec2 floaterNowSpeed;
    Vec2 posLastFrame;
    Vec2 floaterGoingPos;
    static constexpr double floaterAccellation = 0.5;
    static constexpr double floaterDeceleration = 2;
public:
    Vec2 pos;
    Player(uint8 playerIndex, Texture aimTexte, Texture floaterTex);
    void ChangeState(uint8 nextState);
    void StateUpdate(InputManager& input);
    void PlayerMove(InputManager& input);
    void AimMove(InputManager& input);
    void FloaterMove(InputManager& input);
    void CollideJudge(Array<Somen>& somenArray);
    void CatchCollideJudge(Array<Somen>& somenArray);
    void Draw();
};


