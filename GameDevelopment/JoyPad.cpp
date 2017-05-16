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

// ���o�����W���C�p�b�h��ƌĂ΂��R�[���o�b�N
BOOL CALLBACK EnumJoyPadCallBack(LPCDIDEVICEINSTANCE lpddi,void* pvRef)
{
	HRESULT ret;
	SearchJoyPadSet* param = reinterpret_cast<SearchJoyPadSet*>(pvRef);
	LPDIRECTINPUTDEVICE8 dev;
	// �R���g���[���[���Ƀf�o�C�X�𐶐�
	ret = param->dinput->CreateDevice(lpddi->guidInstance, &dev,NULL);
	// ���s
	if (ret != DI_OK)
	{
		goto NEXT;
	}

NEXT:
	// ��������
	return DIENUM_CONTINUE;
}

JoyPad::JoyPad()
{
}

JoyPad::~JoyPad()
{
}

// �������֐�
bool JoyPad::initialize(HWND window)
{
	HRESULT ret;

	// DirectInput�̍쐬
	ret = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DInput, NULL);

	// ���s
	if (ret != DI_OK)
	{
		return false;
	}

	// �󂯓n���p�ϐ�
	SearchJoyPadSet param;

	param.dinput = m_DInput;

	// ���p�\�ȃf�o�C�X�̗�
	ret = m_DInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyPadCallBack, &param, DIEDFL_ATTACHEDONLY);

	// ���s
	if (ret != DI_OK)
	{
		return false;
	}

	return true;
}
