#include "SingleColor.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	//�F
	float4 shadecolor = color;

	//shadecolor.a = 1.0f;
	//�o��
	return float4(shadecolor.r,shadecolor.g,shadecolor.b,1.0f);
}