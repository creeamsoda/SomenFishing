# include <Siv3D.hpp> // Siv3D v0.6.15
# include "Somen.h"
# include "Generater.h"
# include "Drawer.h"
# include "InputManager.h"
# include "Player.h"

void Main()
{
    // ウィンドウを閉じる操作のみを終了操作に設定する。
    System::SetTerminationTriggers(UserAction::CloseButtonClicked);
    
    //麺のテクスチャを入れる
    const Texture noodle{ U"🍜"_emoji };
    const Texture face{ U"😑"_emoji };
    const Texture aim{ U"❌"_emoji };
    const Texture floater{ U"🛟"_emoji };
    
    //フォントの作成
    Font normalFont{FontMethod::MSDF, 48};

    //麺の生成（テスト）
    Somen somen1(500,300);
    somen1.VelocitySet(2,0);
    
    //そうめんの生成
    Generater gene;

    //そうめんのDrawを行う
    Drawer drawer(noodle);

    //コントローラ入力を受け付ける
    InputManager p1Input(0);

    //プレイヤークラスの生成
    Player p1(0, aim, floater);

    //detail::XInput_impl controller = XInput(0);
    //std::cout << typeid(controller).name() << "ああああああああ" << std::endl;
    
    while (System::Update()){
        somen1.Flow();
        somen1.Draw(noodle);

        gene.Generate();
        
        for (Somen& somen : gene.somenArray){
            somen.VelocitySet(100, 0);
            somen.Flow();
        }

        p1Input.InputUpdate();
        face.drawAt(400+400*p1Input.lStickX,250+250*p1Input.lStickY);
        
        ClearPrint();
        
        p1.PlayerMove(p1Input);

        
        p1.CollideJudge(gene.somenArray);

        drawer.DrawSomen(gene.somenArray);
        p1.Draw();
    }
}



/*
# include <Siv3D.hpp>

void Main()
{
    Window::Resize(800, 800);

    const Array<String> indices = Range(0, (Gamepad.MaxPlayerCount - 1)).map(Format);

    // ゲームパッドのプレイヤーインデックス
    size_t playerIndex = 0;

    while (System::Update())
    {
        ClearPrint();

        if (const auto gamepad = Gamepad(playerIndex)) // 接続されていたら
        {
            const auto& info = gamepad.getInfo();

            Print << U"{} (VID: {}, PID: {})"_fmt(info.name, info.vendorID, info.productID);

            for (auto [i, button] : Indexed(gamepad.buttons))
            {
                Print << U"button{}: {}"_fmt(i, button.pressed());
            }

            for (auto [i, axe] : Indexed(gamepad.axes))
            {
                Print << U"axe{}: {}"_fmt(i, axe);
            }

            Print << U"POV: " << gamepad.povD8();
        }

        SimpleGUI::RadioButtons(playerIndex, indices, Vec2{ 500, 20 });
    }
}
*/
