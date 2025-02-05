#include "InputManager.h"

InputManager::InputManager(uint8 playerIndex){
    if(Gamepad(playerIndex).isConnected() == true){
        //m_input = Gamepad(playerIndex);
    }else{
        //KeyBoardからの入力を受け付ける
    }
    m_playerIndex = playerIndex;
    rStickX = 0;
    rStickY = 0;
    lStickX = 0;
    lStickY = 0;
    aButton = false;
    aButtonDown = false;
    rTrigger = false;
    rTriggerDown = false;
}

void InputManager::InputUpdate(){
    m_aButtonLastFrame = aButton;
    m_rTriggerLastFrame = rTrigger;
    if (ProController(m_playerIndex).isConnected() == true){
        InputConvertProCon();
    }
    else if(Gamepad(m_playerIndex).isConnected() == true){
        //m_input = Gamepad(m_playerIndex);
        InputConvertGPad();
    }else{
        InputConvertKey();
    }
    aButtonDown = JudgeButtonDown(aButton,m_aButtonLastFrame);
    rTriggerDown = JudgeButtonDown(rTrigger,m_rTriggerLastFrame);
}

bool InputManager::JudgeButtonDown(bool button, bool buttonLastFrame){
    if(button == true){
        if(buttonLastFrame == false){
            //現在入力あり、前フレーム入力なしならbuttonDown
            return true;
        }else{
            //現在入力あり、前フレーム入力ありならbutton長押し
            return false;
        }
    }else{
        //現在入力なしならボタン入力なし
        return false;
    }
}

void InputManager::InputConvertProCon(){
    ProController input = ProController(m_playerIndex);
    lStickX = input.LStick().x;
    lStickY = input.LStick().y;
    aButton = input.buttonA.pressed();
    rTrigger = input.buttonZR.pressed();
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
    aButton = KeyC.pressed(); //CircleのC
    rTrigger = KeyR.pressed();
    lStickX = KeyToOneZero(KeyD)-KeyToOneZero(KeyA);
    lStickY = KeyToOneZero(KeyS)-KeyToOneZero(KeyW);
}

/*std::tuple<std::array<uint8,2>,std::array<uint8,2>>*/ void InputManager::InputConvertDShock(std::array<uint8,2>& buttonIndexes, std::array<uint8,2>& stickIndexes){
    //aボタンと右トリガーのインデックス
    buttonIndexes = {2,7};
    //左スティックxとyのインデックス（右、下が1）
    stickIndexes = {0,1};
    
    //return {buttonIndexes,stickIndexes};
}

int8 InputManager::KeyToOneZero(Input Key){
    if(Key.pressed() == true){
        return 1;
    }else{
        return 0;
    }
}
