#include "InputManager.h"

InputManager::InputManager(uint8 playerIndex){
    if(Gamepad(playerIndex).isConnected() == true){
        //m_input = Gamepad(playerIndex);
    }else{
        //KeyBoardからの入力を受け付ける
    }
    m_playerIndex = playerIndex;
}

void InputManager::InputUpdate(){
    if (ProController(m_playerIndex).isConnected() == true){
        InputConvertProCon();
    }
    else if(Gamepad(m_playerIndex).isConnected() == true){
        //m_input = Gamepad(m_playerIndex);
        InputConvertGPad();
    }else{
        InputConvertKey();
    }
}

void InputManager::InputConvertProCon(){
    ProController input = ProController(m_playerIndex);
    lStickX = input.LStick().x;
    lStickY = input.LStick().y;
    aButton = input.buttonA.pressed();
}

void InputManager::InputConvertGPad(){
    const auto& input = Gamepad(m_playerIndex);
    const auto& padInfo = input.getInfo();
    //aボタンと右トリガーのインデックス
    std::array<uint8, 2> buttonIndexes = {2,7};
    //左スティックxとyのインデックス（右、下が1）
    std::array<uint8, 2> stickIndexes = {0,1};
    if(padInfo.productID == PID_DualShock){
        InputConvertDShock(buttonIndexes,stickIndexes);
    }

    aButton = input.buttons[buttonIndexes[0]].pressed();
    rTrigger = input.buttons[buttonIndexes[1]].pressed();
    lStickX = input.axes[stickIndexes[0]];
    lStickY = input.axes[stickIndexes[1]];
}

void InputManager::InputConvertKey(){
    
}

/*std::tuple<std::array<uint8,2>,std::array<uint8,2>>*/ void InputManager::InputConvertDShock(std::array<uint8,2>& buttonIndexes, std::array<uint8,2>& stickIndexes){
    //aボタンと右トリガーのインデックス
    buttonIndexes = {2,7};
    //左スティックxとyのインデックス（右、下が1）
    stickIndexes = {0,1};
    
    //return {buttonIndexes,stickIndexes};
}
