#include "Gaussian.hlsli"

Texture2D<float4> tex0 : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
	//�ڂ�����
	float totalWeight = 0, _Sigma = 0.005, _StepWidth = 0.001;
	float4 col = float4(0, 0, 0, 0);

	for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth) //xy��2a�̕��ŐF���擾
	{
		for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
		{
            float2 pickUV = input.uv + float2(px, py);
            float weight = Gaussian(input.uv, pickUV, _Sigma);
			col += tex0.Sample(smp, pickUV) * weight;//Gaussian�Ŏ擾�����d�݂�F�ɂ�����

			totalWeight += weight;//�������d�݂̍��v�l�����߂�
		}
	}

	col.rgb = col.rgb / totalWeight; //�������d�ݕ�����
	return col;
}