#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Map.h"

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP		7

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_QUESTIONBRICK	4
#define OBJECT_TYPE_WARPPIPE		5
#define OBJECT_TYPE_BLOCK			6
#define OBJECT_TYPE_GROUND			7
#define OBJECT_TYPE_QUESTIONBRICKITEM		8
#define OBJECT_TYPE_FIREPIRANHAPLANT		9
#define OBJECT_TYPE_GREENKOOPAS				10
#define OBJECT_TYPE_FIREBULLET				100

#define OBJECT_TYPE_PORTAL	50

#define MUSHROOM_ANISET_ID	8
#define FIREBULLET_ANISET_ID	100

#define MAX_SCENE_LINE 1024

class CPlayScene: public CScene
{
protected: 
	Mario *player;					// A play scene has to have player, right? 
	Map* map;
	vector<LPGAMEOBJECT> objects;

	void _ParseSection_MAP(string line);
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);

	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	Mario * GetPlayer() { return player; } 

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) ;
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

