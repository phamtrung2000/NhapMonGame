#pragma once
#include <iostream>
#include <fstream>
using namespace std;

#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "MarioOverWorld.h"
#include "Ground.h"
#include "Map.h"
#include "Gate.h"
#include "PlayScence.h"

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP		7
#define SCENE_SECTION_HUD		8

#define OBJECT_TYPE_MARIOOVERWORLD	0
#define OBJECT_TYPE_GATE			2
#define OBJECT_TYPE_DANCINGTREE		3

#define MAX_SCENE_LINE 1024
#define MAP_MAX_WIDTH	2816

class World1 : public CScene
{
protected:
	MarioOverWorld* player;					// A play scene has to have player, right? 
	//Map* map;
	vector<LPGAMEOBJECT> objects;
	int MapHeight, MapWidth;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);

	void _ParseSection_MAP(string line);
	void _ParseSection_HUD(string line); // hàm đọc HUD trong file txt
public:
	World1(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Load2(float, float);
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	MarioOverWorld* GetPlayer() { return player; }

	//friend class World1ScenceKeyHandler;
};

class World1ScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	World1ScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

