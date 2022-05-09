#include"PostEffectTest.hlsli"
Texture2D<float4>tex:register(t0);

SamplerState smp:register(s0);
float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp,input.uv);
    //default
   // return float4(texcolor.rgb, 1);
    //bokasi
    // 2.5テクセル分ずらすためのUV値を求める
    float offsetU = 3.5f / 1280.0f;
    float offsetV = 3.5f / 720.0f;

    // 基準テクセルから右のテクセルのカラーをサンプリングする
    texcolor += tex.Sample(smp, input.uv + float2(offsetU, 0.0f));

    // 基準テクセルから左のテクセルのカラーをサンプリングする
    texcolor += tex.Sample(smp, input.uv + float2(-offsetU, 0.0f));

    // 基準テクセルから下のテクセルのカラーをサンプリングする
    texcolor += tex.Sample(smp, input.uv + float2(0.0f, offsetV));

    // 基準テクセルから上のテクセルのカラーをサンプリングする
    texcolor += tex.Sample(smp, input.uv + float2(0.0f, -offsetV));

    // 基準テクセルから右下のテクセルのカラーをサンプリングする
    texcolor += tex.Sample(smp, input.uv + float2(offsetU, offsetV));

    // 基準テクセルから右上のテクセルのカラーをサンプリングする
    texcolor += tex.Sample(smp, input.uv + float2(offsetU, -offsetV));

    // 基準テクセルから左下のテクセルのカラーをサンプリングする
    texcolor += tex.Sample(smp, input.uv + float2(-offsetU, offsetV));

    // 基準テクセルから左上のテクセルのカラーをサンプリングする
    texcolor += tex.Sample(smp, input.uv + float2(-offsetU, -offsetV));

    // 基準テクセルと近傍8テクセルの平均なので9で除算する
    texcolor /= 9.0f;

    return float4(texcolor.rgb,1);
   // //return float4(texcolor.rgb, 1);

    //return float4(1.0f - texcolor.rgb, 1);
    //色暗くなるやつ
    //return float4(1-texcolor.rgb, 1);
    //色明るくなるやつ 目が終わる
    // return float4(texcolor.rgb*2, 1);
    //return tex.Sample(smp,input.uv) * color;
//return float4(1.0f, 1.0f, 1.0f, 1.0f);
}