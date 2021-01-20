#pragma once
#include "Game.h"
#include "Sprites.h"
#include<fstream>
#include "Utils.h"
#include <string>
#include"GameObject.h"
#include"Utils.h"
#include "Textures.h"

class Map
{
private:
	static Map* __instance;
public:
	int texID;
	int MaxColumn, column;
	int MaxRow, row;
	int TileWidth;
	int TileHeight;
	int TileRow;
	int TileCollum;
	int TiledID[300][300];

	CSprites* sprites = CSprites::GetInstance();
	bool IsWorldMap;

	static Map* GetInstance();
	Map();
	Map(int ID, LPCWSTR FilePath_data, int Num_Rows, int  Num_Cols, int Num_row_read, int Num_col_read, int map_width = 16, int map_height = 16);

	void LoadMap(int texid, wstring map_txt, int& a, int& b);
	void LoadMap1(int texid, wstring map_txt, int& MapWidth, int& MapHeight);
	void DrawMap();
	void DrawMap1();
	void _ParseSection_INFO(string line);
	void _ParseSection_ROWS(string line);
	int GetHeight();
	int GetWidth();
	void UnLoad();
	~Map();
};

