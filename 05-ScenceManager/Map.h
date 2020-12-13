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
	int TileRow;
	int TileCollum;
	int TiledID[200][200];

	static Map* GetInstance();
	Map();
	void LoadMap(int, wstring );
	void LoadMap(int texid, wstring map_txt, int& a, int& b);
	void DrawMap();
	void _ParseSection_INFO(string line);
	void _ParseSection_ROWS(string line);
	int GetHeight();
	int GetWidth();
	~Map();
};

