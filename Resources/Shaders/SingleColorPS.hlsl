#include "SingleColor.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	//色
	float4 shadecolor = color;

	//shadecolor.a = 1.0f;
	//出力
	return float4(shadecolor.r,shadecolor.g,shadecolor.b,1.0f);
}