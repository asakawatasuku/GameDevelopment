#include "JoyPad.h"

// 検出したジョイパッドの情報を格納する構造体
struct SearchJoyPadSet
{
	// DirectInput
	LPDIRECTINPUT8 dinput;
	// 各コントローラー毎のデバイス
	std::vector<LPDIRECTINPUTDEVICE8> JoyPadDev;
	// コンストラクタ
	SearchJoyPadSet()
	{
		dinput = NULL;
	}
};

// 検出したジョイパッド後と呼ばれるコールバック
BOOL CALLBACK EnumJoyPadCallBack(LPCDIDEVICEINSTANCE lpddi,void* pvRef)
{
	HRESULT ret;
	SearchJoyPadSet* param = reinterpret_cast<SearchJoyPadSet*>(pvRef);
	LPDIRECTINPUTDEVICE8 dev;
	// コントローラー毎にデバイスを生成
	ret = param->dinput->CreateDevice(lpddi->guidInstance, &dev,NULL);
	// 失敗
	if (ret != DI_OK)
	{
		goto NEXT;
	}

NEXT:
	// 次を検索
	return DIENUM_CONTINUE;
}

JoyPad::JoyPad()
{
}

JoyPad::~JoyPad()
{
}

// 初期化関数
bool JoyPad::initialize(HWND window)
{
	HRESULT ret;

	// DirectInputの作成
	ret = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DInput, NULL);

	// 失敗
	if (ret != DI_OK)
	{
		return false;
	}

	// 受け渡し用変数
	SearchJoyPadSet param;

	param.dinput = m_DInput;

	// 利用可能なデバイスの列挙
	ret = m_DInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyPadCallBack, &param, DIEDFL_ATTACHEDONLY);

	// 失敗
	if (ret != DI_OK)
	{
		return false;
	}

	return true;
}
