#pragma once
#define INPUTMANAGER_H
#include <Siv3D.hpp>

class InputManager
{
private:
    /* data */
    uint8 m_playerIndex;
    const int32 PID_DualShock = 1476;

    bool m_aButtonLastFrame;
    bool m_rTriggerLastFrame;
public:
    double rStickX;
    double rStickY;
    double lStickX;
    double lStickY;
    bool aButton;
    bool aButtonDown;
    bool rTrigger;
    bool rTriggerDown;
    
    InputManager(uint8 playerIndex);
    void InputUpdate();
    bool JudgeButtonDown(bool button, bool buttonLastFrame);
    void InputConvertProCon();
    void InputConvertGPad();
    /*std::tuple< std::array<uint8,2>, std::array<uint8,2>>*/void InputConvertDShock(std::array<uint8,2>& buttonIndexes, std::array<uint8,2>& stickIndexes);
    void InputConvertKey();
    int8 KeyToOneZero(Input Key);
};
