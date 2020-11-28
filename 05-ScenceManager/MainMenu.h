#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "MenuOption.h"
#include "Map.h"
#include "Mario.h"

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP		7

#define OBJECT_TYPE_MENUOPTION	0

#define MAX_SCENE_LINE 1024

class MainMenu : public CScene
{
protected:			
	Mario* player;
	MenuOption *MO;
	Map *map;
	vector<LPGAMEOBJECT> objects;

	void _ParseSection_MAP(string line);
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);


public:
	int ID;
	MainMenu(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	MenuOption* GetMenuOption() { return MO; }
	int GetID() { return id; }

	//friend class CPlayScenceKeyHandler;
};

class MainMenuKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	MainMenuKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

