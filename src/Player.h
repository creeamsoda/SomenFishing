#pragma once
#define PLAYER_H
#include <Siv3D.hpp>
#include "InputManager.h"
#include "Somen.h"

class Player
{
private:
    //プレイヤーの番号
    uint8 m_playerIndex;
    //スタート地点
    Vec2 m_spawnPoint;
    //テクスチャ
    Texture aimTex;
    Texture floaterTex;
    Texture swimmerTex;
    Texture drownerTex;
    //状態変数
    uint8 state;
    //aim状態の動き
    static constexpr double aimVelo = 200;
    //浮き状態の動き
    static constexpr double floaterVelo = 100;
    Vec2 floaterNowSpeed;
    Vec2 posLastFrame;
    Vec2 floaterGoingPos;
    static constexpr double floaterAccellation = 5;
    static constexpr double floaterDeceleration = 2;
    //catching状態
    uint32 catchingSomenIndex;
    //判定の大きさ
    static constexpr double catchColliderRadius = 50;
    static constexpr double robColliderRadius = 30;
    //robbed,robbing状態
    double robbedPower;
    double robbingPower;
    double robbedRobbingPowerRatio;
    static constexpr double robbedPwrIncrease = 1;
    static constexpr double robbingPwrIncrease = 1.5;
    static constexpr double robbedSideWinBorder = -5;
    static constexpr double robbingSideWinBorder = 10;
    //swimmig状態
    double stamina;
    static constexpr double staminaMax = 100;
    double riverFlowSpeed;
    static constexpr double dashStrength = 80;
    static constexpr double dashStaminaDecrease = 3;
    static constexpr double naturalStaminaDecrease = 1;
    static constexpr double eatColliderRadius = 30;
    static constexpr double eatStaminaRecover = 0;
    //drowning状態
    const int32 outOfWindowBuffer = 100; 
    //getting状態
    const int32 somenPoint = 1;
    static constexpr double getAreaHeight = 80;
public:
    Vec2 pos;
    int32 score;
    const uint8 aiming = 1;
    const uint8 floating = 2;
    const uint8 catching = 3;
    const uint8 robbing = 4;
    const uint8 robbed = 5;
    const uint8 getting = 6;
    const uint8 swimming = 7;
    const uint8 drowning = 8;

    Player(uint8 playerIndex, Texture aimTexte, Texture floaterTex, Texture swimmerTex,Texture drownerTex);
    void Respawn();
    void ChangeState(uint8 nextState);
    void StateUpdate(InputManager& input, Array<Somen>& somenArray);

    void PlayerMove(InputManager& input,InputManager& opponentPlayerInput, Array<Somen>& somenArray,Player& opponentPlayer);
    void AimMove(InputManager& input);
    void FloaterMove(InputManager& input);
    void CatcherMove(InputManager& input, Array<Somen>& somenArray);
    void RobbedMove(InputManager& input,InputManager& opponentPlayerInput, Array<Somen>& somenArray,Player& opponentPlayer);
    void SwimMove(InputManager& input);
    void DrownMove();

    void CollideJudge(Array<Somen>& somenArray, Player& opponentPlayer);
    void CatchCollideJudge(Array<Somen>& somenArray, Player& opponentPlayer);
    bool RobCollideJudge(Somen& somen, Player& opponentPlayer);
    void EatCollideJudge(Array<Somen>& somenArray);
    Somen& FindSomen(uint32 somenIndex, Array<Somen>& somenArray);
    void CatchSomen(Somen& somen,Array<Somen>& somenArray);
    void RobSomen(Somen& somen, Player& opponentPlayer);
    void GetSomen(Array<Somen>& somenArray);
    bool SomenArrayIndexCheck(Somen& somen);
    void Draw();
    void DrawRatioGaugeHorizontal(Vec2 centerPos,double width, double height, double leftSideRatio, ColorF rightColor, ColorF leftColor);
    void DrawGaugeHorizontal(Vec2 centerPos,double width, double height, double ratio, ColorF color);

    int8 PlayerIndexIsZeroThenOne();
};


