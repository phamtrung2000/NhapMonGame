#include "PlayScence.h"
#include "FlyGoomba.h"
#include "Koopas.h"
#include "FirePiranhaPlant.h"
#include "GreenPlant.h"
#include "GreenFirePlant.h"
#include "GreenKoopas.h"
#include "GreenFlyKoopas.h"
#include "Coin.h"
#include "ItemBrick.h"
#include "Portal.h"
#include "QuestionBrickItem.h"
#include "BrickItem.h"
#include "MarioTail.h"
#include "Game.h"
#include "Camera.h"
#include "ButtonP.h"
#include "QuestionBrick.h"
#include "FireBullet.h"
#include "WarpPipe.h"
#include "Block.h"
#include "Card.h"
#include "FlyWood.h"
#include"BoomerangEnemy.h"
#include "RedFlyKoopas.h"
#include "ListItemBrick.h"
#include "ListQuestionBrick.h"
#include "RedGoomba.h"
#include "ListNormalBrick.h"
#include "HiddenMusicBrick.h"
#include "SmallGoomba.h"

CPlayScene* CPlayScene::__instance = NULL;

CPlayScene::CPlayScene()
{
	SceneID = 0;
	key_handler = new CPlayScenceKeyHandler(this);
	Stop = false;
	TypeScene = 0;
	CourseClear = false;
	MarioLevel = 1;
}

CPlayScene::CPlayScene(int id, LPCWSTR filePath) : CScene(id, filePath)
{
	SceneID = id;
	key_handler = new CPlayScenceKeyHandler(this);
	Stop = false;
	TypeScene = 0;
	CourseClear = false;
	MarioLevel = 1;
}

CPlayScene::CPlayScene(int id, LPCWSTR filePath, int mariolv)
{
	SceneID = id;
	key_handler = new CPlayScenceKeyHandler(this);
	Stop = false;
	TypeScene = 0;
	CourseClear = false;
	MarioLevel = mariolv;
}

void CPlayScene::_ParseSection_TEXTURES(string line)
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

void CPlayScene::_ParseSection_SPRITES(string line)
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

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (unsigned int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (unsigned int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	int width{}, height{};
	int Item{};
	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	CATEGORY object_category = static_cast<CATEGORY>(atoi(tokens[0].c_str()));
	int object_type = atoi(tokens[1].c_str());
	float x = (float) atof(tokens[2].c_str());
	float y = (float)atof(tokens[3].c_str());
	int ani_set_id = atoi(tokens[4].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_category)
	{
		case CATEGORY::PLAYER:
		{
			if (tokens.size() > 4)
			{
				float NewX = (float)atof(tokens[5].c_str());
				float NewY = (float)atof(tokens[6].c_str());
				_Mario->SetPosition(x, y);
				_Mario->start_x = x;
				_Mario->start_y = y;
				_Mario->NewX = NewX;
				_Mario->NewY = NewY;
			}
			else
			{
				_Mario->SetPosition(x, y);
				_Mario->start_x = x;
				_Mario->start_y = y;
			}
			obj = _Mario;
			DebugOut(L"[INFO] Player object created!\n");
			LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
			obj->SetAnimationSet(ani_set);

			obj->SetPosition(x, y);
		}break;

		case CATEGORY::ENEMY:
		{
			switch (object_type)
			{
			case OBJECT_TYPE_GOOMBA:
			{
				obj = new Goomba();
			}break;
			case OBJECT_TYPE_KOOPAS:
			{
				obj = new Koopas();
			}break;
			case OBJECT_TYPE_FIREPIRANHAPLANT:
			{
				obj = new FirePiranhaPlant();
			}break;
			case OBJECT_TYPE_GREENPLANT:
			{
				obj = new GreenPlant();
				x += GREENPLANT_BBOX_WIDTH / 2;
			} break;
			case OBJECT_TYPE_GREENFIREPLANT:
			{
				obj = new GreenFirePlant();
				x += GREENPLANT_BBOX_WIDTH / 2;
			} break;
			case OBJECT_TYPE_GREENKOOPAS:
			{
				obj = new GreenKoopas();
			}
			break;
			case OBJECT_TYPE_GREENFLYKOOPAS:
			{
				obj = new GreenFlyKoopas();
			}
			break;
			case OBJECT_TYPE_REDFLYKOOPAS:
			{
				obj = new RedFlyKoopas();
			}
			break;
			case OBJECT_TYPE_REDGOOMBA:
			{
				obj = new RedGoomba();
			}
			break;
			case OBJECT_TYPE_BOOMERANGENEMY:
			{
				obj = new BoomerangEnemy(x, y);
			}break;

			case OBJECT_TYPE_FLYGOOMBA:
			{
				obj = new FlyGoomba();
			}
			}
			LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
			obj->SetAnimationSet(ani_set);

			_Grid->AddMovingObject(obj, x, y);
		}break;

		case CATEGORY::OBJECT:
		{
			if (object_type == OBJECT_TYPE_BLOCK || object_type == OBJECT_TYPE_WARPPIPE || object_type == OBJECT_TYPE_GROUND)
			{
				width = (int)atof(tokens[5].c_str());
				height = (int)atof(tokens[6].c_str());
			}
			else if (object_type == OBJECT_TYPE_QUESTIONBRICK || object_type == OBJECT_TYPE_BRICK || object_type == OBJECT_TYPE_ITEMBRICK)
			{
				Item = (int)atof(tokens[5].c_str());
			}

			switch (object_type)
			{
				case OBJECT_TYPE_BRICK: obj = new Brick(Item,x,y); break;
				case OBJECT_TYPE_QUESTIONBRICK: obj = new QuestionBrick(Item, x, y); break;
				case OBJECT_TYPE_WARPPIPE: obj = new WarpPipe(width, height, atoi(tokens[7].c_str()), atoi(tokens[8].c_str())); break;
				case OBJECT_TYPE_BLOCK: obj = new Block(width, height); break;
				case OBJECT_TYPE_LISTITEMBRICK:
				{
					int NumberBrick = (int)atof(tokens[5].c_str());
					vector<int>ListBrickType;
					for (int i = 0; i < NumberBrick; i++)
					{
						int BrickType = (int)atof(tokens[6 + i].c_str());
						ListBrickType.push_back(BrickType);
					}
					obj = new ListItemBrick(NumberBrick, ListBrickType, x, y); 
				}
				break;
				case OBJECT_TYPE_LISTQUESTIONBRICK:
				{
					int NumberBrick = (int)atof(tokens[5].c_str());
					vector<int>ListBrickType;
					for (int i = 0; i < NumberBrick; i++)
					{
						int BrickType = (int)atof(tokens[6 + i].c_str());
						ListBrickType.push_back(BrickType);
					}
					obj = new ListQuestionBrick(NumberBrick, ListBrickType, x, y); 
				}
				break;
				case OBJECT_TYPE_LISTNORMALBRICK:
				{
					int NumberBrick = (int)atof(tokens[5].c_str());
					vector<int>ListBrickType;
					for (int i = 0; i < NumberBrick; i++)
					{
						int BrickType = (int)atof(tokens[6 + i].c_str());
						ListBrickType.push_back(BrickType);
					}
					obj = new ListNormalBrick(NumberBrick, ListBrickType, x, y);
				}
				break;
				case OBJECT_TYPE_ITEMBRICK: obj = new ItemBrick(Item, x, y); break;
				case OBJECT_TYPE_BUTTONP: obj = new ButtonP(x, y); break;
				case OBJECT_TYPE_FLYWOOD: obj = new FlyWood(x, y); break;
				case OBJECT_TYPE_GROUND:
				{
					obj = new Ground(width, height); break;
					listGround.push_back(obj);
				}
				case OBJECT_TYPE_PORTAL: 
				{
					float r = (float)atof(tokens[5].c_str());
					float b = (float)atof(tokens[6].c_str());
					int scene_id = (int)atoi(tokens[7].c_str());
					obj = new CPortal(x, y, r, b, scene_id);
				}break;
				case OBJECT_TYPE_MUSICBRICK: obj = new MusicBrick(x, y); break;
				case OBJECT_TYPE_HIDDENMUSICBRICK: obj = new HiddenMusicBrick(x, y); break;
			}
			LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
			obj->SetAnimationSet(ani_set);

			_Grid->AddStaticObject(obj, x, y);
		}break;

		case CATEGORY::ITEM:
		{
			switch (object_type)
			{
			case OBJECT_TYPE_COIN: obj = new Coin(); break;
			case OBJECT_TYPE_CARD: obj = new Card(); break;
			default:
				break;
			}
			LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
			obj->SetAnimationSet(ani_set);

			_Grid->AddStaticObject(obj, x, y);
		}
		break;

		default:
			break;
	}
}

void CPlayScene::_ParseSection_MAP(string line)
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

	_Map->LoadMap1(texID, pathtxt,MapWidth,MapHeight);
}

void CPlayScene::_ParseSection_HUD(string line)
{
	vector<string> tokens = split(line);

	//if (tokens.size() < 6) return; // skip invalid lines

	wstring pathtxt = ToWSTR(tokens[0]);

	_HUD->LoadHUD(pathtxt);
}

void CPlayScene::_ParseSection_MARIO(string line)
{
	vector<string> tokens = split(line);

	//if (tokens.size() < 6) return; // skip invalid lines

	wstring pathtxt = ToWSTR(tokens[0]);

	ifstream f;
	f.open(pathtxt);
	// current resource section flag
	int section = HUD_SECTION_UNKNOWN;

	char str[MAX_HUD_LINE];
	while (f.getline(str, MAX_HUD_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") {
			section = SCENE_SECTION_TEXTURES; continue;
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

		if (line[0] == '[') { section = HUD_SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		}
	}
	f.close();
}

void CPlayScene::_ParseSection_OBJECT(string line)
{
	vector<string> tokens = split(line);

	wstring pathtxt = ToWSTR(tokens[0]);

	ifstream f;
	f.open(pathtxt);
	// current resource section flag
	int section = HUD_SECTION_UNKNOWN;

	char str[MAX_HUD_LINE];
	while (f.getline(str, MAX_HUD_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") {
			section = SCENE_SECTION_TEXTURES; continue;
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

		if (line[0] == '[') { section = HUD_SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		}
	}
	f.close();
}

void CPlayScene::_ParseSection_ENEMY(string line)
{
	vector<string> tokens = split(line);

	//if (tokens.size() < 6) return; // skip invalid lines

	wstring pathtxt = ToWSTR(tokens[0]);

	ifstream f;
	f.open(pathtxt);
	// current resource section flag
	int section = HUD_SECTION_UNKNOWN;

	char str[MAX_HUD_LINE];
	while (f.getline(str, MAX_HUD_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") {
			section = SCENE_SECTION_TEXTURES; continue;
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

		if (line[0] == '[') { section = HUD_SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		}
	}
	f.close();
}

void CPlayScene::_ParseSection_ITEM(string line)
{
	vector<string> tokens = split(line);

	//if (tokens.size() < 6) return; // skip invalid lines

	wstring pathtxt = ToWSTR(tokens[0]);

	ifstream f;
	f.open(pathtxt);
	// current resource section flag
	int section = HUD_SECTION_UNKNOWN;

	char str[MAX_HUD_LINE];
	while (f.getline(str, MAX_HUD_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") {
			section = SCENE_SECTION_TEXTURES; continue;
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

		if (line[0] == '[') { section = HUD_SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		}
	}
	f.close();
}

void CPlayScene::_ParseSection_EFFECT(string line)
{
	vector<string> tokens = split(line);

	//if (tokens.size() < 6) return; // skip invalid lines

	wstring pathtxt = ToWSTR(tokens[0]);

	ifstream f;
	f.open(pathtxt);
	// current resource section flag
	int section = HUD_SECTION_UNKNOWN;

	char str[MAX_HUD_LINE];
	while (f.getline(str, MAX_HUD_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") {
			section = SCENE_SECTION_TEXTURES; continue;
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

		if (line[0] == '[') { section = HUD_SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		}
	}
	f.close();
}

void CPlayScene::_ParseSection_WEAPON(string line)
{
	vector<string> tokens = split(line);

	//if (tokens.size() < 6) return; // skip invalid lines

	wstring pathtxt = ToWSTR(tokens[0]);

	ifstream f;
	f.open(pathtxt);
	// current resource section flag
	int section = HUD_SECTION_UNKNOWN;

	char str[MAX_HUD_LINE];
	while (f.getline(str, MAX_HUD_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") {
			section = SCENE_SECTION_TEXTURES; continue;
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

		if (line[0] == '[') { section = HUD_SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		}
	}
	f.close();
}

void CPlayScene::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;
	if (tokens[0] == "time")
	{
		_HUD->PlayTime = (int(atoi(tokens[1].c_str())));
	}
	else if (tokens[0] == "cam")
	{
		_Camera->SetCamScene((float)(atoi(tokens[1].c_str())), (float)(atoi(tokens[2].c_str())), (float)(atoi(tokens[3].c_str())) - SCREEN_WIDTH, (float)(atoi(tokens[4].c_str())));
		_Camera->SetCamMove(int(atoi(tokens[5].c_str())));
	}
	else if (tokens[0] == "scene")
	{
		TypeScene = int(atoi(tokens[1].c_str()));
	}

}

void CPlayScene::Load()
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
		if (line == "[EFFECT]") {
			section = SCENE_SECTION_EFFECT; continue;
		}
		if (line == "[ENEMY]") {
			section = SCENE_SECTION_ENEMY; continue;
		}
		if (line == "[HUD]") {
			section = SCENE_SECTION_HUD; continue;
		}
		if (line == "[ITEM]") {
			section = SCENE_SECTION_ITEM; continue;
		}
		if (line == "[MARIO]") {
			section = SCENE_SECTION_MARIO; continue;
		}
		if (line == "[OBJECT]") {
			section = SCENE_SECTION_OBJECT; continue;
		}
		if (line == "[WEAPON]") {
			section = SCENE_SECTION_WEAPON; continue;
		}
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line == "[SETTINGS]") {
			section = SCENE_FILE_SECTION_SETTINGS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;

		case SCENE_SECTION_EFFECT: _ParseSection_EFFECT(line); break;
		case SCENE_SECTION_ENEMY: _ParseSection_ENEMY(line); break;
		case SCENE_SECTION_HUD: _ParseSection_HUD(line); break;
		case SCENE_SECTION_ITEM: _ParseSection_ITEM(line); break;
		case SCENE_SECTION_MARIO: _ParseSection_MARIO(line); break;
		case SCENE_SECTION_OBJECT: _ParseSection_OBJECT(line); break;
		case SCENE_SECTION_WEAPON: _ParseSection_WEAPON(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_FILE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		}
	}

	f.close();
	
	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(0, 255, 255));
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}


void CPlayScene::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	coObjects.clear();
	_Grid->UpdateCellInViewPort();

	if (this->Stop == false)
	{
		_Camera->Update();
		
		/*for (auto& obj : listGround) {
			coObjects.push_back(obj);
		}*/
		for (auto& obj : _Grid->GetObjectInViewPort()) {
			coObjects.push_back(obj);
		}
		
		_Mario->Update(dt, &coObjects);
		for (auto& obj : _Grid->GetObjectInViewPort())
		{
			obj->Update(dt, &coObjects);
		}
		
		_HUD->Update(dt);
		// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
		if (_Mario == NULL) return;
		/*DebugOut(L"coObjects.size() = %i\n", coObjects.size());
		DebugOut(L"objects.size() = %i\n", objects.size());*/
	}
	else
	{
		for (auto& obj : _Grid->GetObjectInViewPort()) 
		{
			if (obj->IsMovingObject == false)
				coObjects.push_back(obj);
		}

		_Mario->Update(dt, &coObjects);
		for (auto& obj : _Grid->GetObjectInViewPort())
		{
			if(obj->IsMovingObject == false)
				obj->Update(dt, &coObjects);
		}
	}
}

void CPlayScene::Render()
{
	_Map->DrawMap1();
	
	if (_Mario->GoHiddenWorld == true)
	{
		_Mario->Render();

		for (auto& obj : _Grid->GetObjectInViewPort())
		{
			if (obj->IsMovingObject == true)
				obj->Render();
		}
		for (auto& obj : _Grid->GetObjectInViewPort())
		{
			if (obj->IsMovingObject == false)
				obj->Render();
		}
	}
	else
	{
		_Mario->Render();
	
		for (auto& obj : _Grid->GetObjectInViewPort())
		{
			if(obj->IsMovingObject == true)
				obj->Render();
		}
		for (auto& obj : _Grid->GetObjectInViewPort())
		{
			if (obj->IsMovingObject == false)
				obj->Render();
		}
	}
	
	_HUD->Render();
	//_Grid->RenderCell();
}

void CPlayScene::Unload()
{
	for (auto& obj : listGround)
		delete obj;
	/*for (auto& obj : _Grid->GetObjectInViewPort())
	{
		delete obj;
	}*/
	listGround.clear();
	_Mario->Unload();
	_Map->UnLoad();
	//_HUD->Unload();
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

CPlayScene* CPlayScene::GetInstance()
{
	if (__instance == NULL) __instance = new CPlayScene();
	return __instance;
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	Mario* mario = _Mario;
	if (_Mario->GetState() == MARIO_STATE_DIE || _Mario->GetState() == MARIO_STATE_ENDSCENE)
		return;
	switch (KeyCode)
	{
	case DIK_H:
	{
		_Mario->SetLevel(MARIO_LEVEL_SMALL);
		_Mario->y = _Mario->y - MARIO_SMALL_BBOX_HEIGHT - 1;
		_Mario->OnGround = false;
	}break;
	
	break;
	case DIK_J:
	{
		_Mario->SetLevel(MARIO_LEVEL_BIG);
		_Mario->y = _Mario->y - MARIO_BIG_BBOX_HEIGHT - 1;
		_Mario->OnGround = false;
	}break;

	case DIK_K:
	{
		_Mario->SetLevel(MARIO_LEVEL_TAIL);
		_Mario->y = _Mario->y - MARIO_TAIL_BBOX_HEIGHT - 1;
		_Mario->OnGround = false;
	}break;

	case DIK_L:
	{
		_Mario->SetLevel(MARIO_LEVEL_FIRE);
		_Mario->y = _Mario->y - MARIO_BIG_BBOX_HEIGHT - 1;
		_Mario->OnGround = false;
	}break;

	case DIK_U:
	{
		_Mario->UpLevel();
		//_Mario->untouchable = true;
	}break;

	case DIK_I:
	{
		_Mario->DownLevel();
	}break;

	case DIK_S:
	{
		DebugOut(L"Down S\n");
		_Mario->pressS = true;
		if (_Mario->level == MARIO_LEVEL_TAIL && _Mario->isFalling == true)
		{
			_Mario->SetState(MARIO_STATE_FLY_LOW);
			_Mario->TimeDelayFly = GetTickCount64();
		}
		else
		{
			if (_Mario->isFalling == false && _Mario->OnGround == true)
			{
				_Mario->SetState(MARIO_STATE_JUMP);
				_Mario->TimeJumpS = GetTickCount64();
			}
		}
			
		if (_Mario->GetLevel() == MARIO_LEVEL_TAIL)
		{
			if (_Mario->canFlyS == false)
			{
				if (_Mario->isMaxRunning == true)
				{
					_Mario->canFlyS = true;
					_Mario->FlyTimePerS = 0;
				}
					
			}
			else
			{
				_Mario->FlyTimePerS = 0;
				_Mario->SetState(MARIO_STATE_FLY_HIGH);
			}
		}
	}
	break;

	case DIK_X:
	{
		//DebugOut(L"Down X\n");
	}
	break;

	case DIK_R: // reset
		_Mario->Reset();
		break;

	case DIK_A:
	{
		DebugOut(L"Down A\n");
		_Mario->pressA = _Mario->isRunning = true;
		_Mario->StopRunning = false;
		if (_Mario->GetLevel() == MARIO_LEVEL_TAIL && _Mario->isAttacking == false)
		{
			_Mario->SetState(MARIO_STATE_ATTACK);
		}
		else if (_Mario->GetLevel() == MARIO_LEVEL_FIRE && _Mario->isAttacking == false)
		{
			_Mario->SetState(MARIO_STATE_ATTACK);
			_Mario->TimeDelayUseFireBullet = TIMEDELAYUSEBFIREBULLET_A;
		}
		
	}
	break;

	case DIK_Z:
	{
		_Mario->pressA = _Mario->isRunning = true;
		if (_Mario->GetLevel() == MARIO_LEVEL_FIRE && _Mario->isAttacking == false)
		{
			_Mario->SetState(MARIO_STATE_ATTACK);
			_Mario->TimeDelayUseFireBullet = TIMEDELAYUSEBFIREBULLET_Z;
		}

	}
	break;

	case DIK_LEFT:
	{
		//DebugOut(L"DIK_LEFT	\n");
		//if (_Mario->GetState() == MARIO_STATE_WALKING_RIGHT)
		//{
		//	//_Mario->SetState(MARIO_STATE_WALKING_LEFT);
		//	_Mario->nx = -1;
		//}
	}
	break;

	case DIK_RIGHT:
	{
		//DebugOut(L"Key down DIK_RIGHT\n");

	}
	break;

	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	//Mario* _Mario = ((CPlayScene*)scence)->GetPlayer();
	Mario* mario = _Mario;
	if (_Mario->GetState() == MARIO_STATE_DIE || _Mario->GetState() == MARIO_STATE_ENDSCENE)
		return;
	switch (KeyCode)
	{
	case DIK_A:
	{
		DebugOut(L"Up A\n");
		_Mario->pressA = false;
		/*if (_Mario->isHolding == true)
			_Mario->isHolding = false;*/

		if (_Mario->endAttack == true)
		{
			_Mario->endAttack = false;
			_Mario->isAttacking = false;
		}
		if (_Mario->isHolding == true) // đang giữ rùa, thả nút a thì chuyển sang đá
		{
			_Mario->canKick = true;
			_Mario->isHolding = false;
		}
	}break;

	case DIK_S:
	{
		//DebugOut(L"Up S\n");
		_Mario->pressS = false;
	}break;

	case DIK_X:
	{
		//DebugOut(L"Up X\n");
		_Mario->isFlyingHigh = _Mario->isFlyingLow = false;
	}break;

	case DIK_P:
		if (_PlayScene->Stop == false)
			_PlayScene->Stop = true;
		else
			_PlayScene->Stop = false;
	}

}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	//Mario* _Mario = ((CPlayScene*)scence)->GetPlayer();

	Mario* mario = _Mario;

	// disable control key when Mario die 
	if (_Mario->GetState() == MARIO_STATE_DIE || _Mario->GetState() == MARIO_STATE_ENDSCENE || _Mario->loseControl==true) 
		return;


	if (game->IsKeyDown(DIK_LEFT))
	{
		if(_Mario->isAttacking != true)
			_Mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		/*if(_Mario->canFlyS == true && _Mario->isFlyingHigh==true)
			_Mario->SetState(MARIO_STATE_FLY_HIGH);
		else*/
		if (_Mario->isAttacking != true)
			_Mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (_Mario->isHolding == false && _Mario->canFlyS == false && _Mario->canFlyX == false)
		{
			_Mario->SetState(MARIO_STATE_SITDOWN);
		}
	}
	else
	{
		if(_Mario->isHolding == true)
			_Mario->SetState(MARIO_STATE_HOLDING_IDLE);
		else
		{
			if (_Mario->isFlyingLow == true)
				_Mario->SetState(MARIO_STATE_FLY_LOW);
			else
				mario->SetState(MARIO_STATE_IDLE);
		}
	}
	if (game->IsKeyDown(DIK_X))
	{
		if (_Mario->OnGround == true)
		{
			if (_Mario->canFlyX == true && _Mario->GetLevel() == MARIO_LEVEL_TAIL) // ở dưới đất nhưng vẫn còn có thể bay
				_Mario->SetState(MARIO_STATE_FLY_HIGH);
			else
				_Mario->SetState(MARIO_STATE_JUMP_LOW);
		}
		if (_Mario->GetLevel() == MARIO_LEVEL_TAIL)
		{
			if (_Mario->isFalling == true)
			{
				_Mario->SetState(MARIO_STATE_FLY_LOW);
				if (game->IsKeyDown(DIK_LEFT))
				{
					_Mario->SetState(MARIO_STATE_WALKING_LEFT);
				}
				else if (game->IsKeyDown(DIK_RIGHT))
				{
					_Mario->SetState(MARIO_STATE_WALKING_RIGHT);
				}
			}
			if (_Mario->isMaxRunning == true)
			{
				_Mario->canFlyX = true;
				if (game->IsKeyDown(DIK_LEFT))
				{
					_Mario->SetState(MARIO_STATE_FLYING_HIGH_LEFT);
				}
				else if (game->IsKeyDown(DIK_RIGHT))
				{
					_Mario->SetState(MARIO_STATE_FLYING_HIGH_RIGHT);
				}
				else
					_Mario->SetState(MARIO_STATE_FLY_HIGH);
			}
		}
	}
	else if (game->IsKeyDown(DIK_A))
	{
		if (_Mario->isHolding == true)
		{
			if (game->IsKeyDown(DIK_LEFT))
			{
				_Mario->SetState(MARIO_STATE_HOLDING_LEFT);
			}
			else if (game->IsKeyDown(DIK_RIGHT))
			{
				_Mario->SetState(MARIO_STATE_HOLDING_RIGHT);
			}
			else
				_Mario->SetState(MARIO_STATE_HOLDING_IDLE);
		}
	}
}