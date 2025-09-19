#include "Map.h"
#include "DxLib.h"

Map::Map(): m_Width(0),
	m_Height(0), 
	m_MapImage(0), 
	m_X(0),
	m_Y(0)
{
}

Map::~Map()
{
}

void Map::Init()
{// マップのサイズを設定
	m_Width = 10;
	m_Height = 10;
	// マップのデータを初期化
	for (int y = 0; y < m_Height; ++y)
	{
		for (int x = 0; x < m_Width; ++x)
		{
			m_MapData[y][x] = 0; // 空のマップ
		}
	}
	// マップの画像を読み込む
	m_MapImage = LoadGraph("map.png");
	if (m_MapImage == -1)
	{
		// エラー処理
		// 画像の読み込みに失敗した場合、エラーメッセージを表示
		DrawString(0, 0, "Failed to load map image", GetColor(255, 0, 0));
	}
}

void Map::End()
{
	// マップの画像を解放
	if (m_MapImage != -1)
	{
		DeleteGraph(m_MapImage);
		m_MapImage = -1;
	}
}

void Map::Update()
{

}

void Map::Draw()
{
}
