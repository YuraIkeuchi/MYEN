#include "Toon.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	// テクスチャマッピング
	float4 texcolor = tex.Sample(smp, input.uv);

	// 光沢度
	const float shininess = 4.0f;
	// 頂点から視点への方向ベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	// 環境反射光
	float3 ambient = m_ambient;

	// シェーディングによる色
	float4 shadecolor = float4(ambientColor * ambient, m_alpha);

	for (int i = 0; i < DIRLIGHT_NUM; i++) {
		if (dirLights[i].active) {
			// ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
			// 反射光ベクトル
			float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			// 拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;
			// 鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//すべて加算した色
			float3 color = (ambient + diffuse + specular) * dirLights[i].lightcolor;
			// 全て加算する
			shadecolor.rgb += color;
		}
	}

	//トゥーンシェーダのための処理
	float4 brightColor = float4(0.7, 0.7, 0.7, 1.0);							//明るい箇所の色
	float4 bright = brightColor * smoothstep(0.3, 0.4, shadecolor.r);		//明るい箇所の判定
	float4 darkColor = float4(0.4, 0.4, 0.4, 1.0);							//暗い箇所の色
	float4 dark = darkColor * (1 - smoothstep(0.3, 0.4, shadecolor.r));		//暗い箇所の判定
	float4 reflectionColor = float4(1.0, 1.0, 1.0, 1.0);					//反射光の箇所の色
	float4 reflect = reflectionColor * smoothstep(0.98, 1.0, shadecolor.r);	//反射光の判定

	float4 col = bright + dark + reflect;//色を足す

	// シェーディングによる色で描画
	return col * texcolor;
	
}