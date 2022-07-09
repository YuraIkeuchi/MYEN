#include "Toon.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	// �e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp, input.uv);

	// ����x
	const float shininess = 4.0f;
	// ���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	// �����ˌ�
	float3 ambient = m_ambient;

	// �V�F�[�f�B���O�ɂ��F
	float4 shadecolor = float4(ambientColor * ambient, m_alpha);

	for (int i = 0; i < DIRLIGHT_NUM; i++) {
		if (dirLights[i].active) {
			// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
			float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
			// ���ˌ��x�N�g��
			float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			// �g�U���ˌ�
			float3 diffuse = dotlightnormal * m_diffuse;
			// ���ʔ��ˌ�
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//���ׂĉ��Z�����F
			float3 color = (ambient + diffuse + specular) * dirLights[i].lightcolor;
			// �S�ĉ��Z����
			shadecolor.rgb += color;
		}
	}

	//�g�D�[���V�F�[�_�̂��߂̏���
	float4 brightColor = float4(0.7, 0.7, 0.7, 1.0);							//���邢�ӏ��̐F
	float4 bright = brightColor * smoothstep(0.3, 0.4, shadecolor.r);		//���邢�ӏ��̔���
	float4 darkColor = float4(0.4, 0.4, 0.4, 1.0);							//�Â��ӏ��̐F
	float4 dark = darkColor * (1 - smoothstep(0.3, 0.4, shadecolor.r));		//�Â��ӏ��̔���
	float4 reflectionColor = float4(1.0, 1.0, 1.0, 1.0);					//���ˌ��̉ӏ��̐F
	float4 reflect = reflectionColor * smoothstep(0.98, 1.0, shadecolor.r);	//���ˌ��̔���

	float4 col = bright + dark + reflect;//�F�𑫂�

	// �V�F�[�f�B���O�ɂ��F�ŕ`��
	return col * texcolor;
	
}