﻿#include "World1.h"
#include "Game.h"
#include "HUD.h"

//World1::World1()
//{
//	player = NULL;
//	map = NULL;
//	SceneID = 0;
//	key_handler = new CPlayScenceKeyHandler(this);
//}

World1::World1(int id, LPCWSTR filePath) : CScene(id, filePath)
{
	player = NULL;
	//map = NULL;
	//SceneID = id;
	key_handler = new World1ScenceKeyHandler(this);
	MapHeight = MapWidth = 0;
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

void World1::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());
	wstring pathtxt = ToWSTR(tokens[5]);

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
	//map = new Map();
	//map->LoadMap(texID, pathtxt,MapWidth,MapHeight);
	_Map->LoadMap(texID, pathtxt, MapWidth, MapHeight);
}

void World1::_ParseSection_HUD(string line)
{
	vector<string> tokens = split(line);

	//if (tokens.size() < 6) return; // skip invalid lines

	wstring pathtxt = ToWSTR(tokens[0]);

	_HUD->LoadHUD(pathtxt);
}

void World1::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void World1::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void World1::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void World1::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void World1::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	int width{}, height{};
	int GateNumber{};
	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	int ani_set_id = atoi(tokens[3].c_str());
	if (object_type == OBJECT_TYPE_GROUND)
	{
		
		
	}
	else if (object_type == OBJECT_TYPE_GATE)
	{
		GateNumber = atof(tokens[4].c_str());
	}


	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIOOVERWORLD:
	{
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new MarioOverWorld(x, y);
		player = (MarioOverWorld*)obj;

		DebugOut(L"[INFO] Player object created!\n");
	}break;

	case OBJECT_TYPE_GROUND:
	{
		width = atof(tokens[4].c_str());
		height = atof(tokens[5].c_str());
		if (tokens.size() <= 6)
		{
			obj = new Ground(width, height);
		}
		else
		{
			int BehindGate = atof(tokens[6].c_str());
			obj = new Ground(width, height, BehindGate);
		}
		
	}break;
	case OBJECT_TYPE_GATE: obj = new Gate(GateNumber); break;

	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
	
}

void World1::Load()
{
	if (objects.size() == 0)
	{
		DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

		ifstream f;
		f.open(sceneFilePath);

		// current resource section flag
		int section = SCENE_SECTION_UNKNOWN;

		char str[MAX_SCENE_LINE];
		while (f.getline(str, MAX_SCENE_LINE))
		{
			string line(str);

			if (line[0] == '#') continue;	// skip comment lines	

			if (line == "[TEXTURES]") {
				section = SCENE_SECTION_TEXTURES; continue;
			}

			if (line == "[MAP]") {
				section = SCENE_SECTION_MAP; continue;
			}
			if (line == "[SPRITES]") {
				section = SCENE_SECTION_SPRITES; continue;
			}
			if (line == "[ANIMATIONS]") {
				section = SCENE_SECTION_ANIMATIONS; continue;
			}
			if (line == "[ANIMATION_SETS]") {
				section = SCENE_SECTION_ANIMATION_SETS; continue;
			}
			if (line == "[OBJECTS]") {
				section = SCENE_SECTION_OBJECTS; continue;
			}
			if (line == "[HUD]") {
				section = SCENE_SECTION_HUD; continue;
			}
			if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

			switch (section)
			{
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
			case SCENE_SECTION_HUD: _ParseSection_HUD(line); break;
			}
		}
		f.close();
		_HUD->Init();
		CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
		DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
	}
	
}

void World1::Load2(float x , float y)
{
	if (objects.size() == 0)
	{
		DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

		ifstream f;
		f.open(sceneFilePath);

		// current resource section flag
		int section = SCENE_SECTION_UNKNOWN;

		char str[MAX_SCENE_LINE];
		while (f.getline(str, MAX_SCENE_LINE))
		{
			string line(str);

			if (line[0] == '#') continue;	// skip comment lines	

			if (line == "[TEXTURES]") {
				section = SCENE_SECTION_TEXTURES; continue;
			}

			if (line == "[MAP]") {
				section = SCENE_SECTION_MAP; continue;
			}
			if (line == "[SPRITES]") {
				section = SCENE_SECTION_SPRITES; continue;
			}
			if (line == "[ANIMATIONS]") {
				section = SCENE_SECTION_ANIMATIONS; continue;
			}
			if (line == "[ANIMATION_SETS]") {
				section = SCENE_SECTION_ANIMATION_SETS; continue;
			}
			if (line == "[OBJECTS]") {
				section = SCENE_SECTION_OBJECTS; continue;
			}
			if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

			switch (section)
			{
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
			}
		}

		f.close();
		CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
		DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
	}
	GetPlayer()->SetPosition(x, y);
}

void World1::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;

	for (size_t i = 1; i < objects.size(); i++)
	{
		if (objects[i]->isDie == false)
		{
			coObjects.push_back(objects[i]);
		}
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->ObjType == OBJECT_TYPE_GATE)
		{
			Gate* gate = (Gate*)objects[i];
			//if(gate->isComplete==true)
		}
		objects[i]->Update(dt, &coObjects);
	}

	//DebugOut(L"obj size=%i, cooobj size = %i\n", objects.size(),coObjects.size());

	// skip the rest if scene was already unloaded (MarioOverWorld::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	//// Update camera to follow mario
	//float cx, cy;
	//player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();

	_Camera->Update();
	_Camera->SetCamPos((MapWidth - game->GetScreenWidth()) / 2, (MapHeight - game->GetScreenHeight()) / 4);
	CGame::GetInstance()->SetCamPos((MapWidth - game->GetScreenWidth()) / 2, (MapHeight - game->GetScreenHeight()) / 4);
	_HUD->Update(dt);
	DebugOut(L"cam y = %f\n", _Camera->cam_y);
}

void World1::Render()
{
	//map->DrawMap();
	_Map->DrawMap();
	for (int i = 0; i < objects.size(); i++)
	{
		//if (objects[i]->GetState() != GOOMBA_STATE_DIE)
		if (objects[i]->isDie != true)
		{
			objects[i]->Render();
		}
	}
	_HUD->Render();
}

/*
	Unload current scene
*/
void World1::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;
	_Map->UnLoad();
	_HUD->Unload();
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

//World1* World1::GetInstance()
//{
//	if (__instance == NULL) __instance = new World1();
//	return __instance;
//}

void World1ScenceKeyHandler::OnKeyDown(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	MarioOverWorld* mario = ((World1*)scence)->GetPlayer();
	if (game->IsKeyDown(DIK_LEFT))
	{
		mario->SetState(MARIO_OVERWORLD_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		mario->SetState(MARIO_OVERWORLD_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_UP))
	{
		mario->SetState(MARIO_OVERWORLD_STATE_WALKING_UP);
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		mario->SetState(MARIO_OVERWORLD_STATE_WALKING_DOWN);
	}
	else if (game->IsKeyDown(DIK_RETURN))
	{
		if(mario->Scene > 0 && mario->GetState() == MARIO_OVERWORLD_STATE_IN_GATE)
			CGame::GetInstance()->SwitchScene(mario->Scene);
	}
}

void World1ScenceKeyHandler::OnKeyUp(int KeyCode)
{
	
}

void World1ScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	MarioOverWorld* mario = ((World1*)scence)->GetPlayer();
	
	
}