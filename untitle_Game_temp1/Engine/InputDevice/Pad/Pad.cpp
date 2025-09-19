#include "Pad.h"
#include "DxLib.h"

namespace
{
	int padInput = 0;  // 現在のフレームのパッド入力
	int lastInput = 0; // ひとつ前のフレームのパッド入力
}

namespace Pad
{
	void Update()
	{
		// 前のフレームに押されていたボタンの情報を覚えておく
		lastInput = padInput;
		// 個のフレームに押されているボタンの情報を取得する
		padInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	}

	bool IsPress(int button)
	{
		return (padInput & button);
	}

	bool IsTrigger(int button)
	{
		// (padInput & button) == 0の場合false
		// それ以外の場合はtrueが入る
		bool isNow = (padInput & button);
		bool isLast = (lastInput & button);
		return (isNow && !isLast);
	}
}