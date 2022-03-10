#pragma once
#include <wrl.h>
#include <windows.h>
#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
#include <dinput.h>
#include "WinApp.h"
class Input
{
public:
	//ゲームパッド（ボタン）
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

	//スティック
	enum StickKind
	{
		Up, Down, Right, Left
	};

	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	static Input* GetInstance();
	//初期化
	void Initialize(WinApp* winApp);
	//更新
	//キーボード
	void Update();
	bool PushKey(BYTE keyNumber);
	bool TriggerKey(BYTE keyNumber);
	//マウス
	bool PushMouseLeft();
	bool PushMouseMiddle();
	bool TriggerMouseLeft();
	bool TriggerMouseMiddle();
	MouseMove GetMouseMove();
	//ゲームパッド
	bool LeftTiltStick(int stick);
	bool LeftTriggerStick(int stick);
	bool RightTiltStick(int stick);
	bool RightTriggerStick(int stick);
	bool PushButton(int Button);
	bool TriggerButton(int Button);
	bool PushCrossKey(int CrossKey);
	bool TriggerCrossKey(int CrossKey);
private://メンバ変数
	//キーボードのデパイス
	ComPtr<IDirectInputDevice8> devkeyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};
	//ゲームパッドデバイス
	ComPtr<IDirectInputDevice8> devGamePad;
	DIJOYSTATE gamePadState = {};
	DIJOYSTATE oldGamePadState = {};
	DIJOYSTATE gamePadState2 = {};
	DIJOYSTATE oldGamePadState2 = {};
	bool is_push[32] = {};
	//スティックの無反応範囲
	LONG unresponsive_range = 200;
	LONG unresponsive_range2 = 200;
private:
	WinApp* winApp = nullptr;
};