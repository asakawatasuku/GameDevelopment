#include "JoyPad.h"

// ���o�����W���C�p�b�h�̏����i�[����\����
struct SearchJoyPadSet
{
	// DirectInput
	LPDIRECTINPUT8 dinput;
	// �e�R���g���[���[���̃f�o�C�X
	std::vector<LPDIRECTINPUTDEVICE8> JoyPadDev;
	// �R���X�g���N�^
	SearchJoyPadSet()
	{
		dinput = NULL;
	}
};

//BOOL CALLBACK EnumObjectsCallback(LPCDIDEVICEOBJECTINSTANCE pInstance,LPVOID pvRef)
//{
//	// ��������
//	return DIENUM_CONTINUE;
//}

// ���o�����W���C�p�b�h��ƌĂ΂��R�[���o�b�N
BOOL CALLBACK EnumJoyPadCallBack(LPCDIDEVICEINSTANCE lpddi,void* pvRef)
{
	// ����
	HRESULT ret;

	SearchJoyPadSet* param = reinterpret_cast<SearchJoyPadSet*>(pvRef);
	LPDIRECTINPUTDEVICE8 dev;
	// �R���g���[���[���Ƀf�o�C�X�𐶐�
	ret = param->dinput->CreateDevice(lpddi->guidInstance, &dev,NULL);
	// �����Ɏ��s
	if (ret != DI_OK)
	{
		goto NEXT;
	}

	// �f�[�^�`����ݒ�(�g���@�\�t��)
	dev->SetDataFormat(&c_dfDIJoystick2);
	// �����Ɏ��s
	if (ret != DI_OK)
	{
		goto NEXT;
	}

	// �z��ɒǉ�
	param->JoyPadDev.push_back(dev);
	
NEXT:
	// ��������
	return DIENUM_CONTINUE;
}

JoyPad::JoyPad()
{
}

JoyPad::~JoyPad()
{
	// �z��ɂ���f�o�C�X���J��
	std::vector<JoyPadSet>::iterator it;
	for (it = m_joyPadSet.begin(); it != m_joyPadSet.end(); it++)
	{
		SafeRelease(it->inputDevice);
	}
}

// �������֐�
bool JoyPad::initialize(HWND window)
{
	HRESULT ret;

	// DirectInput�̍쐬
	ret = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DInput, NULL);
	// �����Ɏ��s
	if (ret != DI_OK)
	{
		return false;
	}

	// �󂯓n���p�ϐ�
	SearchJoyPadSet param;
	param.dinput = m_DInput;

	// ���p�\�ȃf�o�C�X�̗�
	ret = m_DInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyPadCallBack, &param, DIEDFL_ATTACHEDONLY);
	// �����Ɏ��s
	if (ret != DI_OK)
	{
		return false;
	}

	// �f�o�C�X�z���ݒ�
	std::vector<LPDIRECTINPUTDEVICE8>::iterator it;
	for (it = param.JoyPadDev.begin(); it != param.JoyPadDev.end(); it++)
	{
		LPDIRECTINPUTDEVICE8 dev = *it;

		// �A�v�����A�N�e�B�u���̓f�o�C�X��r���A�N�Z�X�ɐݒ�
		ret = dev->SetCooperativeLevel(window, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
		// ���s
		if (ret != DI_OK)
		{
			continue;
		}

		// �A�N�Z�X����v��
		ret = dev->Acquire();
		// ���s
		if (ret != DI_OK)
		{
			continue;
		}

		// �f�o�C�X1���̏��
		JoyPadSet initPad;
		initPad.inputDevice = dev;
		// �z��ɒǉ�
		m_joyPadSet.push_back(initPad);
	}

	return true;
}

void JoyPad::update()
{
	// �S�Ẵf�o�C�X�ɂ��ď�������
	std::vector<JoyPadSet>::iterator it;
	for (it = m_joyPadSet.begin(); it != m_joyPadSet.end(); it++)
	{
		HRESULT ret;

		// �����X�V
		it->inputDevice->Poll();
		if (ret != DI_OK)
		{
			// �A�N�Z�X����v��
			ret = it->inputDevice->Acquire();
			while (ret == DIERR_INPUTLOST)
			{
				// �A�N�Z�X����v��
				ret = it->inputDevice->Acquire();
			}
		}

		// �O�̏��
		it->joypadOld = it->joypad;

		// ���͏����擾
		ret = it->inputDevice->GetDeviceState(sizeof(DIJOYSTATE2), &it->joypad);
		// ���s
		if (ret != DI_OK)
		{
			continue;
		}

		if ((BYTE)(it->joypad.rgbButtons[0] & 0x80 == 0))
		{
			// �{�^����������Ă���
		}
	}
}
