#include "Framework.h"
#include "FbxLoader.h"
void Framework::Run()
{
	Initialize(dxcommon);
	while (true) {
		if (FPSManager::GetInstance()->Run()) {

			//毎フレーム更新
			Update(dxcommon);

			//終了リクエストが来たら抜ける
			if (endRequst) {
				break;
			}

			//描画
			Draw(dxcommon);
		}
	}
	//解放処理
	Finalize();

}
void Framework::Initialize(DirectXCommon* dxCommon)
{
	winApp = new WinApp();
	winApp->Initialize();
	dxcommon = new DirectXCommon();
	dxcommon->Initialize(winApp);
	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize(winApp);

	audio = Audio::GetInstance();
	audio->Initialize();
	//FPS固定
	fpsManager = FPSManager::GetInstance();
	fpsManager->Init();
	// nullptrチェック
	assert(dxcommon);
	assert(input);
	assert(audio);
	const int debugTextTexNumber = 0;
	Sprite::StaticInitialize(dxcommon->GetDev(), dxcommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/2d/debugfont.png")) {
		assert(0);
		return;
	}
	// ライト静的初期化
	LightGroup::StaticInitialize(dxcommon->GetDev());
	// デバッグテキスト初期化
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextTexNumber);

	Object3d::StaticInitialize(dxcommon->GetDev(), dxcommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	Texture::StaticInitialize(dxcommon->GetDev(), dxcommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	// FBX関連静的初期化
	FbxLoader::GetInstance()->Initialize(dxcommon->GetDev());
	// パーティクルマネージャ初期化
	ParticleManager::GetInstance()->Initialize(dxcommon->GetDev());
	ModelManager::GetInstance()->Initialize();
	ImageManager::GetIns()->LoadTex2D();
}

void Framework::Finalize()
{

	delete dxcommon;
	winApp->Finalize();
	delete winApp;
}

void Framework::Update(DirectXCommon* dxCommon)
{
	//ウィンドウメッセージ処理
	if (winApp->ProcessMessage()) {
		//ゲームループ終了
		endRequst = true;
		return;
	}

	input->Update();

	//シーン更新処理
	SceneManager::GetInstance()->Update(dxCommon);
}

void Framework::Draw(DirectXCommon* dxCommon)
{
	
	//シーン描画
	SceneManager::GetInstance()->Draw(dxCommon);
	//でバックテキストの描画
	//debugText->DrawAll();
}