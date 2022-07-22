#include "Toon.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	//テクスチャマッピング
	float4 texcolor = tex.Sample(smp, input.uv);

	//シェーディングによる色
	float4 shadecolor;
	//光沢度
	const float shininess = 50.0f;
	//頂点から視点へのベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	//ハーフベクトル
	float3 halfvec = normalize(lightv + eyedir);
	//ライトに向かうベクトルと法線の内積
	float intensity = saturate(dot(normalize(input.normal), halfvec));
	//環境反射光
	float3 ambient = m_ambient;
	//smoothstep用変数
	float _Threshold = 0.45;
	//陰の部分
	float3 diffuse = m_diffuse * smoothstep(_Threshold, _Threshold + 0.1f, intensity);
	//光の部分
	float3 specular = m_specular * smoothstep(_Threshold, _Threshold + 0.1f, pow(intensity, shininess));
	//すべて加算
	float3 ads = (ambient + diffuse + specular) * texcolor.rgb;
	//暗い部分の色
	float3 dark_color = texcolor.rgb * 0.3;
	//明るい部分の色
	float3 light_color = ads;
	//陰と光の部分の判定
	shadecolor.rgb = smoothstep(_Threshold, _Threshold + 0.1f, intensity) * light_color + 
		(1 - smoothstep(_Threshold, _Threshold + 0.1f, intensity)) * dark_color * lightcolor;
	//a値設定
	shadecolor.a = color.a;
	//出力
	return shadecolor * color;
}