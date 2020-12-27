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

CPlayScene* CPlayScene::__instance = NULL;

CPlayScene::CPlayScene()
{
	SceneID = 0;
	key_handler = new CPlayScenceKeyHandler(this);
}

CPlayScene::CPlayScene(int id, LPCWSTR filePath) : CScene(id, filePath)
{
	SceneID = id;
	key_handler = new CPlayScenceKeyHandler(this);
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
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_KOOPAS: obj = new Koopas(); break;
	case OBJECT_TYPE_QUESTIONBRICK: obj = new QuestionBrick(Item, x, y); break;
	case OBJECT_TYPE_WARPPIPE:
	{
		if (tokens.size() > 6 && tokens[6] == "true")
			obj = new WarpPipe(width, height, true);
		else
			obj = new WarpPipe(width, height, false);

	}break;

	case OBJECT_TYPE_BLOCK: obj = new Block(width, height); break;
	case OBJECT_TYPE_GROUND: obj = new Ground(width, height); break;
	case OBJECT_TYPE_FIREPIRANHAPLANT: obj = new FirePiranhaPlant(); break;
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
	case OBJECT_TYPE_GREENKOOPAS: obj = new GreenKoopas(); break;
	case OBJECT_TYPE_GREENFLYKOOPAS: obj = new GreenFlyKoopas(); break;
	case OBJECT_TYPE_COIN: obj = new Coin(); break;
	case OBJECT_TYPE_ITEMBRICK: obj = new ItemBrick(Item, x, y); break;
	case OBJECT_TYPE_BUTTONP: obj = new ButtonP(x, y); break;
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

	_Map->LoadMap(texID, pathtxt,MapWidth,MapHeight);
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

	// Tạo đạn lửa khi _Mario bắn lửa
	if (_Mario->level == MARIO_LEVEL_FIRE && _Mario->isAttacking == true && (_Mario->ani == MARIO_ANI_FIRE_ATTACK_RIGHT_2 || _Mario->ani == MARIO_ANI_FIRE_ATTACK_LEFT_2))
	{
		// kiểm soát số đạn <=2
		if (_Mario->NumberBullet <= 2 && _Mario->NumberBullet > 0)
		{
			//DebugOut(L"level=%i , number = %i \n", _Mario->level, _Mario->NumberBullet);
			FireBullet* fb = new FireBullet(_Mario->x + 5, _Mario->y);
			fb->FireMario = true;
			// chiều của viên đạn
			fb->nx = _Mario->nx;

			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(FIREBULLET_ANISET_ID);
			fb->SetAnimationSet(ani_set);
			objects.push_back(fb);
			_Mario->NumberBullet--;
			// điều kiện dừng vòng lặp, nếu không có thì nó sẽ quăng 2 viên cùng 1 lúc
			_Mario->isAttacking = false;
			//	DebugOut(L"Them dan lua -> objects.size = %i \n", objects.size());
		}
	}
	// tạo object đuôi(MarioTail) khi _Mario quật đuôi, xóa object khi thực hiện xong hành động quật đuôi
	else if (_Mario->level == MARIO_LEVEL_TAIL && _Mario->render_tail == false)
	{
		//DebugOut(L" ani = %i, time %i\n", _Mario->ani, _Mario->time_attack);
		MarioTail* tail = new MarioTail(_Mario->x, _Mario->y + 18);
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(1200);
		tail->SetAnimationSet(ani_set);
		objects.push_back(tail);
		_Mario->render_tail = true;
	}

	for (size_t i = 1; i < objects.size(); i++)
	{
		if (objects[i]->isDie == false)
		{
			coObjects.push_back(objects[i]);
			if (objects[i]->ObjType == OBJECT_TYPE_QUESTIONBRICK && objects[i]->state == BRICK_STATE_EMPTY)
			{
				QuestionBrick* a = (QuestionBrick*)objects[i];

				if (a->hasItem == true)
				{
					float x = objects[i]->x;
					float y = objects[i]->y;
					if (a->Item > MONEY)
					{
						switch (_Mario->level)
						{
						case MARIO_LEVEL_SMALL:
						{
							a->Item = MUSHROOM;
						}break;


						default:
							a->Item = LEAF;
							break;
						}
					}
					else if (a->Item == MONEY)
					{
						_HUD->Money++;
						_HUD->Score += 100;
					}

					QuestionBrickItem* questionbrickitem = new QuestionBrickItem(a->Item, x, y);
					questionbrickitem->CaclVx(_Mario->x);
					CAnimationSets* animation_sets = CAnimationSets::GetInstance();
					LPANIMATION_SET ani_set = animation_sets->Get(QBI_MUSHROOM_ANISET_ID);
					questionbrickitem->SetAnimationSet(ani_set);
					objects.push_back(questionbrickitem);
					a->hasItem = false;
				}
			}
			else if (objects[i]->ObjType == OBJECT_TYPE_ITEMBRICK && objects[i]->state == BRICK_STATE_EMPTY)
			{
				ItemBrick* itembrick = (ItemBrick*)objects[i];

				if (itembrick->hasItem == true && itembrick->Item == MUSHROOM)
				{
					float x = objects[i]->x;
					float y = objects[i]->y;
					BrickItem* brickitem = new BrickItem(itembrick->Item, x, y);
					brickitem->CaclVx(_Mario->x);
					objects.push_back(brickitem);
					itembrick->hasItem = false;
				}
			}
			else if (objects[i]->ObjType == OBJECT_TYPE_FIREPIRANHAPLANT && objects[i]->state == FIREPIRANHAPLANT_STATE_ATTACK)
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
		else
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
			//DebugOut(L"_Mario x = %f, _Mario y = %f, plant x = %f, plant y =%f\n", _Mario->x, _Mario->y, fireplant->x, fireplant->y);
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
		else if (objects[i]->ObjType == OBJECT_TYPE_KOOPAS && objects[i]->GetState() == KOOPAS_STATE_SHELL_HOLD)
		{
			Koopas* koopas = (Koopas*)objects[i];
			if (_Mario->pressA == false)
			{
				koopas->isHold = false;
				if (_Mario->nx == 1)
					koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
				else
					koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
			}
			else
			{
				if (_Mario->nx == RIGHT)
				{
					//koopas->x = _Mario->x + MARIO_SMALL_BBOX_WIDTH + 1;
					if (_Mario->level == MARIO_LEVEL_SMALL)
					{
						koopas->x = _Mario->x + MARIO_SMALL_BBOX_WIDTH - 1.0f;
						koopas->y = _Mario->y - 2;
					}
					else if (_Mario->level == MARIO_LEVEL_TAIL)
					{
						koopas->x = _Mario->x + MARIO_TAIL_BBOX_WIDTH - 2.0f;
						koopas->y = _Mario->y + 7;
					}
					else
					{
						koopas->x = _Mario->x + MARIO_BIG_BBOX_WIDTH - 1.0f;
						koopas->y = _Mario->y + 6;
					}
					koopas->dx = _Mario->dx;
					koopas->nx = _Mario->nx;
					//	DebugOut(L"_Mario x = %f, koopas x = %f, _Mario right = %f\n", _Mario->x, koopas->x);
				}
				else
				{

					//koopas->x = _Mario->x - KOOPAS_BBOX_WIDTH - 1;
					//DebugOut(L"vao day, vx=%f\n", _Mario->vx);
					if (_Mario->level == MARIO_LEVEL_SMALL)
					{
						koopas->x = _Mario->x - KOOPAS_BBOX_WIDTH + 1;
						koopas->y = _Mario->y - 2;
					}
					else
					{
						koopas->x = _Mario->x - KOOPAS_BBOX_WIDTH + 1;
						koopas->y = _Mario->y + 5;
					}
					koopas->dx = _Mario->dx;
					koopas->nx = _Mario->nx;

				}
			}

		}
		else if (objects[i]->ObjType == OBJECT_TYPE_GREENKOOPAS && objects[i]->GetState() == GREENKOOPAS_STATE_SHELL_HOLD)
		{
			GreenKoopas* greenkoopas = (GreenKoopas*)objects[i];
			if (_Mario->pressA == false)
			{
				greenkoopas->isHold = false;
				if (_Mario->nx == 1)
					greenkoopas->SetState(GREENKOOPAS_STATE_SHELL_WALKING_RIGHT);
				else
					greenkoopas->SetState(GREENKOOPAS_STATE_SHELL_WALKING_LEFT);
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
		else if (objects[i]->ObjType == OBJECT_TYPE_MARIO_TAIL)
		{
			/*MarioTail* tail = (MarioTail*)objects[i];
			if (_Mario->nx == RIGHT && _Mario->isAttacking == true)
			{
				if (_Mario->ani == MARIO_ANI_TAIL_ATTACK_3)
					tail->SetPosition(_Mario->x + MARIO_TAIL_BBOX_WIDTH, _Mario->y + 18);
				else if (_Mario->ani >= MARIO_ANI_TAIL_ATTACK_4)
					tail->isDie = true;
				else
					tail->SetPosition(_Mario->x, _Mario->y + 18);
			}
			else if (_Mario->nx == LEFT && _Mario->isAttacking == true)
			{
				if (_Mario->ani == MARIO_ANI_TAIL_ATTACK_1)
					tail->SetPosition(_Mario->x - 7, _Mario->y + 18);
				else if (_Mario->ani == MARIO_ANI_TAIL_ATTACK_4)
					tail->isDie = true;
				else
					tail->SetPosition(_Mario->x + MARIO_BIG_BBOX_WIDTH, _Mario->y + 18);
			}*/
		}
		objects[i]->Update(dt, &coObjects);
	}
	//_Mario->Update(dt, &coObjects);
	_Camera->Update();
	_HUD->Update(dt);
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (_Mario == NULL) return;
	DebugOut(L"objects.size() = %i\n", objects.size());
}

void CPlayScene::Render()
{
	// Background đen phía sau
	_Map->DrawMap();
	for (unsigned int i = 1; i < objects.size(); i++)
	{
		if (objects[i]->isDie != true)
		{
			objects[i]->Render();
		}
	}
	_Mario->Render();
	_HUD->Render();
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
	//if (_Mario->GetState() == MARIO_STATE_DIE) return;
	switch (KeyCode)
	{
	case DIK_1:
	{
		_Mario->SetLevel(MARIO_LEVEL_SMALL);
		_Mario->y = _Mario->y - MARIO_SMALL_BBOX_HEIGHT - 1;
	}break;

	case DIK_2:
	{
		_Mario->SetLevel(MARIO_LEVEL_BIG);
		_Mario->y = _Mario->y - MARIO_BIG_BBOX_HEIGHT - 1;
	}break;

	case DIK_3:
	{
		_Mario->SetLevel(MARIO_LEVEL_TAIL);
		_Mario->y = _Mario->y - MARIO_TAIL_BBOX_HEIGHT - 1;
	}break;

	case DIK_4:
	{
		_Mario->SetLevel(MARIO_LEVEL_FIRE);
		_Mario->y = _Mario->y - MARIO_BIG_BBOX_HEIGHT - 1;
	}break;

	case DIK_U:
	{
		_Mario->UpLevel();
	}break;

	case DIK_I:
	{
		_Mario->DownLevel();
	}break;

	case DIK_S:
	{
		DebugOut(L"Down S\n");
		_Mario->SetState(MARIO_STATE_JUMP);
		if (_Mario->GetLevel() == 3)
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
		_Mario->pressA = true;
		if (_Mario->GetLevel() >= 3 && _Mario->isAttacking == false)
		{
			_Mario->SetState(MARIO_STATE_ATTACK);
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
		//DebugOut(L"DIK_RIGHT\n");

	}
	break;

	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	//Mario* _Mario = ((CPlayScene*)scence)->GetPlayer();
	Mario* mario = _Mario;
	switch (KeyCode)
	{
	case DIK_A:
	{
		//if (_Mario->isRunning == true)
		_Mario->isRunning = false;
		_Mario->pressA = false;
		/*if (_Mario->isHolding == true)
			_Mario->isHolding = false;*/

		if (_Mario->endAttack == true)
		{
			_Mario->endAttack = false;
			_Mario->isAttacking = false;
		}
	}break;

	case DIK_S:
	{
		DebugOut(L"Up S\n");
		// thả nút S thì rớt xuống liền
		if (_Mario->vy < 0)
		{
			/*if (_Mario->vy <= -MARIO_JUMP_SPEED_FAST/2 && _Mario->GoHiddenWorld == false)
				_Mario->vy = 0;
			else
				_Mario->isFalling = true;*/
			_Mario->isFalling = true;
			_Mario->OnGround = false;
			if ((_Mario->canFlyX == false && _Mario->canFlyS == false) && _Mario->GoHiddenWorld == false) // đang bay thì k rớt -> bấm liên tục nút S nó bay lên
				_Mario->vy = 0;
		}

		_Mario->isFlyingHigh = _Mario->isFlyingLow = false;
	}break;

	case DIK_X:
	{
		DebugOut(L"Up X\n");
		/*if (_Mario->isFlyingHigh == true)
		{
			_Mario->isFlyingHigh = false;
		}*/
		_Mario->isFlyingHigh = _Mario->isFlyingLow = false;
	}break;


	case DIK_RIGHT:
	{
		DebugOut(L"UP DIK_RIGHT\n");
	}
	break;
	}

}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	//Mario* _Mario = ((CPlayScene*)scence)->GetPlayer();

	Mario* mario = _Mario;

	// disable control key when Mario die 
	if (_Mario->GetState() == MARIO_STATE_DIE) return;

	if (game->IsKeyDown(DIK_LEFT))
	{
		_Mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		_Mario->SetState(MARIO_STATE_WALKING_RIGHT);
		if(_Mario->canFlyS == true && _Mario->isFlyingHigh==true)
			_Mario->SetState(MARIO_STATE_FLY_HIGH);
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (_Mario->GetLevel() > 1)
		{
			_Mario->SetState(MARIO_STATE_SITDOWN);
		}
	}
	else
		_Mario->SetState(MARIO_STATE_IDLE);


	if (game->IsKeyDown(DIK_A))
	{
		if (_Mario->pressA == true && (_Mario->GetState() == MARIO_STATE_WALKING_LEFT || _Mario->GetState() == MARIO_STATE_WALKING_RIGHT))
			_Mario->isRunning = true;
	}
	if (game->IsKeyDown(DIK_X))
	{
		if (_Mario->OnGround == true)
		{
			_Mario->SetState(MARIO_STATE_JUMP_LOW);
			if (_Mario->canFlyX == true)
				_Mario->SetState(MARIO_STATE_FLY_HIGH);
		}
		if (_Mario->GetLevel() == 3)
		{
			if (_Mario->isFalling == true)
			{
				_Mario->SetState(MARIO_STATE_FLY_LOW);
			}
			if (_Mario->isMaxRunning == true)
			{
				_Mario->canFlyX = true;
				_Mario->SetState(MARIO_STATE_FLY_HIGH);
			}
		}

		/*	if (_Mario->OnGround == true)
			{
				_Mario->SetState(MARIO_STATE_JUMP_LOW);
				if (_Mario->GetLevel() == 3)
				{
					if (_Mario->isMaxRunning == true)
					{
						_Mario->canFlyX = true;
						_Mario->SetState(MARIO_STATE_FLY_HIGH);
					}
				}
			}
			else
			{
				if (_Mario->isFalling == true)
				{
					_Mario->SetState(MARIO_STATE_FLY_LOW);
				}
				else
				{
					if(_Mario->canFlyX == true)
						_Mario->SetState(MARIO_STATE_FLY_HIGH);
				}
			}*/

	}
}