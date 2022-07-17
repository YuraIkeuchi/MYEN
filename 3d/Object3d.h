#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "Model.h"
#include "Camera.h"
#include "LightGroup.h"

#include "CollisionInfo.h"

class BaseCollider;

/// 3Dオブジェクト
class Object3d
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス	

	// パイプラインセット
	struct PipelineSet
	{
		// ルートシグネチャ
		ComPtr<ID3D12RootSignature> rootsignature;
		// パイプラインステートオブジェクト
		ComPtr<ID3D12PipelineState> pipelinestate;
	};

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		XMFLOAT4 color;//色情報
		XMMATRIX viewproj;    // ビュープロジェクション行列
		XMMATRIX world; // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）
	};

private: // 定数


public: //静的メンバ関数
	
	//静的初期化
	static bool StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height, Camera* camera = nullptr);

	
	//グラフィックパイプラインの生成
	void CreateGraphicsPipeline(const wchar_t* vsShaderName, const wchar_t* psShaderName);

	
	//描画前処理
	static void PreDraw();
	//描画後処理
	static void PostDraw();
	//3Dオブジェクト生成
	static Object3d* Create();

	static void SetCamera(Camera* camera) {
		Object3d::camera = camera;
	}

	static void SetLightGroup(LightGroup* lightGroup) {
		Object3d::lightGroup = lightGroup;
	}
	//ベクトルによる移動
	static void CameraMoveVector(XMFLOAT3 move);

	
	// ベクトルによる視点移動
	static void CameraMoveEyeVector(XMFLOAT3 move);

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	
	// ビュー行列
	static XMMATRIX matView;
	// 射影行列
	static XMMATRIX matProjection;
	// 視点座標
	static XMFLOAT3 eye;
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;
	// ビルボード行列
	static XMMATRIX matBillboard;
	// Y軸回りビルボード行列
	static XMMATRIX matBillboardY;
	// カメラ
	static Camera* camera;
	// ライト
	static LightGroup* lightGroup;


private:// 静的メンバ関数	
	//カメラ初期化
	static void InitializeCamera(int window_width, int window_height);

	
	//ビュー行列を更新
	static void UpdateViewMatrix();

public: // メンバ関数

	Object3d() = default;

	virtual ~Object3d();

	virtual	bool Initialize();
	
	//毎フレーム処理
	virtual void Update();
	// 描画
	virtual void Draw();
	//行列の更新
	void UpdateWorldMatrix();


	//座標の取得
	const XMFLOAT3& GetPosition() { return position; }

	//回転の取得
	const XMFLOAT3& GetRotation() { return rotation; }

	const XMMATRIX& GetMatWorld() { return matWorld; }

	/// <summary>
/// モデルを取得
/// </summary>
	inline Model* GetModel() { return model; }

	//座標の設定
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	//スケールの設定
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	//色の設定
	void SetColor(XMFLOAT4 color) { this->color = color; }
	//モデルのセット
	void SetModel(Model* model) { this->model = model; }

	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }

	//当たり判定セット
	void SetCollider(BaseCollider* collider);

	//コールバック
	virtual void OnCollision(const CollisionInfo& info) {}

protected: // メンバ変数
	// テクスチャあり用パイプライン
	PipelineSet pipelineSet;
	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	// 色
	XMFLOAT4 color = { 1,0,0,1 };
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// 親オブジェクト
	Object3d* parent = nullptr;
	// モデル
	Model* model = nullptr;
	// ビルボード
	bool isBillboard = false;
	//クラス名
	const char* name = nullptr;
	//コライダー
	BaseCollider* collider = nullptr;

};

