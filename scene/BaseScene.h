#pragma once

//前方宣言
class SceneManager;
//シーンインターフェース
class BaseScene
{
public:
	//仮想デストラクタ
	virtual ~BaseScene() = default;

	//初期化
	virtual void Initiallize() = 0;
	//開放
	virtual void Finalize() = 0;
	//更新
	virtual void Update() = 0;
	//描画
	virtual void Draw() = 0;
};