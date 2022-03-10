#pragma once
#include <wrl.h>
#include <windows.h>
#define DIRECTINPUT_VERSION 0x0800//DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include "WinApp.h"
class Input
{
public:
	//�Q�[���p�b�h�i�{�^���j
	enum ButtonKind
	{
		Button_A,
		Button_B,
		Button_X,
		Button_Y,
		Button_LB,
		Button_RB,
		Select,
		Start,
		Button_LeftStick,
		Button_RightStick,
		Cross_Up,
		Cross_Down,
		Cross_Right,
		Cross_Left,
		ButtonMax
	};

	//�X�e�B�b�N
	enum StickKind
	{
		Up, Down, Right, Left
	};

	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};
	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	static Input* GetInstance();
	//������
	void Initialize(WinApp* winApp);
	//�X�V
	//�L�[�{�[�h
	void Update();
	bool PushKey(BYTE keyNumber);
	bool TriggerKey(BYTE keyNumber);
	//�}�E�X
	bool PushMouseLeft();
	bool PushMouseMiddle();
	bool TriggerMouseLeft();
	bool TriggerMouseMiddle();
	MouseMove GetMouseMove();
	//�Q�[���p�b�h
	bool LeftTiltStick(int stick);
	bool LeftTriggerStick(int stick);
	bool RightTiltStick(int stick);
	bool RightTriggerStick(int stick);
	bool PushButton(int Button);
	bool TriggerButton(int Button);
	bool PushCrossKey(int CrossKey);
	bool TriggerCrossKey(int CrossKey);
private://�����o�ϐ�
	//�L�[�{�[�h�̃f�p�C�X
	ComPtr<IDirectInputDevice8> devkeyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};
	//�Q�[���p�b�h�f�o�C�X
	ComPtr<IDirectInputDevice8> devGamePad;
	DIJOYSTATE gamePadState = {};
	DIJOYSTATE oldGamePadState = {};
	DIJOYSTATE gamePadState2 = {};
	DIJOYSTATE oldGamePadState2 = {};
	bool is_push[32] = {};
	//�X�e�B�b�N�̖������͈�
	LONG unresponsive_range = 200;
	LONG unresponsive_range2 = 200;
private:
	WinApp* winApp = nullptr;
};