#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"

#include "Map.h"
#include "QuestionBrick.h"
#include "WarpPipe.h"
#include "Block.h"
#include"Ground.h"
#include "QuestionBrickItem.h"
#include "FirePiranhaPlant.h"

#define MAP_MAX_WIDTH	2816

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = new Mario();
	map = new Map();
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/



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
	map->LoadMap(pathtxt);
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
		int frame_time = atoi(tokens[i+1].c_str());
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

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS] 
*/
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
	else if (object_type == OBJECT_TYPE_QUESTIONBRICK || object_type == OBJECT_TYPE_BRICK)
	{
		Item = atof(tokens[4].c_str());
	}

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new Mario(x,y); 
		player = (Mario*)obj;  

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_BRICK: obj = new Brick(Item); break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_KOOPAS: obj = new Koopas(); break;
	case OBJECT_TYPE_QUESTIONBRICK: obj = new QuestionBrick(Item,x,y); break;
	case OBJECT_TYPE_WARPPIPE: obj = new WarpPipe(width, height); break;
	case OBJECT_TYPE_BLOCK: obj = new Block(width, height); break;
	case OBJECT_TYPE_GROUND: obj = new Ground(width, height); break;
	case OBJECT_TYPE_FIREPIRANHAPLANT: obj = new FirePiranhaPlant(); break;
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
			section = SCENE_SECTION_TEXTURES; continue; }

		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
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
	//CTextures::GetInstance()->Add(300, L"textures/map1.png", D3DCOLOR_XRGB(255, 255, 255));
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	


	/*for (size_t i = 1; i < objects.size(); i++)
	{
		if (objects[i]->isDie != true)
		{
			coObjects.push_back(objects[i]);
		}
		
	}
	if (player->isAttacking == true)
		DebugOut(L"player->isAttacking == true\n");
	else
		DebugOut(L"player->isAttacking == false\n");*/

	// mario bắn lửa
	if (player->level == MARIO_LEVEL_FIRE && player->isAttacking == true && (player->ani == MARIO_ANI_FIRE_ATTACK_RIGHT_2 || player->ani == MARIO_ANI_FIRE_ATTACK_LEFT_2))
	{
		// kiểm soát số đạn <=2
		if (player->NumberBullet <= 2 && player->NumberBullet > 0)
		{
			//DebugOut(L"level=%i , number = %i \n", player->level, player->NumberBullet);
			FireBullet* fb = new FireBullet(player->x + 5, player->y);
			fb->SetSpeed(0.1 * player->nx, 0);
			fb->FireMario = true;
			// chiều của viên đạn
			if (player->nx == 1)
				fb->nx = 1;
			else
				fb->nx = -1;

			CAnimationSets* animation_sets = CAnimationSets::GetInstance();
			LPANIMATION_SET ani_set = animation_sets->Get(FIREBULLET_ANISET_ID);
			fb->SetAnimationSet(ani_set);
			objects.push_back(fb);
			player->NumberBullet--;
			// điều kiện dừng vòng lặp, nếu không có thì nó sẽ quăng 2 viên cùng 1 lúc
			player->isAttacking = false;
		//	DebugOut(L"Them dan lua -> objects.size = %i \n", objects.size());
		}

	}
	
	for (size_t i = 1; i < objects.size(); i++)
	{
		if (objects[i]->isDie == false)
		{
			coObjects.push_back(objects[i]);
			//DebugOut(L"1.i=%i,	state=%i, tyle=%i\n", i, objects[i]->GetState(), objects[i]->ObjType);
			//DebugOut(L"size=%i\n", objects.size());
		
		}
		else
		{
			if (objects[i]->ObjType == OBJECT_TYPE_FIREBULLET)
			{
				FireBullet* fb = (FireBullet*)objects[i];
				//DebugOut(L"1.i=%i,	state=%i, tyle=%i\n", i, objects[i]->GetState(), objects[i]->ObjType);
				//DebugOut(L"Xoa dan lua -> objects.size = %i \n", objects.size());
				if(fb->FireMario==true)
					player->NumberBullet++;
				objects.erase(objects.begin() + i);
				continue;
			}
				
		}
			
		if (objects[i]->ObjType == OBJECT_TYPE_QUESTIONBRICK && objects[i]->state == BRICK_STATE_EMPTY)
		{
			QuestionBrick* a = (QuestionBrick*)objects[i];

			if (a->hasItem == 0)
			{
				float x = objects[i]->x;
				float y = objects[i]->y;
				if (a->Item > MONEY)
				{
					switch (player->level)
					{
					case MARIO_LEVEL_SMALL:
					{
						a->Item = MUSHROOM;
					}break;

					case MARIO_LEVEL_BIG:
					{
						a->Item = LEAF;
					}break;
					default:
						break;
					}
				}
				QuestionBrickItem* questionbrickitem = new QuestionBrickItem(a->Item,x, y);
				questionbrickitem->CaclVx(player->x);
				CAnimationSets* animation_sets = CAnimationSets::GetInstance();
				LPANIMATION_SET ani_set = animation_sets->Get(MUSHROOM_ANISET_ID);
				questionbrickitem->SetAnimationSet(ani_set);
				objects.push_back(questionbrickitem);
				a->hasItem = 1;
			}
		}

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
		
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
		if (objects[i]->ObjType == OBJECT_TYPE_FIREPIRANHAPLANT)
		{
			FirePiranhaPlant* fireplant = (FirePiranhaPlant*)objects[i];
			fireplant->GetEnemyPos(player->x, player->y);
			if (abs(player->x - fireplant->x) <= 20 && fireplant->Stop == true)
				fireplant->SetState(FIREPIRANHAPLANT_STATE_STOP);
			else if (fireplant->Stop == true)
			{
				fireplant->SetState(FIREPIRANHAPLANT_STATE_APPEAR);
				fireplant->Stop = false;
			}
			DebugOut(L"mario x = %f, mario y = %f, plant x = %f, plant y =%f\n", player->x, player->y, fireplant->x, fireplant->y);
		}
		else if (objects[i]->ObjType == OBJECT_TYPE_KOOPAS && objects[i]->GetState() == KOOPAS_STATE_SHELL_HOLD )
		{
			Koopas* koopas = (Koopas*)objects[i];
			if (player->pressA == false)
			{
				koopas->isHold = false;
				if (player->nx == 1)
					koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
				else
					koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
			}
			else
			{
				if (player->nx == 1)
				{
					//koopas->x = player->x + MARIO_SMALL_BBOX_WIDTH + 1;
					if (player->level == MARIO_LEVEL_SMALL)
					{
						koopas->x = player->x + MARIO_SMALL_BBOX_WIDTH + 1;
						koopas->y = player->y - 2;
					}
					else if (player->level == MARIO_LEVEL_TAIL)
					{
						koopas->x = player->x + MARIO_TAIL_BBOX_WIDTH + 1;
						koopas->y = player->y + 7;
					}
					else
					{
						koopas->x = player->x + MARIO_BIG_BBOX_WIDTH + 1;
						koopas->y = player->y + 6;
					}
					koopas->dx = player->dx;
					koopas->nx = player->nx;
				//	DebugOut(L"player x = %f, koopas x = %f, player right = %f\n", player->x, koopas->x);
				}
				else
				{

					//koopas->x = player->x - KOOPAS_BBOX_WIDTH - 1;
					//DebugOut(L"vao day, vx=%f\n", player->vx);
					if (player->level == MARIO_LEVEL_SMALL)
					{
						koopas->x = player->x - KOOPAS_BBOX_WIDTH - 1;
						koopas->y = player->y - 2;
					}
					else
					{
						koopas->x = player->x - KOOPAS_BBOX_WIDTH - 1;
						koopas->y = player->y + 5;
					}
					koopas->dx = player->dx;
					koopas->nx = player->nx;

				}
			}
			
		}
		
	}

	//DebugOut(L"obj size=%i, cooobj size = %i\n", objects.size(),coObjects.size());

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame *game = CGame::GetInstance();

	cx -= game->GetScreenWidth() / 2;

	// xóa khúc vàng ở sau lúc bắt đầu game
	if (cx < 0)
	{
		cx = 0;
	}
	if (cx > MAP_MAX_WIDTH)
		cx = MAP_MAX_WIDTH;

	cy -= game->GetScreenHeight() /2 ;
	/*if (cy >= 270)
	{
		cy = 270;
	}
	else
		cy = 270;*/
	/*if (player->y >= 240)
		cy = 240;*/

	if (player->y >= 240)
		cy = 240;

	// CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
	CGame::GetInstance()->SetCamPos(cx, cy);
}

void CPlayScene::Render()
{
	map->DrawMap();
	for (int i = 0; i < objects.size(); i++)
	{
		//if (objects[i]->GetState() != GOOMBA_STATE_DIE)
		
			if(objects[i]->isDie!=true)
			{
				objects[i]->Render();
			}
	}
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	Mario *mario = ((CPlayScene*)scence)->GetPlayer();
	//if (mario->GetState() == MARIO_STATE_DIE)
	//	return;
	//switch (KeyCode)
	//{
	//case DIK_S:
	//	mario->SetState(MARIO_STATE_JUMP);
	//	break;
	//case DIK_X:
	//	mario->SetState(MARIO_STATE_JUMP_LOW);
	//	break;
	//case DIK_R: // reset
	//	mario->SetState(MARIO_STATE_IDLE);
	//	mario->SetLevel(MARIO_LEVEL_SMALL);
	//	mario->SetPosition(40.0f, 150 - BRICK_BBOX_HEIGHT);
	//	mario->SetSpeed(0, 0);
	//	break;
	//case DIK_A:
	//	mario->isRunning = true;
	//	mario->level_of_running = 1;
	//	break;
	//
	//case DIK_LEFT:
	//	mario->level_of_stopping = 1;
	//	if (mario->GetState() == MARIO_STATE_WALKING_RIGHT)
	//	{
	//		mario->SetState(MARIO_STATE_WALKING_LEFT);
	//	}
	//	break;
	//
	//case DIK_RIGHT:
	//	mario->level_of_stopping = 1;
	//	if (mario->GetState() == MARIO_STATE_WALKING_LEFT)
	//	{
	//		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	//	}
	//	break;
	//}


	//DebugOut(L"1. [INFO] KeyDown:	");
	/*if (mario->GetState() == MARIO_STATE_DIE)
		return;*/
	switch (KeyCode)
	{
	case DIK_1:
	{
		mario->SetLevel(MARIO_LEVEL_SMALL);
		mario->y = mario->y - MARIO_SMALL_BBOX_HEIGHT - 1;
	}break;

	case DIK_2:
	{
		mario->SetLevel(MARIO_LEVEL_BIG);
		mario->y = mario->y - MARIO_BIG_BBOX_HEIGHT - 1;
	}break;

	case DIK_3:
	{
		mario->SetLevel(MARIO_LEVEL_TAIL);
		mario->y = mario->y - MARIO_TAIL_BBOX_HEIGHT - 1;
	}break;

	case DIK_4:
	{
		mario->SetLevel(MARIO_LEVEL_FIRE);
		mario->y = mario->y - MARIO_BIG_BBOX_HEIGHT - 1;
	}break;

	case DIK_S:
	{
		//DebugOut(L"Down S\n");
		mario->SetState(MARIO_STATE_JUMP);
		if (mario->GetLevel() == 3)
		{
			
			if (mario->isFalling == true)
			{
				mario->SetState(MARIO_STATE_FLY_LOW);
			}
			if (mario->isMaxRunning == true)
			{
				mario->canFlyS = true;
				mario->SetState(MARIO_STATE_FLY_HIGH);
			}
		}
	}
	break;

	case DIK_X:
	{
		//DebugOut(L"Down X\n");
		mario->SetState(MARIO_STATE_JUMP_LOW);

		//if (mario->GetLevel() == 3)
		//	mario->canFlyX = true;
	}
	break;

	case DIK_R: // reset
	mario->Reset();
	break;

	case DIK_A:
	{
		mario->pressA = true;
		if (mario->GetLevel() >= 3 && mario->isAttacking == false)
		{
			mario->SetState(MARIO_STATE_ATTACK);
		}
			
	}
	break;

	case DIK_LEFT:
	{
		//DebugOut(L"DIK_LEFT	\n");
		mario->level_of_stopping = 1;
		mario->level_of_walking = 0;
		if (mario->GetState() == MARIO_STATE_WALKING_RIGHT)
		{
			//mario->SetState(MARIO_STATE_WALKING_LEFT);
			mario->nx = -1;

		}
	}
	break;

	case DIK_RIGHT:
	{
		//DebugOut(L"DIK_RIGHT	\n");
		mario->level_of_stopping = 1;
		mario->level_of_walking = 0;
	}
	break;

	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	Mario* mario = ((CPlayScene*)scence)->GetPlayer();

	switch (KeyCode)
	{
	case DIK_A:
	{
		//if (mario->isRunning == true)
			mario->isRunning = false;
			mario->pressA = false;
		/*if (mario->isHolding == true)
			mario->isHolding = false;*/
		
		if (mario->endAttack == true)
		{
			mario->endAttack = false;
			mario->isAttacking = false;
		}
	}break;

	case DIK_S:
	{
		//DebugOut(L"Up S\n");
		// thả nút S thì rớt xuống liền
		if (mario->vy < 0)
		{
			mario->vy = 0;
			mario->isFalling = true;
			mario->OnGround = false;
		}

		mario->isFlyingHigh = mario->isFlyingLow = false;
	}break;

	case DIK_X:
	{
		//DebugOut(L"Up X\n");
		/*if (mario->isFlyingHigh == true)
		{
			mario->isFlyingHigh = false;
		}*/
		mario->isFlyingHigh = mario->isFlyingLow = false;
	}break;
		
	}

}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	Mario *mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;

	if (game->IsKeyDown(DIK_LEFT))
	{
		mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (mario->GetLevel() > 1)
		{
			mario->SetState(MARIO_STATE_SITDOWN);
		}
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
	if (game->IsKeyDown(DIK_A))
	{
		if (mario->pressA == true && (mario->GetState() == MARIO_STATE_WALKING_LEFT || mario->GetState() == MARIO_STATE_WALKING_RIGHT))
			mario->isRunning = true;
	}

	if (game->IsKeyDown(DIK_X))
	{
		/*int state = 0;
		if (mario->OnGround == true)
		{
			state = MARIO_STATE_JUMP_LOW;
			if (mario->canFlyX == true)
				state = MARIO_STATE_FLY_HIGH;
		}
		if (mario->GetLevel() == 3)
		{
			if (mario->isFalling == true)
			{
				state = MARIO_STATE_FLY_LOW;
			}
			if (mario->isMaxRunning == true)
			{
				mario->canFlyX = true;
				state = MARIO_STATE_FLY_HIGH;
			}
		}
		mario->SetState(state);*/

		if (mario->OnGround == true)
		{
			mario->SetState(MARIO_STATE_JUMP_LOW);
			if (mario->canFlyX == true)
				mario->SetState(MARIO_STATE_FLY_HIGH);
		}
		if (mario->GetLevel() == 3)
		{
			if (mario->isFalling == true)
			{
				mario->SetState(MARIO_STATE_FLY_LOW);
			}
			if (mario->isMaxRunning == true)
			{
				mario->canFlyX = true;
				mario->SetState(MARIO_STATE_FLY_HIGH);
			}
		}
	}
	/*else if (game->IsKeyDown(DIK_S))
	{
		mario->SetState(MARIO_STATE_JUMP);
		if (mario->canFlyS == true)
		{
			if (mario->isFalling == true)
			{
				mario->SetState(MARIO_STATE_FLY_LOW);
			}
			if (mario->isMaxRunning == true)
			{
				mario->SetState(MARIO_STATE_FLY_HIGH);
			}
		}
	}*/
}

