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
	int MaxColumn, column;
	int MaxRow, row;
	int TileWidth;
	int TileRow;
	int TileCollum;
	int TiledID[200][200];
public:
	Map();
	void LoadMap(wstring map_txt);
	void DrawMap();
	void _ParseSection_INFO(string line);
	void _ParseSection_ROWS(string line);
	~Map();
};

