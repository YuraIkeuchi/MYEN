#pragma once
#include "BaseScene.h"

//ゲームプレイシーン
class GamePlayScene : public BaseScene
{
private://メンバ変数

public:
	//初期化
	void Initiallize(DirectXCommon* dxCommon) override;
	//更新
	void Update(DirectXCommon* dxCommon) override;
	//描画
	void Draw(DirectXCommon* dxCommon) override;
	//開放
	void Finalize() override;
	void ModelDraw(DirectXCommon* dxCommon);
	void GameDraw(DirectXCommon* dxCommon);
	void ImGuiDraw();

	void ChangePostEffect(int PostType);
	
};