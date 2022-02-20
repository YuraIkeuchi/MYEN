#pragma once
#include "Sprite.h"
#include "Object3d.h"
#include "BaseScene.h"
/// タイトルシーン
class TitleScene : public BaseScene{
public:
	/// 初期化
	void Initiallize() override;
	/// 終了
	void Finalize() override;
	/// 毎フレーム更新
	void Update() override;
	/// 描画
	void Draw() override;

private://メンバ変数
	Sprite* sprite = { nullptr };
};

