/// <summary>
/// �R���g���[���[�������N���X
/// </summary>
#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include "dinput.h"
#include <vector>

// �e���v���[�g
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

	// �W���C�p�b�h1���̏��
	struct JoyPadSet {
		JoyPadSet() {
			inputDevice = NULL;
		}
		// �f�o�C�X
		LPDIRECTINPUTDEVICE8 inputDevice;
		// ���͏��
		DIJOYSTATE2 joypad;
		// �O��̓��͏��
		DIJOYSTATE2 joypadOld;
	};

	// DirectInput�C���X�^���X
	LPDIRECTINPUT8 m_DInput;

	// �W���C�p�b�h�z��
	std::vector<JoyPadSet> m_joyPadSet;

public:
	JoyPad();
	~JoyPad();
	// �������֐�
	bool initialize(HWND window);
	// �X�V�֐�
	void update();
};
