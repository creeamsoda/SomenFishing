#include "Player.h"

Player::Player(uint8 playerIndex, Texture aimTexture, Texture floaterTexture){
    m_playerIndex = playerIndex;
    aimTex = aimTexture;
    floaterTex = floaterTexture;
    //state = aiming;
    ChangeState(aiming);
}

//stateを変えて同時に初期化を行う関数
void Player::ChangeState(uint8 nextState){
    if(nextState == aiming){
        state = aiming;
    }else if(nextState == floating){
        state = floating;
        //初期化
        floaterNowSpeed = {0,0};
        floaterGoingPos = {0,0};
        posLastFrame = pos;
    }else if(nextState == catching){
        state = catching;
    }else if(nextState == robbed){
        state = robbed;
    }else if(nextState == getting){
        state = getting;
    }
}

//ボタン入力に応じてChangeStateを呼ぶ
void Player::StateUpdate(InputManager& input){
    if(input.aButton == true){
        if(state == aiming){
            ChangeState(floating);
        }else if(state == floating){
            ChangeState(aiming);
        }
    }
}

void Player::PlayerMove(InputManager& input){
    StateUpdate(input);
    //Aimで狙っているところなら
    if(state == aiming){
        AimMove(input);
    }else if(state == floating){
        FloaterMove(input);
    }
}

void Player::CollideJudge(Array<Somen>& somenArray){
    if(state == floating){
        CatchCollideJudge(somenArray);
    }
}

void Player::CatchCollideJudge(Array<Somen>& somenArray){
    Circle catchCollider{pos, 50};
    for(auto &somen : somenArray){
        if(catchCollider.intersects(somen.ColliderGet())){
            //キャッチ成功
            Print << U"キャッチ成功";
            //ChangeState(catching);
        };
    }
}

void Player::Draw(){
    //Aimで狙っているところなら
    if(state == aiming){
        aimTex.drawAt(pos);
    }else if(state == floating){
        floaterTex.drawAt(pos);
    }else{
        aimTex.drawAt(pos);
        floaterTex.drawAt(pos);
    }
}

void Player::AimMove(InputManager& input){
    pos.x += aimVelo*input.lStickX*Scene::DeltaTime();
    pos.y += aimVelo*input.lStickY*Scene::DeltaTime();
}

void Player::FloaterMove(InputManager& input){
    //移動パワーの時間減少分。マイナスにならないように丸める。
    floaterGoingPos.y = Clamp((floaterGoingPos.y-floaterDeceleration*Scene::DeltaTime()), 0.0, 9999.9);
    //移動パワーを実際に前フレーム移動した分だけ減らす
    floaterGoingPos.y = Clamp(floaterGoingPos.y -(pos.y - posLastFrame.y), 0.0, 9999.9);
    if(input.rTrigger == true){
        floaterGoingPos.y += floaterAccellation;
    }
    floaterNowSpeed = {0,(pos.y-posLastFrame.y)/Scene::DeltaTime()};
    posLastFrame = pos;
    pos = Math::SmoothDamp(pos, (pos+floaterGoingPos),floaterNowSpeed,0.8);

    pos.x += floaterVelo*input.lStickX*Scene::DeltaTime();
}
