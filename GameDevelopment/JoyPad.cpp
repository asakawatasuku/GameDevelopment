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

//BOOL CALLBACK EnumObjectsCallback(LPCDIDEVICEOBJECTINSTANCE pInstance,LPVOID pvRef)
//{
//	// 次を検索
//	return DIENUM_CONTINUE;
//}

// 検出したジョイパッド後と呼ばれるコールバック
BOOL CALLBACK EnumJoyPadCallBack(LPCDIDEVICEINSTANCE lpddi,void* pvRef)
{
	// 結果
	HRESULT ret;

	SearchJoyPadSet* param = reinterpret_cast<SearchJoyPadSet*>(pvRef);
	LPDIRECTINPUTDEVICE8 dev;
	// コントローラー毎にデバイスを生成
	ret = param->dinput->CreateDevice(lpddi->guidInstance, &dev,NULL);
	// 生成に失敗
	if (ret != DI_OK)
	{
		goto NEXT;
	}

	// データ形式を設定(拡張機能付き)
	dev->SetDataFormat(&c_dfDIJoystick2);
	// 生成に失敗
	if (ret != DI_OK)
	{
		goto NEXT;
	}

	// 配列に追加
	param->JoyPadDev.push_back(dev);
	
NEXT:
	// 次を検索
	return DIENUM_CONTINUE;
}

JoyPad::JoyPad()
{
}

JoyPad::~JoyPad()
{
	// 配列にあるデバイスを開放
	std::vector<JoyPadSet>::iterator it;
	for (it = m_joyPadSet.begin(); it != m_joyPadSet.end(); it++)
	{
		SafeRelease(it->inputDevice);
	}
}

// 初期化関数
bool JoyPad::initialize(HWND window)
{
	HRESULT ret;

	// DirectInputの作成
	ret = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DInput, NULL);
	// 生成に失敗
	if (ret != DI_OK)
	{
		return false;
	}

	// 受け渡し用変数
	SearchJoyPadSet param;
	param.dinput = m_DInput;

	// 利用可能なデバイスの列挙
	ret = m_DInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyPadCallBack, &param, DIEDFL_ATTACHEDONLY);
	// 生成に失敗
	if (ret != DI_OK)
	{
		return false;
	}

	// デバイス配列を設定
	std::vector<LPDIRECTINPUTDEVICE8>::iterator it;
	for (it = param.JoyPadDev.begin(); it != param.JoyPadDev.end(); it++)
	{
		LPDIRECTINPUTDEVICE8 dev = *it;

		// アプリがアクティブ時はデバイスを排他アクセスに設定
		ret = dev->SetCooperativeLevel(window, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
		// 失敗
		if (ret != DI_OK)
		{
			continue;
		}

		// アクセス権を要求
		ret = dev->Acquire();
		// 失敗
		if (ret != DI_OK)
		{
			continue;
		}

		// デバイス1個分の情報
		JoyPadSet initPad;
		initPad.inputDevice = dev;
		// 配列に追加
		m_joyPadSet.push_back(initPad);
	}

	return true;
}

void JoyPad::update()
{
	// 全てのデバイスについて処理する
	std::vector<JoyPadSet>::iterator it;
	for (it = m_joyPadSet.begin(); it != m_joyPadSet.end(); it++)
	{
		HRESULT ret;

		// 情報を更新
		it->inputDevice->Poll();
		if (ret != DI_OK)
		{
			// アクセス権を要求
			ret = it->inputDevice->Acquire();
			while (ret == DIERR_INPUTLOST)
			{
				// アクセス権を要求
				ret = it->inputDevice->Acquire();
			}
		}

		// 前の情報
		it->joypadOld = it->joypad;

		// 入力情報を取得
		ret = it->inputDevice->GetDeviceState(sizeof(DIJOYSTATE2), &it->joypad);
		// 失敗
		if (ret != DI_OK)
		{
			continue;
		}

		if ((BYTE)(it->joypad.rgbButtons[0] & 0x80 == 0))
		{
			// ボタンが押されている
		}
	}
}
