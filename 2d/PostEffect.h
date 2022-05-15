#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:

	//コンストラクタ
	PostEffect();
	/// <summary>
  /// パイプライン生成
  /// </summary>
  /// <param name="cmdList"></param>
	void CreateGraphicsPipeline();
	//初期化
	void Initialize();

	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// シーン描画前処理
	/// </summary>
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);
private://静的メンバ変数
	static const float clearColor[4];
private://メンバ変数
	//テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff[2];
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap>descHeapSRV;
	//深度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//グラフィックスパイプライン
	ComPtr<ID3D12PipelineState>pipelineState;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature>rootSignature;
};