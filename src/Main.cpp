# include <Siv3D.hpp> // Siv3D v0.6.15
# include <CoTaskLib.hpp>
# include "Somen.h"
# include "Generater.h"
# include "Drawer.h"
# include "InputManager.h"
# include "Player.h"

Co::Task<> DebugWaitAndSituationSet(Player& player){
    
    //System::Sleep(10000);
    co_await Co::Delay(0.01s);
    player.pos = {400,500};
    player.ChangeState(player.floating);

}


void Main()
{
    // ウィンドウを閉じる操作のみを終了操作に設定する。
    System::SetTerminationTriggers(UserAction::CloseButtonClicked);
    //ウィンドウのリサイズ
    //Window::Resize(1440,810);
    Window::Resize(1920*3/4,1080*3/4);
    //背景
    const Texture background{U"../images/backgroundLessWater.PNG"};
    
    const VertexShader vs
            = HLSL{ U"example/shader/hlsl/wave.hlsl" }
            | GLSL{ U"example/shader/glsl/wave.vert", { { U"VSConstants2D", 0 } } };
    if (not vs)
    {
        throw Error{ U"Failed to load a shader file" };
    }
    
    ConstantBuffer<float> cb;
    
    bool showWireframe = false;
    
    //麺のテクスチャを入れる
    const Texture noodle{ U"../images/Somen.PNG" };
    const Texture face{ U"😑"_emoji };
    const Texture aim{ U"../images/redAim.PNG"};
    const Texture floater{ U"../images/redFloater.PNG" };
    const Texture swimmer{ U"🏊"_emoji };
    const Texture drowner{ U"😫"_emoji };
    
    //フォントの作成
    Font normalFont{FontMethod::MSDF, 48};

    
    //そうめんの生成
    Generater gene;
    Somen debugSomen{1000,0,200};

    //そうめんのDrawを行う
    Drawer drawer(noodle);

    //コントローラ入力を受け付ける
    InputManager p1Input(0);
    InputManager p2Input(1);

    //プレイヤークラスの生成
    Player p1(0, aim, floater,swimmer,drowner);
    Player p2(1, aim, floater,swimmer,drowner);

    //非同期タスクの作成
    Co::Init();
    //デバッグ用の初期化関数
    //DebugTask = Async(DebugWaitAndSituationSet, std::ref(p2));
    const auto _ = DebugWaitAndSituationSet(p2).runScoped();

    //detail::XInput_impl controller = XInput(0);
    //std::cout << typeid(controller).name() << "ああああああああ" << std::endl;
    
    while (System::Update()){
        background.scaled(0.75).draw(0,0);
        cb = static_cast<float>(Scene::Time());

        {
            Graphics2D::SetVSConstantBuffer(1, cb);
            const ScopedRenderStates2D states{ showWireframe ? RasterizerState::WireframeCullNone : RasterizerState::Default2D };
            const ScopedCustomShader2D shader{ vs };

            // 頂点情報の無い三角形を 2 * 160 個描画する
            // （頂点情報は頂点シェーダで設定する）
            Graphics2D::DrawTriangles(2 * Scene::Width()/8);
        }
        gene.Generate();
        
        for (Somen& somen : gene.somenArray){
            somen.VelocitySet(100, 0);
            somen.Flow();
        }

        p1Input.InputUpdate();
        p2Input.InputUpdate();
        face.drawAt(400+400*p1Input.lStickX,250+250*p1Input.lStickY);
        
        //ClearPrint();
        
        p1.PlayerMove(p1Input,p2Input, gene.somenArray, p2);
        p2.PlayerMove(p2Input,p1Input, gene.somenArray, p1);

        
        p1.CollideJudge(gene.somenArray, p2);
        p2.CollideJudge(gene.somenArray, p1);


        drawer.DrawSomen(gene.somenArray);
        p1.Draw();
        p2.Draw();
        
        Print << U"mousePos{}"_fmt(Cursor::Pos());
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
