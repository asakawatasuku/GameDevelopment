/// <summary>
/// コントローラーを扱うクラス
/// </summary>
#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include "dinput.h"
#include <vector>

class JoyPad
{
private:
	LPDIRECTINPUT8 m_DInput;

public:
	JoyPad();
	~JoyPad();
	bool initialize(HWND window);
	
};
