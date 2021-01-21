#pragma once
#include <iostream>
#include <fstream>
using namespace std;

#include "Game.h"
#include "Textures.h"
#include "GameObject.h"
#include "Utils.h"
#include "Map.h"
#include "Mario.h"
#include "HUD.h"

#define SCENE_ID_HIDDENMAP_1_1 11

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP		7
#define SCENE_SECTION_HUD		8
#define SCENE_SECTION_MARIO		9
#define SCENE_FILE_SECTION_SETTINGS 10

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_QUESTIONBRICK	4
#define OBJECT_TYPE_WARPPIPE		200
#define OBJECT_TYPE_BLOCK			6
#define OBJECT_TYPE_GROUND			7
#define OBJECT_TYPE_QUESTIONBRICKITEM		8
#define OBJECT_TYPE_FIREPIRANHAPLANT		9
#define OBJECT_TYPE_GREENKOOPAS				10
#define OBJECT_TYPE_GREENFLYKOOPAS			11
#define OBJECT_TYPE_COIN					12
#define OBJECT_TYPE_ITEMBRICK				13
#define OBJECT_TYPE_BRICKITEM				14
#define OBJECT_TYPE_GREENPLANT				15
#define OBJECT_TYPE_GREENFIREPLANT			16
#define OBJECT_TYPE_MARIO_TAIL				17
#define OBJECT_TYPE_BUTTONP					18
#define OBJECT_TYPE_CARD					19
#define OBJECT_TYPE_FLYWOOD					20
#define OBJECT_TYPE_BOOMERANGENEMY			21
#define OBJECT_TYPE_FIREBULLET				100
#define OBJECT_TYPE_BOOMERANG				102

#define OBJECT_TYPE_PORTAL	50

#define QBI_MUSHROOM_ANISET_ID	8

#define FIREBULLET_ANISET_ID	100

#define MAX_SCENE_LINE 1024
#define MAP_MAX_WIDTH	2816

class CPlayScene : public CScene
{
public:
	int SceneID;
	bool Stop;
	int TypeScene;
	static CPlayScene* __instance;
	vector<LPGAMEOBJECT> objects;

	CPlayScene();
	CPlayScene(int id, LPCWSTR filePath);

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);

	void _ParseSection_MAP(string line); // hàm đọc Map trong file txt
	void _ParseSection_HUD(string line); // hàm đọc HUD trong file txt
	void _ParseSection_MARIO(string line); // hàm đọc MARIO trong file txt

	void _ParseSection_SETTINGS(string line);



	virtual void Load();
	virtual void Load2(float, float); // load cảnh + tọa độ mới của mario
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	static CPlayScene* GetInstance();
};

//friend class CPlayScenceKeyHandler;
class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

