#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Curtain.h"
#include "Map.h"

class IntroScene : public CScene
{
protected:
	Map* IntroMap;
	vector<LPGAMEOBJECT> objects;

public:
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);
	Curtain* curtain;
	Curtain* Part2;
	Curtain* Part3;
	Curtain* Number;
	Curtain* Choose;

	IntroScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Load2(float, float) {};
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	//friend class CPlayScenceKeyHandler;
};

class IntroSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	IntroSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};




