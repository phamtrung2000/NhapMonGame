#include "PlayScence.h"
#include "Goomba.h"
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

CPlayScene* CPlayScene::__instance = NULL;

CPlayScene::CPlayScene()
{
	SceneID = 0;
	key_handler = new CPlayScenceKeyHandler(this);
	Stop = false;
	TypeScene = 0;
	CourseClear = false;
}

CPlayScene::CPlayScene(int id, LPCWSTR filePath) : CScene(id, filePath)
{
	SceneID = id;
	key_handler = new CPlayScenceKeyHandler(this);
	Stop = false;
	TypeScene = 0;
	CourseClear = false;
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
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
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

	for (int i = 1; i < tokens.size(); i++)
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

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	if (object_type == OBJECT_TYPE_BLOCK || object_type == OBJECT_TYPE_WARPPIPE || object_type == OBJECT_TYPE_GROUND)
	{
		width = atof(tokens[4].c_str());
		height = atof(tokens[5].c_str());
	}
	else if (object_type == OBJECT_TYPE_QUESTIONBRICK || object_type == OBJECT_TYPE_BRICK || object_type == OBJECT_TYPE_ITEMBRICK)
	{
		Item = atof(tokens[4].c_str());
	}

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
	{
		if (tokens.size() > 4)
		{
			float NewX = atof(tokens[4].c_str());
			float NewY = atof(tokens[5].c_str());
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
		//_Mario = (Mario*)obj;
			
		DebugOut(L"[INFO] Player object created!\n");
	}break;

	case OBJECT_TYPE_BRICK: obj = new Brick(Item); break;
	case OBJECT_TYPE_QUESTIONBRICK: obj = new QuestionBrick(Item, x, y); break;
	case OBJECT_TYPE_WARPPIPE:
	{
		obj = new WarpPipe(width, height, atoi(tokens[6].c_str()), atoi(tokens[7].c_str()));
	}break;

	case OBJECT_TYPE_BLOCK: obj = new Block(width, height); break;
	case OBJECT_TYPE_GROUND: obj = new Ground(width, height); break;

	case OBJECT_TYPE_GOOMBA:
	{
		obj = new Goomba();
		obj->StartX = x;
		obj->StartY = y;
	}break;
	case OBJECT_TYPE_KOOPAS:
	{
		obj = new Koopas();
		obj->StartX = x;
		obj->StartY = y;
	}break;
	
	case OBJECT_TYPE_FIREPIRANHAPLANT:
	{
		obj = new FirePiranhaPlant();
		obj->StartX = x;
		obj->StartY = y;
	}break;
	case OBJECT_TYPE_GREENPLANT:
	{
		obj = new GreenPlant();
		x += GREENPLANT_BBOX_WIDTH / 2;
		obj->StartX = x;
		obj->StartY = y;
	} break;
	case OBJECT_TYPE_GREENFIREPLANT:
	{
		obj = new GreenFirePlant();
		x += GREENPLANT_BBOX_WIDTH / 2;
		obj->StartX = x;
		obj->StartY = y;
	} break;
	case OBJECT_TYPE_GREENKOOPAS:
	{
		obj = new GreenKoopas(); 
		obj->StartX = x;
		obj->StartY = y;
	}
	break;
	case OBJECT_TYPE_GREENFLYKOOPAS:
	{
		obj = new GreenFlyKoopas();
		obj->StartX = x;
		obj->StartY = y;
	}
	break;
	case OBJECT_TYPE_REDFLYKOOPAS:
	{
		obj = new RedFlyKoopas();
		obj->StartX = x;
		obj->StartY = y;
	}
	break;

	case OBJECT_TYPE_COIN: obj = new Coin(); break;
	case OBJECT_TYPE_ITEMBRICK: obj = new ItemBrick(Item, x, y); break;
	case OBJECT_TYPE_BUTTONP: obj = new ButtonP(x, y); break;
	case OBJECT_TYPE_CARD: obj = new Card(); break;
	case OBJECT_TYPE_FLYWOOD: obj = new FlyWood(x,y); break;
	case OBJECT_TYPE_BOOMERANGENEMY: obj = new BoomerangEnemy(x, y); break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	obj->SetStartPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);

	objects.push_back(obj);
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

//void CPlayScene::_ParseSection_MAP(string line)
//{
//
//	vector<string> tokens = split(line);
//
//	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());
//
//	if (tokens.size() < 9) return; // skip invalid lines - an object set must have at least id, x, y
//
//	int ID = atoi(tokens[0].c_str());
//	wstring FilePath_data = ToWSTR(tokens[1]);
//
//	int Map_width = atoi(tokens[2].c_str());
//	int Map_height = atoi(tokens[3].c_str());
//	int Num_row_read = atoi(tokens[4].c_str());
//	int Num_col_read = atoi(tokens[5].c_str());
//	int Tile_width = atoi(tokens[6].c_str());
//	int Tile_height = atoi(tokens[7].c_str());
//	int A = atoi(tokens[8].c_str());
//	map = new Map(ID, FilePath_data.c_str(), Map_width, Map_height, Num_row_read, Num_col_read, Tile_width, Tile_height);
//	if (A == 1)
//	{
//		map->IsWorldMap = true;
//	}
//}

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
		_Camera->SetCamScene(int(atoi(tokens[1].c_str())), int(atoi(tokens[2].c_str())), int(atoi(tokens[3].c_str())) - SCREEN_WIDTH, int(atoi(tokens[4].c_str())));
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
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line == "[HUD]") {
			section = SCENE_SECTION_HUD; continue;
		}
		if (line == "[MARIO]") {
			section = SCENE_SECTION_MARIO; continue;
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
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_HUD: _ParseSection_HUD(line); break;
		case SCENE_SECTION_MARIO: _ParseSection_MARIO(line); break;
		case SCENE_FILE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;

		}
	}

	f.close();
	
	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(0, 255, 255));
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Load2(float x, float y)
{
	//	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);
	//
	//	ifstream f;
	//	f.open(sceneFilePath);
	//
	//	// current resource section flag
	//	int section = SCENE_SECTION_UNKNOWN;
	//
	//	char str[MAX_SCENE_LINE];
	//	while (f.getline(str, MAX_SCENE_LINE))
	//	{
	//		string line(str);
	//
	//		if (line[0] == '#') continue;	// skip comment lines	
	//
	//		if (line == "[TEXTURES]") {
	//			section = SCENE_SECTION_TEXTURES; continue;
	//		}
	//
	//		if (line == "[MAP]") {
	//			section = SCENE_SECTION_MAP; continue;
	//		}
	//		if (line == "[SPRITES]") {
	//			section = SCENE_SECTION_SPRITES; continue;
	//		}
	//		if (line == "[ANIMATIONS]") {
	//			section = SCENE_SECTION_ANIMATIONS; continue;
	//		}
	//		if (line == "[ANIMATION_SETS]") {
	//			section = SCENE_SECTION_ANIMATION_SETS; continue;
	//		}
	//		if (line == "[OBJECTS]") {
	//			section = SCENE_SECTION_OBJECTS; continue;
	//		}
	//		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }
	//
	//		switch (section)
	//		{
	//		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
	//		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
	//		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
	//		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
	//		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
	//		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
	//		}
	//	}
	//
	//	f.close();
	//	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(0, 255, 255));
	//	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;

	if (this->Stop == false)
	{
		_Camera->Update();
		// Tạo đạn lửa khi _Mario bắn lửa
		if (_Mario->level == MARIO_LEVEL_FIRE && _Mario->isAttacking == true && (_Mario->ani == MARIO_ANI_FIRE_ATTACK_RIGHT_2 || _Mario->ani == MARIO_ANI_FIRE_ATTACK_LEFT_2))
		{
			if (_Mario->TimeDelayUseFireBullet == TIMEDELAYUSEBFIREBULLET_A)
			{
				// kiểm soát số đạn <=2
				if (_Mario->NumberBullet <= 2 && _Mario->NumberBullet > 0 && _Mario->TimeUseFireBullet == 0)
				{
					//DebugOut(L"level=%i , number = %i \n", _Mario->level, _Mario->NumberBullet);
					FireBullet* fb = new FireBullet(_Mario->x + 5, _Mario->y);
					fb->FireMario = true;
					// chiều của viên đạn

					CAnimationSets* animation_sets = CAnimationSets::GetInstance();
					LPANIMATION_SET ani_set = animation_sets->Get(FIREBULLET_ANISET_ID);
					fb->SetAnimationSet(ani_set);
					objects.push_back(fb);
					_Mario->NumberBullet--;
					// điều kiện dừng vòng lặp, nếu không có thì nó sẽ quăng 2 viên cùng 1 lúc
					//_Mario->isAttacking = false;
					_Mario->TimeUseFireBullet = GetTickCount64();
					//	DebugOut(L"Them dan lua -> objects.size = %i \n", objects.size());
				}
			}
			else if (_Mario->TimeDelayUseFireBullet == TIMEDELAYUSEBFIREBULLET_Z)
			{
				// kiểm soát số đạn <=2
				if (_Mario->NumberBullet <= 2 && _Mario->NumberBullet > 0 && (_Mario->TimeUseFireBullet == 0 || GetTickCount64() - _Mario->TimeUseFireBullet > _Mario->TimeDelayUseFireBullet))
				{
					//DebugOut(L"level=%i , number = %i \n", _Mario->level, _Mario->NumberBullet);
					FireBullet* fb = new FireBullet(_Mario->x + 5, _Mario->y);
					fb->FireMario = true;
					// chiều của viên đạn

					CAnimationSets* animation_sets = CAnimationSets::GetInstance();
					LPANIMATION_SET ani_set = animation_sets->Get(FIREBULLET_ANISET_ID);
					fb->SetAnimationSet(ani_set);
					objects.push_back(fb);
					_Mario->NumberBullet--;
					// điều kiện dừng vòng lặp, nếu không có thì nó sẽ quăng 2 viên cùng 1 lúc
					//_Mario->isAttacking = false;
					_Mario->TimeUseFireBullet = GetTickCount64();

				}
			}
		}
		// tạo object đuôi(MarioTail) khi _Mario quật đuôi, xóa object khi thực hiện xong hành động quật đuôi
		else if (_Mario->level == MARIO_LEVEL_TAIL && _Mario->render_tail == false)
		{
			//DebugOut(L" ani = %i, time %i\n", _Mario->ani, _Mario->time_attack);
			MarioTail* tail = new MarioTail(_Mario->x, _Mario->y + 18);
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(MARIOTAIL_ANI_SET_ID);
			tail->SetAnimationSet(ani_set);
			objects.push_back(tail);
			_Mario->render_tail = true;
		}

		for (size_t i = 1; i < objects.size(); i++)
		{
			if ( objects[i]->canDelete == false && objects[i]->isDisappear == false)
			{
				coObjects.push_back(objects[i]);
				if (objects[i]->ObjType == OBJECT_TYPE_FIREPIRANHAPLANT && objects[i]->state == FIREPIRANHAPLANT_STATE_ATTACK)
				{
					FirePiranhaPlant* a = (FirePiranhaPlant*)objects[i];

					if (a->NumberBullet == 1)
					{
						FireBullet* fb = new FireBullet(a->x, a->y);
						fb->SetSpeed(a->VxBullet, a->VyBullet);
						// chiều của viên đạn
						if (a->nx == 1)
							fb->nx = 1;
						else
							fb->nx = -1;

						CAnimationSets* animation_sets = CAnimationSets::GetInstance();
						LPANIMATION_SET ani_set = animation_sets->Get(FIREBULLET_ANISET_ID);
						fb->SetAnimationSet(ani_set);
						objects.push_back(fb);
						// điều kiện dừng vòng lặp, nếu không có thì nó sẽ quăng 2 viên cùng 1 lúc
						a->NumberBullet--;

						//a->SetState(FIREPIRANHAPLANT_STATE_HIDE);
					}
				}
				else if (objects[i]->ObjType == OBJECT_TYPE_GREENFIREPLANT && objects[i]->state == FIREPIRANHAPLANT_STATE_ATTACK)
				{
					GreenFirePlant* a = (GreenFirePlant*)objects[i];

					if (a->NumberBullet == 1)
					{
						FireBullet* fb = new FireBullet(a->x, a->y);
						fb->SetSpeed(a->VxBullet, a->VyBullet);
						// chiều của viên đạn
						if (a->nx == 1)
							fb->nx = 1;
						else
							fb->nx = -1;

						CAnimationSets* animation_sets = CAnimationSets::GetInstance();
						LPANIMATION_SET ani_set = animation_sets->Get(FIREBULLET_ANISET_ID);
						fb->SetAnimationSet(ani_set);
						objects.push_back(fb);
						// điều kiện dừng vòng lặp, nếu không có thì nó sẽ quăng 2 viên cùng 1 lúc
						a->NumberBullet--;

						//a->SetState(FIREPIRANHAPLANT_STATE_HIDE);
					}
				}

			}
			else if(objects[i]->canDelete == true)
			{
				if (objects[i]->ObjType == OBJECT_TYPE_FIREBULLET)
				{
					FireBullet* fb = (FireBullet*)objects[i];
					if (fb->FireMario == true)
						_Mario->NumberBullet++;
					objects.erase(objects.begin() + i);
				}
				else if (objects[i]->Category == CATEGORY::EFFECT || objects[i]->Category == CATEGORY::WEAPON)
				{
					objects.erase(objects.begin() + i);
				}
				else
					objects.erase(objects.begin() + i);
			}

		}

		for (size_t i = 0; i < objects.size(); i++)
		{
			if (objects[i]->ObjType == OBJECT_TYPE_FIREPIRANHAPLANT)
			{
				FirePiranhaPlant* fireplant = (FirePiranhaPlant*)objects[i];
				fireplant->GetEnemyPos(_Mario->x, _Mario->y);
				if (abs(_Mario->x - fireplant->x) <= 20 && fireplant->Stop == true)
					fireplant->SetState(FIREPIRANHAPLANT_STATE_STOP);
				else if (fireplant->Stop == true)
				{
					fireplant->SetState(FIREPIRANHAPLANT_STATE_APPEAR);
					fireplant->Stop = false;
				}
			}
			else if (objects[i]->ObjType == OBJECT_TYPE_GREENPLANT)
			{
				GreenPlant* greenplant = (GreenPlant*)objects[i];
				greenplant->GetEnemyPos(_Mario->x, _Mario->y);
				if (abs(_Mario->x - greenplant->x) <= 20 && greenplant->isBlocked == true)
					greenplant->SetState(GREENPLANT_STATE_STOP);
				else if (greenplant->isBlocked == true)
				{
					greenplant->SetState(GREENPLANT_STATE_APPEAR);
					greenplant->isBlocked = false;
				}
				//DebugOut(L"_Mario x = %f, _Mario y = %f, plant x = %f, plant y =%f\n", _Mario->x, _Mario->y, fireplant->x, fireplant->y);
			}
			else if (objects[i]->ObjType == OBJECT_TYPE_GREENFIREPLANT)
			{
				GreenFirePlant* greenfireplant = (GreenFirePlant*)objects[i];
				greenfireplant->GetEnemyPos(_Mario->x, _Mario->y);
				if (abs(_Mario->x - greenfireplant->x) <= 20 && greenfireplant->Stop == true)
					greenfireplant->SetState(FIREPIRANHAPLANT_STATE_STOP);
				else if (greenfireplant->Stop == true)
				{
					greenfireplant->SetState(FIREPIRANHAPLANT_STATE_APPEAR);
					greenfireplant->Stop = false;
				}
				//DebugOut(L"_Mario x = %f, _Mario y = %f, plant x = %f, plant y =%f\n", _Mario->x, _Mario->y, fireplant->x, fireplant->y);
			}
			else if (objects[i]->ObjType == OBJECT_TYPE_GREENKOOPAS && objects[i]->GetState() == KOOPAS_STATE_SHELL_HOLD)
			{
				GreenKoopas* greenkoopas = (GreenKoopas*)objects[i];
				if (_Mario->pressA == false)
				{
					greenkoopas->isHold = false;
					if (_Mario->nx == 1)
						greenkoopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
					else
						greenkoopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
				}
				else
				{
					if (_Mario->nx == RIGHT)
					{
						if (_Mario->level == MARIO_LEVEL_SMALL)
						{
							//greenkoopas->x = _Mario->x + MARIO_SMALL_BBOX_WIDTH + 1;
							greenkoopas->x = _Mario->x + MARIO_SMALL_BBOX_WIDTH + 1;
							greenkoopas->y = _Mario->y - 2;
						}
						else if (_Mario->level == MARIO_LEVEL_TAIL)
						{
							greenkoopas->x = _Mario->x + MARIO_TAIL_BBOX_WIDTH + 1;
							greenkoopas->y = _Mario->y + 7;
						}
						else
						{
							greenkoopas->x = _Mario->x + MARIO_BIG_BBOX_WIDTH + 1;
							greenkoopas->y = _Mario->y + 6;
						}
						greenkoopas->dx = _Mario->dx;
						greenkoopas->nx = _Mario->nx;
						//	DebugOut(L"_Mario x = %f, greenkoopas x = %f, _Mario right = %f\n", _Mario->x, greenkoopas->x);
					}
					else
					{

						//greenkoopas->x = _Mario->x - GREENKOOPAS_BBOX_WIDTH - 1;
						//DebugOut(L"vao day, vx=%f\n", _Mario->vx);
						if (_Mario->level == MARIO_LEVEL_SMALL)
						{
							greenkoopas->x = _Mario->x - GREENKOOPAS_BBOX_WIDTH + 1;
							greenkoopas->y = _Mario->y - 2;
						}
						else if (_Mario->level == MARIO_LEVEL_TAIL)
						{
							greenkoopas->x = _Mario->x - GREENKOOPAS_BBOX_WIDTH + 1;
							greenkoopas->y = _Mario->y + 7;
						}
						else
						{
							greenkoopas->x = _Mario->x - GREENKOOPAS_BBOX_WIDTH + 1;
							greenkoopas->y = _Mario->y + 6;
						}
						greenkoopas->dx = _Mario->dx;
						greenkoopas->nx = _Mario->nx;

					}
				}

			}

			objects[i]->Update(dt, &coObjects);
		}
		//_Mario->Update(dt, &coObjects);
		
		_HUD->Update(dt);
		// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
		if (_Mario == NULL) return;
		/*DebugOut(L"coObjects.size() = %i\n", coObjects.size());
		DebugOut(L"objects.size() = %i\n", objects.size());*/
	}
	else
	{
		if(_Mario->isLevelUp == true || _Mario->isLevelDown==true)
		_Mario->Update(dt, &coObjects);
	}
}

void CPlayScene::Render()
{
	// Background đen phía sau
	_Map->DrawMap1();
	
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->canDelete == false && objects[i]->isDisappear == false)
		{
			objects[i]->Render();
		}
	}
	_HUD->Render();
	/*if (CourseClear == true)
	{
		CSprites::GetInstance()->Get(90010)->Draw(_Map->GetWidth() - 250, _Map->GetHeight() - 150);
	}*/
}

void CPlayScene::Unload()
{
	for (unsigned int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
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
	case DIK_1:
	{
		_Mario->SetLevel(MARIO_LEVEL_SMALL);
		_Mario->y = _Mario->y - MARIO_SMALL_BBOX_HEIGHT - 1;
		_Mario->OnGround = false;
	}break;

	case DIK_2:
	{
		_Mario->SetLevel(MARIO_LEVEL_BIG);
		_Mario->y = _Mario->y - MARIO_BIG_BBOX_HEIGHT - 1;
		_Mario->OnGround = false;
	}break;

	case DIK_3:
	{
		_Mario->SetLevel(MARIO_LEVEL_TAIL);
		_Mario->y = _Mario->y - MARIO_TAIL_BBOX_HEIGHT - 1;
		_Mario->OnGround = false;
	}break;

	case DIK_4:
	{
		_Mario->SetLevel(MARIO_LEVEL_FIRE);
		_Mario->y = _Mario->y - MARIO_BIG_BBOX_HEIGHT - 1;
		_Mario->OnGround = false;
	}break;

	case DIK_U:
	{
		//_Mario->UpLevel();
		_Mario->untouchable = true;
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
			_Mario->SetState(MARIO_STATE_JUMP);
			_Mario->TimeJumpS = GetTickCount64();
		}
			
		if (_Mario->GetLevel() == MARIO_LEVEL_TAIL)
		{
			if (_Mario->canFlyS == false)
			{
				if (_Mario->isMaxRunning == true)
					_Mario->canFlyS = true;
			}
			else
			{
				_Mario->SetState(MARIO_STATE_FLY_HIGH);
			}
		}
	}
	break;

	case DIK_X:
	{
		//DebugOut(L"Down X\n");
		_Mario->SetState(MARIO_STATE_JUMP_LOW);
	}
	break;

	case DIK_R: // reset
		_Mario->Reset();
		break;

	case DIK_A:
	{
		_Mario->pressA = _Mario->isRunning = true;
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
		if (_Mario->GetState() == MARIO_STATE_WALKING_RIGHT)
		{
			//_Mario->SetState(MARIO_STATE_WALKING_LEFT);
			_Mario->nx = -1;

		}
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
		//DebugOut(L"Up A\n");
		_Mario->isRunning = false;
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
		_Mario->isFlyingHigh = false;
	}break;

	case DIK_X:
	{
		//DebugOut(L"Up X\n");
		/*if (_Mario->isFlyingHigh == true)
		{
			_Mario->isFlyingHigh = false;
		}*/
		_Mario->isFlyingHigh = _Mario->isFlyingLow = false;
	}break;


	case DIK_RIGHT:
	{
		//DebugOut(L"UP DIK_RIGHT\n");
	}
	break;

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
		_Mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		if(_Mario->canFlyS == true && _Mario->isFlyingHigh==true)
			_Mario->SetState(MARIO_STATE_FLY_HIGH);
		else
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