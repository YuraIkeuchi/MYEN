#include "Gaussian.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * sigma * sigma));
}

float4 GaussianBlur(float2 uv, float sigma, float stepWidth) {
	//ぼかし↓
	float totalWeight = 0, _Sigma = 0.005, _StepWidth = 0.001;
	float4 col = float4(0, 0, 0, 0);

	for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
	{
		for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
		{
			float2 pickUV = uv + float2(px, py);
			float weight = Gaussian(uv, pickUV, _Sigma);
			col += tex0.Sample(smp, pickUV) * weight;

			totalWeight += weight;
		}
	}

	col.rgb = col.rgb / totalWeight;
	return col;
}
float4 main(VSOutput input) : SV_TARGET
{
	float _Depth = 0.05f;
	float _Width = 0.05f;

	float4 depth = tex0.Sample(smp, input.uv);
	float4 pintColor = tex0.Sample(smp, input.uv);
	float4 overColor = GaussianBlur(input.uv, 0.03f, 0.005f);
	//float4 pintcolor = float4(1, 0, 0, 1);
	//float4 overcolor = float4(0, 0, 1, 1);
	float pint = smoothstep(0, _Width / 2, abs(depth.r - _Depth));

	return pint * overColor + (1 - pint) * pintColor;
	//return (1 - pint) * pintColor + pint * overColor;
}