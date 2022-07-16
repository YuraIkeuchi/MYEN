#include "Toon.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	//�e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp, input.uv);

	//�V�F�[�f�B���O�ɂ��F
	float4 shadecolor;
	//����x
	const float shininess = 30.0;
	//���_���王�_�ւ̃x�N�g��
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	//�n�[�t�x�N�g��
	float3 halfvec = normalize(lightv + eyedir);
	//���C�g�Ɍ������x�N�g���Ɩ@���̓���
	float intensity = saturate(dot(normalize(input.normal), halfvec));
	//�����ˌ�
	float3 ambient = m_ambient;
	//smoothstep�p�ϐ�
	float a_scale = 0.45;
	float b_scale = a_scale + 0.1;
	//�g�U���ˌ�
	float3 diffuse = m_diffuse * smoothstep(a_scale, b_scale, intensity);
	//���ʔ��ˌ�
	float3 specular = m_specular * smoothstep(a_scale, b_scale, pow(intensity, shininess));
	//���ׂĉ��Z
	float3 ads = (ambient + diffuse + specular) * texcolor.rgb;
	//���邢�����̐F
	float3 l_color = ads;
	//�Â������̐F
	float3 d_color = texcolor.rgb * 0.3;
	//�g�D�[����
	shadecolor.rgb = smoothstep(a_scale, b_scale, intensity) * l_color + (1 - smoothstep(a_scale, b_scale, intensity)) * d_color * lightcolor;
	shadecolor.a = m_alpha;

	//�o��
	return shadecolor;
}