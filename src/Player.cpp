#include "Player.h"

Player::Player(uint8 playerIndex, Texture aimTexture, Texture floaterTexture,Texture swimmerTexture,Texture drownerTex){
    m_playerIndex = playerIndex;
    aimTex = aimTexture;
    floaterTex = floaterTexture;
    swimmerTex = swimmerTexture;

    if(m_playerIndex == 0){
        m_spawnPoint = Vec2{400,600};
    }else{
        m_spawnPoint = Vec2{400,100};
    }
    pos = m_spawnPoint;
    score = 0;
    //state = aiming;
    ChangeState(aiming);
    Print << U"playerコンストラクト";
}

void Player::Respawn(){
    pos = m_spawnPoint;
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
    }else if(nextState == robbing){
        state = robbing;
        Print << U"Playerがrobbing状態";
    }else if(nextState == robbed){
        state = robbed;
        robbedPower = 0;
        robbingPower = 0;
        Print << U"Playerがrobbed状態";
    }else if(nextState == getting){
        state = getting;
    }else if(nextState == swimming){
        state = swimming;
        stamina = staminaMax;
        riverFlowSpeed = 100;
    }else if(nextState == drowning){
        state = drowning;
    }
    std::cout << U"ChangeState,p{}が{}にチェンジ"_fmt(m_playerIndex,nextState) << std::endl;
}

//ボタン入力に応じてChangeStateを呼ぶ
void Player::StateUpdate(InputManager& input, Array<Somen>& somenArray){
    //aボタンでaimingとfloating切り替え
    if(input.aButtonDown == true){
        if(state == aiming){
            ChangeState(floating);
        }else if(state == floating){
            //ChangeState(aiming);
        }
    }

    //一定自陣に近づいたらゲット
    if(state == catching){
        if(m_playerIndex == 0){
            if(m_spawnPoint.y - getAreaHeight <= pos.y){
                GetSomen(somenArray);
            }
        }else if(m_playerIndex == 1){
            if(m_spawnPoint.y + getAreaHeight >= pos.y){
                GetSomen(somenArray);
            }
        }
    }

    //スタミナ切れ
    if(state == swimming){
        if(stamina <= 0){
            ChangeState(drowning);
        }
    }
    
    //泳がずに下流に流されたらリスポーン
    if(state == swimming){
        if (pos.x > Scene::Width() + outOfWindowBuffer * 3){
            Respawn();
        }
    }
    
    //溺れて画面外に行ったらリスポーン
    if(state == drowning){
        if(pos.x > Scene::Width() + outOfWindowBuffer || pos.x < (0 - outOfWindowBuffer)){
            Respawn();
        }
    }

    Print << U"p{},state{}"_fmt(m_playerIndex,state);
}

void Player::PlayerMove(InputManager& input,InputManager& opponentPlayerInput, Array<Somen>& somenArray,Player& opponentPlayer){
    StateUpdate(input, somenArray);
    //Aimで狙っているところなら
    if(state == aiming){
        AimMove(input);
    }else if(state == floating){
        FloaterMove(input);
    }else if(state == catching){
        CatcherMove(input, somenArray);
    }else if(state == robbed){
        RobbedMove(input,opponentPlayerInput,somenArray,opponentPlayer);
    }else if(state == swimming){
        SwimMove(input);
    }else if(state == drowning){
        DrownMove();
    }
    //Print << U"p{},({},{})"_fmt(m_playerIndex,pos.x,pos.y);
}

void Player::CollideJudge(Array<Somen>& somenArray, Player& opponent){
    if(state == floating){
        CatchCollideJudge(somenArray, opponent);
    }
    if(state == swimming){
        EatCollideJudge(somenArray);
    }
}

void Player::CatchCollideJudge(Array<Somen>& somenArray, Player& opponent){
    Circle catchCollider{pos, catchColliderRadius};
    for(auto &somen : somenArray){
        if(catchCollider.intersects(somen.ColliderGet())){
            if(RobCollideJudge(somen, opponent) == false){
                if(somen.GetState() != somen.caught & somen.GetState() != somen.robbed){
                    //キャッチ成功
                    Print << U"キャッチ成功";
                    CatchSomen(somen,somenArray);
                }
                return;
            }else{
                //rob成功
                return;
            };
            
        }
    }
}

bool Player::RobCollideJudge(Somen& somen, Player& opponent){
    Circle robCollider{pos, robColliderRadius};
    if(somen.GetState() == somen.caught){
        //所定のそうめんがcaught中だったら
        if(robCollider.intersects(somen.ColliderGet())){
            //Print << U"a";
            RobSomen(somen, opponent);
            return true;
        }else{
            //Print << U"b";
            return false;
        }
    }
    else{
        //Print << U"c";
        //robできるほど近くなかったら
        return false;
    }
}

void Player::EatCollideJudge(Array<Somen>& somenArray){
    Circle eatCollider{pos,eatColliderRadius};
    for(auto &somen : somenArray){
        if(eatCollider.intersects(somen.ColliderGet())){
            somen.ChangeState(somen.gotten);
            score += somenPoint;
            catchingSomenIndex = somen.index;
            somenArray.remove_if([this](Somen somenInArray){return (somenInArray.index == catchingSomenIndex);});
            stamina += eatStaminaRecover;
            return;
        }
    }
}

Somen& Player::FindSomen(uint32 somenIndex, Array<Somen>& somenArray){
    for(auto &somen : somenArray){
        if(somen.index == somenIndex){
            //見つけたら終了
            return somen;
        }
    }
    //見つからなければ失敗
    std::cout << U"FindSomen失敗,state{},somenIndex{}"_fmt(state,somenIndex)<< std::endl;
    //とりあえず
    return somenArray[0];
}

void Player::CatchSomen(Somen& somen,Array<Somen>& somenArray){
    ChangeState(catching);
    Somen& somenHere = FindSomen(catchingSomenIndex, somenArray);
    somenHere.ChangeState(somenHere.caught);
    catchingSomenIndex = somenHere.index;
}

void Player::RobSomen(Somen& somen, Player& opponent){
    Print << U"RobSomen内";
    ChangeState(robbing);
    somen.ChangeState(somen.robbed);
    catchingSomenIndex = somen.index;
    opponent.ChangeState(robbed);
}

void Player::GetSomen(Array<Somen>& somenArray){
    //Print << U"score{}"_fmt(score);
    ChangeState(getting);
    Somen& somen = FindSomen(catchingSomenIndex, somenArray);
    somen.ChangeState(somen.gotten);
    score += somenPoint;
    somenArray.remove_if([this](Somen somenInArray){return (somenInArray.index == catchingSomenIndex);});
    //Print << U"score{}"_fmt(score);
    pos = m_spawnPoint;
    ChangeState(aiming);
}

//メンバ変数のcatchingSomenIndexと同じindexならtrueを返す
bool Player::SomenArrayIndexCheck(Somen& somen){
    if(somen.index == catchingSomenIndex){
        return true;
    }else{
        return false;
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
    floaterGoingPos.y = Clamp(floaterGoingPos.y -(pos.y - posLastFrame.y)*PlayerIndexIsZeroThenOne(), 0.0, 9999.9);
    if(input.rTriggerDown == true){
        floaterGoingPos.y += floaterAccellation;
    }
    floaterNowSpeed = {0,(pos.y-posLastFrame.y)/Scene::DeltaTime()};
    posLastFrame = pos;
    pos = Math::SmoothDamp(pos, (pos+floaterGoingPos*PlayerIndexIsZeroThenOne()),floaterNowSpeed,0.8);

    pos.x += floaterVelo*input.lStickX*Scene::DeltaTime();
}

void Player::CatcherMove(InputManager& input, Array<Somen>& somenArray){
    //手抜きでコードの使い回し。速度変更が必要ならその時に。
    FloaterMove(input);
    Somen& somen = FindSomen(catchingSomenIndex, somenArray);
    somen.somenPos = pos;
    //Print << U"CatcherMove";
}

void Player::RobbedMove(InputManager& input,InputManager& opponentPlayerInput, Array<Somen>& somenArray,Player& opponentPlayer){
    //そうめんを両プレイヤーの真ん中に置く(揺れも持たせる)
    Somen& somen = FindSomen(catchingSomenIndex, somenArray);
    std::cout << U"RobbedMove,somenIndex{},catchingSomenIndex{}"_fmt(somen.index,catchingSomenIndex) << std::endl;
    somen.somenPos = (pos + opponentPlayer.pos)/2 + (pos - opponentPlayer.pos)*Periodic::Sine1_1(0.25)*0.4;

    //ゲージの出現？
    //連打量の比較
    if(input.rTriggerDown == true){
        robbedPower += robbedPwrIncrease;
    }
    if(opponentPlayerInput.rTriggerDown == true){
        robbingPower += robbingPwrIncrease;
    }
    robbedRobbingPowerRatio = (robbingPower-robbedPower-robbedSideWinBorder)/(robbingSideWinBorder-robbedSideWinBorder);

    if(robbingPower - robbedPower > robbingSideWinBorder){
        //略奪側が勝利
        //自分はswimmingに
        ChangeState(swimming);
        //相手はcatcherに
        opponentPlayer.CatchSomen(somen,somenArray);
        Print << U"rob成功";
    }else if (robbingPower - robbedPower < robbedSideWinBorder){
        //被略奪側(自分)が勝利
        //再びcatcherにする
        CatchSomen(somen,somenArray);
        //相手はswimmingに
        opponentPlayer.ChangeState(opponentPlayer.swimming);
        Print << U"rob失敗";
    }
}

void Player::SwimMove(InputManager& input){
    if(input.rTriggerDown == true){
        pos.x += dashStrength*input.lStickX;
        pos.y += dashStrength*input.lStickY*0.3;
        stamina -= dashStaminaDecrease;
    }
    pos.x += riverFlowSpeed*Scene::DeltaTime();
    stamina -= naturalStaminaDecrease*Scene::DeltaTime();

    if(stamina <= 0){
        stamina = 0;
    }
}

void Player::DrownMove(){
    pos.x += riverFlowSpeed*Scene::DeltaTime();
}


void Player::Draw(){
    //Aimで狙っているところなら
    if(state == aiming){
        aimTex.scaled(0.1).drawAt(pos);
    }else if(state == floating || state == catching){
        floaterTex.scaled(0.1).drawAt(pos);
    }else if(state == robbed){
        floaterTex.scaled(0.1).drawAt(pos);
        if(m_playerIndex == 0){
            DrawRatioGaugeHorizontal(Vec2{400,100},400,50,robbedRobbingPowerRatio,{1,0,0},{0,0,1});
        }else if(m_playerIndex == 1){
            DrawRatioGaugeHorizontal(Vec2{400,100},400,50,robbedRobbingPowerRatio,{0,0,1},{1,0,0});
        }
    }else if(state == swimming){
        swimmerTex.drawAt(pos);
        DrawGaugeHorizontal((pos + Vec2{0,30}),200,25,(stamina/staminaMax),{0,1,0});
    }else if(state == drowning){
        drownerTex.drawAt(pos);
    }else{
        aimTex.scaled(0.1).drawAt(pos);
        floaterTex.scaled(0.1).drawAt(pos);
    }
    //Print << U"player{}のdraw"_fmt(m_playerIndex);
}

void Player::DrawRatioGaugeHorizontal(Vec2 centerPos,double width, double height, double leftSideRatio, ColorF rightColor, ColorF leftColor){
    RectF{Arg::center(centerPos),width,height}.draw(rightColor);
    RectF{(centerPos - Vec2{width/2,height/2}),width*leftSideRatio,height}.draw(leftColor);
}

void Player::DrawGaugeHorizontal(Vec2 centerPos,double width, double height, double ratio, ColorF color){
    RectF{Arg::center(centerPos), width*ratio,height}.draw(color);
}

int8 Player::PlayerIndexIsZeroThenOne(){
    if(m_playerIndex == 0){
        return 1;
    }else{
        return -1;
    }
}
