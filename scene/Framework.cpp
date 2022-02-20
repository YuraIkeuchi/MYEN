#include "Framework.h"
//#include "FbxLoader.h"
void Framework::Run() 
{
	Initialize();
	while (true) {
		//更新処理
		Update();
		//終了リクエストが来たら抜ける
		if (endRequst) {
			break;
		}
		//描画処理
		Draw();
	}
	//解放処理
	Finalize();

}
void Framework::Initialize()
{
	winApp = new WinApp();
	winApp->Initialize();
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);
	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize(winApp);

	audio = Audio::GetInstance();
	audio->Initialize();
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);
	const int debugTextTexNumber = 0;
	Sprite::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	// ライト静的初期化
	LightGroup::StaticInitialize(dxCommon->GetDev());
	// デバッグテキスト初期化
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextTexNumber);

	Object3d::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	// FBX関連静的初期化
	//FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());

}

void Framework::Finalize()
{

	delete dxCommon;
	winApp->Finalize();
	delete winApp;
}

void Framework::Update()
{
	//ウィンドウメッセージ処理
	if (winApp->ProcessMessage()) {
		//ゲームループ終了
		endRequst = true;
		return;
	}

	input->Update();
	
	//シーン更新処理
	SceneManager::GetInstance()->Update();
}

void Framework::Draw()
{
	dxCommon->PreDraw();
	//シーン描画
	SceneManager::GetInstance()->Draw();
	//でバックテキストの描画
	debugText->DrawAll();

	dxCommon->PostDraw();

}