#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	
	//コンストラクタ
	PostEffect();

	//初期化
	void Initialize();

	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

private://メンバ変数
	//テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff;
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap>descHeapSRV;
};