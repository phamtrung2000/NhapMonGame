﻿#include "Map.h"

#define MAX_MAP_LINE 1024
#define MAP_SECTION_UNKNOWN		-1
#define MAP_SECTION_INFO		1
#define MAP_SECTION_ROWS		2

Map* Map::__instance = NULL;

Map* Map::GetInstance()
{
	if (__instance == NULL)
		__instance = new Map();
	return __instance;
}

Map::Map()
{
	texID = 0;
	MaxColumn = column = MaxRow = row = TileWidth = TileHeight = TileRow = TileCollum = GridCellSize = 0;
	IsWorldMap = false;
}

void Map::_ParseSection_INFO(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 7) // TH map intro thì k cần grid
	{
		MaxRow = atoi(tokens[0].c_str());
		MaxColumn = atoi(tokens[1].c_str());
		TileRow = atoi(tokens[2].c_str());
		TileCollum = atoi(tokens[3].c_str());
		TileWidth = atoi(tokens[4].c_str());
		TileHeight = atoi(tokens[5].c_str());
	}
	else
	{
		MaxRow = atoi(tokens[0].c_str());
		MaxColumn = atoi(tokens[1].c_str());
		TileRow = atoi(tokens[2].c_str());
		TileCollum = atoi(tokens[3].c_str());
		TileWidth = atoi(tokens[4].c_str());
		TileHeight = atoi(tokens[5].c_str());
		GridCellSize = atoi(tokens[6].c_str());

		int gridsizecell = GridCellSize * OBJECT_BBOX_WIDTH_HEIGHT;
		_Grid->SetSizeCell(gridsizecell);
		int widthmap = _Map->GetWidth();
		int b = (widthmap % gridsizecell == 0) ? (widthmap / gridsizecell) : (widthmap / gridsizecell) + 1;
		_Grid->cols = b;
		int a = (_Map->GetHeight() % gridsizecell == 0) ? (_Map->GetHeight() / gridsizecell) : (_Map->GetHeight() / gridsizecell) + 1;
		_Grid->rows = a;
		_Grid->Init();
	}
}

void Map::_ParseSection_ROWS(string line)
{
	vector<string> tokens = split(line);

	for (unsigned int i = 0; i < tokens.size(); i++)
	{
		int ID = atoi(tokens[i].c_str());

		TiledID[column][row] = ID - 1;
		row++;
	}
	column++;
	row = 0;
}

int Map::GetHeight()
{
	return MaxRow * TileWidth;
}

int Map::GetWidth()
{
	return MaxColumn * TileWidth;
}

void Map::UnLoad()
{
	__instance = NULL;
}

void Map::LoadMap(int texid, wstring map_txt, int& MapWidth, int& MapHeight)
{
	texID = texid;
	ifstream f;
	f.open(map_txt);
	// current resource section flag
	int section = MAP_SECTION_UNKNOWN;
	char str[MAX_MAP_LINE];
	while (f.getline(str, MAX_MAP_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[INFO]") { section = MAP_SECTION_INFO; continue; }
		if (line == "[ROWS]") {
			section = MAP_SECTION_ROWS; continue;
		}

		if (line[0] == '[') { section = MAP_SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case MAP_SECTION_INFO: _ParseSection_INFO(line); break;
		case MAP_SECTION_ROWS: _ParseSection_ROWS(line); break;
		}
	}
	f.close();
	MapWidth = MaxColumn * TileWidth;
	MapHeight = MaxRow * TileWidth;
}

void Map::LoadMap1(int texid, wstring map_txt, int& MapWidth, int& MapHeight)
{
	texID = texid;
	ifstream f;
	f.open(map_txt);
	// current resource section flag
	int section = MAP_SECTION_UNKNOWN;
	char str[MAX_MAP_LINE];
	while (f.getline(str, MAX_MAP_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[INFO]") { section = MAP_SECTION_INFO; continue; }
		if (line == "[ROWS]") {
			section = MAP_SECTION_ROWS; continue;
		}

		if (line[0] == '[') { section = MAP_SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case MAP_SECTION_INFO: _ParseSection_INFO(line); break;
		case MAP_SECTION_ROWS: _ParseSection_ROWS(line); break;
		}
	}
	f.close();
	MapWidth = MaxColumn * TileWidth;
	MapHeight = MaxRow * TileWidth;

	CTextures* texture = CTextures::GetInstance();
	LPDIRECT3DTEXTURE9 texMap = texture->Get(texID);
	int id_sprite = 0;

	for (int i = 0; i < TileRow; i++)
	{
		for (int j = 0; j < TileCollum; j++)
		{
			int id_Sprite = texID + id_sprite;
			sprites->Add(id_Sprite, TileWidth * j, TileHeight * i, TileWidth * (j + 1), TileHeight * (i + 1), texMap);
			id_sprite = id_sprite + 1;
		}
	}
}

void Map::DrawMap()
{
	for (int i = 0; i < MaxRow; i++)
		for (int j = 0; j < MaxColumn; j++)
		{
			int Id = TiledID[i][j] ;
			RECT r;

			r.left = Id % TileCollum * TileWidth;
			r.top = (Id / TileCollum) * TileWidth;
			r.right = r.left + TileWidth;
			r.bottom = r.top + TileWidth;

			CGame::GetInstance()->Draw((float)(j * TileWidth), (float)(i * TileWidth), CTextures::GetInstance()->Get(texID), r.left, r.top, r.right, r.bottom);
		}


	//int i = 22, j = 3;
	//int Id = TiledID[i][j] - 1;
	//RECT r;
	//r.left = Id % TileCollum * TileWidth;

	//if (Id % TileRow == 0)
	//	r.top = ((Id/TileRow)-1) * TileWidth;
	//else
	//	r.top = ((Id / TileRow) - 1) * TileWidth;
	//r.right = r.left + TileWidth;
	//r.bottom = r.top + TileWidth;
	////DebugOut(L"%d\n",Id);
	//CGame::GetInstance()->Draw(j * 16, i * 16, CTextures::GetInstance()->Get(300), r.left, r.top, r.right, r.bottom);
}

void Map::DrawMap1()
{
	//float temp = (Camera::GetInstance()->cam_x / 16);
	float temp = (CGame::GetInstance()->GetCamX() / 16);
	int firstcol = (int)temp;
	if (firstcol < 0)  
		firstcol = 0; 
	int lastcol = firstcol + (SCREEN_WIDTH / 16);
	for ( int i = 0; i < MaxRow; i++)
	{
		for ( int j = firstcol; j <= lastcol; j++)
		{
			float x = (float)(TileWidth * j);
			float y = (float)(TileWidth * i);
			sprites->Get(TiledID[i][j] + texID)->Draw(x, y);
		}
	}
}

Map::~Map()
{
}