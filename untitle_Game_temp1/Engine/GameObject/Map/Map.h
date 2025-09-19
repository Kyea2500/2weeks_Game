#pragma once
class Map
{
public:
	Map();
	~Map();
	void Init();
	void End();
	void Update();
	void Draw();
private:
	// マップのデータ
	int m_MapData[10][10];
	// マップのサイズ
	int m_Width;
	int m_Height;
	// マップの画像
	int m_MapImage;
	// マップの位置
	int m_X;
	int m_Y;
};

