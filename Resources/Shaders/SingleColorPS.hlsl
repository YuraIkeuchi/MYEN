#include "SingleColor.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	//色
	float4 shadecolor = color;

	//shadecolor.a = 1.0f;
	//出力
	return shadecolor;
	
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
	//return color;

	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
}