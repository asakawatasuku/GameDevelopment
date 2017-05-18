/// <summary>
/// コントローラーを扱うクラス
/// </summary>
#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include "dinput.h"
#include <vector>

// テンプレート
template <typename T>
void SafeRelease(T*& p)
{
	if (p)
	{
		p->Release();
		p = NULL;
	}
}

class JoyPad
{
private:

	// ジョイパッド1個分の情報
	struct JoyPadSet {
		JoyPadSet() {
			inputDevice = NULL;
		}
		// デバイス
		LPDIRECTINPUTDEVICE8 inputDevice;
		// 入力情報
		DIJOYSTATE2 joypad;
		// 前回の入力情報
		DIJOYSTATE2 joypadOld;
	};

	// DirectInputインスタンス
	LPDIRECTINPUT8 m_DInput;

	// ジョイパッド配列
	std::vector<JoyPadSet> m_joyPadSet;

public:
	JoyPad();
	~JoyPad();
	// 初期化関数
	bool initialize(HWND window);
	// 更新関数
	void update();
};
