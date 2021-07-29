#include "HUD.h"
#include "Mario.h"
#include "Camera.h"
#include "Game.h"

#include "Goomba.h"
#include "FirePiranhaPlant.h"
#include "EffectScore.h"
#include "QuestionBrickItem.h"
#include "Card.h"
#include "BrickItem.h"

HUD* HUD::__instance = NULL;

HUD* HUD::GetInstance()
{
	if (__instance == NULL)
		__instance = new HUD();
	return __instance;
	
}

HUD::HUD() 
{
	HUB = Item1 = Item2 = Item3 = push = NULL;
	speed = typePlayer = NULL;
	NumSpeed = 0;
	isDrawPush = true;
	CamX = CamY = 0.f;
	MarioLife = 4;
	Score = 0;
	PlayTime = 300;
	Money = 0;
	countPlayTime = 0;
	font = NULL;
	hasItem1 = hasItem2 = hasItem3 = false;
	TypeItem[0] = TypeItem[1] = TypeItem[2] = 0;
}

void HUD::_ParseSection_TEXTURES(string line)
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

void HUD::_ParseSection_SPRITES(string line)
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

void HUD::_ParseSection_ANIMATIONS(string line)
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

void HUD::UpdateScore(LPGAMEOBJECT e, int _nScore)
{
	switch (e->Category)
	{
		case CATEGORY::ITEM:
		{
			int score = 0;
			Item* item = dynamic_cast<Item*>(e);
			switch (item->TypeItem)
			{
				case ITEM_TYPE_QUESTIONBRICKITEM:
				{
					QuestionBrickItem* qbitem = dynamic_cast<QuestionBrickItem*>(item);
					score = qbitem->Score;
					this->Score += qbitem->Score;
					if (qbitem->Item != MONEY)
					{
						EffectScore* effectscore = new EffectScore(_Mario->x - 2, _Mario->y - 20, score);
						_Grid->AddMovingObject(effectscore, _Mario->x - 2, _Mario->y - 20);
					}
				}
				break;

				case ITEM_TYPE_BRICKITEM:
				{
					BrickItem* bitem = dynamic_cast<BrickItem*>(item);
					switch (bitem->Item)
					{
						case MUSHROOM:
						{
							this->MarioLife++;
							EffectScore* effectscore = new EffectScore(_Mario->x, _Mario->y - 10, 10000);
							_Grid->AddMovingObject(effectscore, _Mario->x, _Mario->y - 10);
						}
					break;
						case MONEY:
						{
							score = item->Score;
							this->Score += item->Score;
							this->Money++;
						}
					}
					
				}
				break;

				case ITEM_TYPE_COIN:
				{
					score = item->Score;
					this->Score += item->Score;
					this->Money++;
				}
				break;
			}
		}
		break;

		case CATEGORY::ENEMY:
		{
			int score = 0;
			Enemy* enemy = dynamic_cast<Enemy*>(e);
			if (_nScore == 9)
			{
				score = enemy->Score * 100;
				this->MarioLife++;
			}
			else
			{
				switch (_nScore)
				{
				case 1:
					score = enemy->Score * 1;
					break;

				case 2:
					score = enemy->Score * 2;
					break;

				case 3:
					score = enemy->Score * 4;
					break;

				case 4:
					score = enemy->Score * 8;
					break;

				case 5:
					score = enemy->Score * 10;
					break;

				case 6:
					score = enemy->Score * 20;
					break;

				case 7:
					score = enemy->Score * 40;
					break;

				case 8:
					score = enemy->Score * 80;
					break;
				}
				this->Score += score;
			}
			
			EffectScore* effectscore = new EffectScore(enemy->x, enemy->y - 10, score);
			_Grid->AddMovingObject(effectscore, enemy->x, enemy->y - 10);
		}
		break;
	
	default:
		break;
	}
	
}


void HUD::LoadHUD(wstring map_txt)
{
	ifstream f;
	f.open(map_txt);
	// current resource section flag
	int section = HUD_SECTION_UNKNOWN;

	char str[MAX_HUD_LINE];
	while (f.getline(str, MAX_HUD_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") {
			section = HUD_SECTION_TEXTURES; continue;
		}

		if (line == "[SPRITES]") {
			section = HUD_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = HUD_SECTION_ANIMATIONS; continue;
		}
		
		if (line[0] == '[') { section = HUD_SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case HUD_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case HUD_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case HUD_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}
	f.close();
}

void HUD::Init()
{
	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 buffer = CGame::GetInstance()->GetBackBuffer();
	AddFontResourceEx(L".\\font\\Super-Mario-Bros--3.ttf", FR_PRIVATE, NULL);
	HRESULT result = D3DXCreateFont(
		d3ddv,		// d3d divice
		9,			//	Height
		0,			//	Width
		FW_NORMAL,	//	Weight
		1,			//MipLevels
		false,		//Italic,
		DEFAULT_CHARSET, //CharSet
		OUT_CHARACTER_PRECIS, //OutputPrecision
		ANTIALIASED_QUALITY, // Quality
		FF_DONTCARE, // PitchAndFamily,
		L"Super Mario Bros. 3",
		&font);

	CamX = float(_Camera->cam_x + (SCREEN_WIDTH / 30));
	CamY = _Camera->cam_y + (SCREEN_HEIGHT * 3 / 4) - 5;
	// chiều dài màn hình
	int ScreenWidth = CGame::GetInstance()->GetScreenWidth();
	//SetRect(&rect, CamX + (SCREEN_WIDTH / 10) + 10, CamY, CamX + static_cast<float>(ScreenWidth), CamY + (SCREEN_HEIGHT * 3 / 4) - 10);
	SetRect(&rect, (int)(CamX + 39), (int)CamY, (int)(CamX + static_cast<float>(ScreenWidth)), (int)(CamY + (SCREEN_HEIGHT * 3 / 4) - 10));

	auto sprites = CSprites::GetInstance();
	HUB = sprites->Get(90000);
	Item1 = sprites->Get(90001);
	Item2 = sprites->Get(90001);
	Item3 = sprites->Get(90001);
	typePlayer = sprites->Get(92001);
	speed = sprites->Get(91001);
	push = sprites->Get(91002);
}

void HUD::Init(int playtime)
{
	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 buffer = CGame::GetInstance()->GetBackBuffer();
	AddFontResourceEx(L".\\font\\Super-Mario-Bros--3.ttf", FR_PRIVATE, NULL);
	HRESULT result = D3DXCreateFont(
		d3ddv,		// d3d divice
		9,			//	Height
		0,			//	Width
		FW_NORMAL,	//	Weight
		1,			//MipLevels
		false,		//Italic,
		DEFAULT_CHARSET, //CharSet
		OUT_CHARACTER_PRECIS, //OutputPrecision
		ANTIALIASED_QUALITY, // Quality
		FF_DONTCARE, // PitchAndFamily,
		L"Super Mario Bros. 3",
		&font);

	CamX = float(_Camera->cam_x + (SCREEN_WIDTH/30) );
	CamY = _Camera->cam_y + (SCREEN_HEIGHT * 3 / 4) - 5;
	// chiều dài màn hình
	int ScreenWidth = CGame::GetInstance()->GetScreenWidth();
	//SetRect(&rect, CamX + (SCREEN_WIDTH / 10) + 10, CamY, CamX + static_cast<float>(ScreenWidth), CamY + (SCREEN_HEIGHT * 3 / 4) - 10);
	SetRect(&rect, (int)(CamX + 39), (int)CamY, (int)(CamX + static_cast<float>(ScreenWidth)), (int)(CamY + (SCREEN_HEIGHT * 3 / 4) - 10));

	auto sprites = CSprites::GetInstance();
	HUB = sprites->Get(90000);
	Item1 = sprites->Get(90001);
	Item2 = sprites->Get(90001);
	Item3 = sprites->Get(90001);
	typePlayer = sprites->Get(92001);
	speed = sprites->Get(91001);
	push = sprites->Get(91002);

	PlayTime = playtime;

}

void HUD::Update(DWORD dt)
{
	string scoregame = to_string(this->Score);
	while (scoregame.length() < 7)
		scoregame = "0" + scoregame;
	if (PlayTime > 0 && GetTickCount64() - countPlayTime > 1000 )
	{
		PlayTime--;
		countPlayTime = GetTickCount64();
	}
	string timeString = to_string(this->PlayTime);
	while (timeString.length() < 3)
		timeString = "0" + timeString;
	string life = to_string(this->MarioLife);
	string money = to_string(this->Money);
	/*while (money.length() < 2)
		money = "  " + money;*/

	string scene = to_string(_Game->current_scene / 10);

	float vx, vy;
	_Mario->GetSpeed(vx, vy);
	
	// k cần đặt dk isrunning = true vì nó mặc định xét theo speed
	if (_Mario->canFlyX == false && _Mario->canFlyS == false)
		NumSpeed = (INT16)(_Mario->level_of_running / (MAX_LEVEL_OF_RUNNING / 7));
	else
		NumSpeed = 7;
	

	information = scene;
	if(money.size() == 1)
		information += "                                  " + money + "\n";
	else if (money.size() == 2)
		information += "                                   " + money + "\n";

	information += life + "   " + scoregame + "	             " + timeString + "\n";

	if (TypeItem[0] == 0)
	{
		Item1 = CSprites::GetInstance()->Get(90001);
		Item2 = CSprites::GetInstance()->Get(90001);
		Item3 = CSprites::GetInstance()->Get(90001);
		TypeItem[0] = TypeItem[1] = TypeItem[2] = 0;
	}
	else
	{
		switch (TypeItem[0])
		{
		case CARD_MUSHROOM:
			Item1 = CSprites::GetInstance()->Get(90002); break;
		case CARD_FLOWER:
			Item1 = CSprites::GetInstance()->Get(90003); break;
		case  CARD_STAR:
			Item1 = CSprites::GetInstance()->Get(90004); break;
		}

		if (TypeItem[1] == 0)
		{
			Item2 = CSprites::GetInstance()->Get(90001);
			Item3 = CSprites::GetInstance()->Get(90001);
		}
		else
		{
			switch (TypeItem[1])
			{
			case CARD_MUSHROOM:
				Item2 = CSprites::GetInstance()->Get(90002); break;
			case CARD_FLOWER:
				Item2 = CSprites::GetInstance()->Get(90003); break;
			case  CARD_STAR:
				Item2 = CSprites::GetInstance()->Get(90004); break;
			}
			if (TypeItem[2] == 0)
			{
				Item3 = CSprites::GetInstance()->Get(90001);
			}
			else
			{
				switch (TypeItem[1])
				{
				case CARD_MUSHROOM:
					Item3 = CSprites::GetInstance()->Get(90002); break;
				case CARD_FLOWER:
					Item3 = CSprites::GetInstance()->Get(90003); break;
				case  CARD_STAR:
					Item3 = CSprites::GetInstance()->Get(90004); break;
				}
			}
		}

	}
	
	int ScreenWidth = CGame::GetInstance()->GetScreenWidth();
}

void HUD::Render()
{
	CamX = _Game->GetCamX();
	CamY = _Game->GetCamY() + (SCREEN_HEIGHT * 3 / 4) - 5; // 458
	float camx = _Camera->cam_x;
	float camy = _Camera->cam_y + _Game->GetScreenHeight() - HUD_HEIGHT;; // 431 : chuẩn
	//================ Vẽ HUD ======================
	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_HUB);
	// khung đen sau HUD
	_Game->DrawHud(0, 174, bbox, 0, 0, _Camera->GetWidth(), HUD_HEIGHT, 255);
	float tempy = _Mario->y; // 198
	int a = int(CamX); // 311
	int b = int(roundf(camy)) + 1; //342
	//_Game->Draw(a, b, bbox, 0, 0, _Camera->GetWidth(), HUD_HEIGHT, 255);
	//DebugOut(L"CamX = %f , camy = %f\n", CamX, camy);

	// Thanh HUD
	CamX = CamX + SCREEN_WIDTH / 30;
	HUB->DrawHud(10, 185);
	// 3 item hình thẻ bài

	/*Item1->Draw(CamX + 190, CAM_Y_HUD_ITEM1);
	Item2->Draw(CamX + 215, CAM_Y_HUD_ITEM1);
	Item3->Draw(CamX + 240, CAM_Y_HUD_ITEM1);*/
	
	Item1->DrawHud(200, 184);
	Item2->DrawHud(224, 184);
	Item3->DrawHud(248, 184);

	// icon số mạng mario
	typePlayer->DrawHud(16, 199);

	if (_Mario->StopRunning == false)
	{
		// thanh tốc độ
		for (int i = 0; i <= NumSpeed - 1; i++)
		{
			// đạt max : numspeed = 7 thì khi i = 6 vẽ push chứ k vẽ speed
			if (i == 6)
				push->DrawHud(67 + (6 * 8), 191);
			else
				speed->DrawHud(float(66 + (i * 8)), 191);
		}
	}
	

	LPD3DXSPRITE spriteHandler = _Game->GetSpriteHandler();
	int aa = _Game->current_scene;
	if (aa == 1)
		SetRect(&rect, (int)(_Game->GetCamX() + 80), (int)(CamY + 15), (int)(CamX + static_cast<float>(CGame::GetInstance()->GetScreenWidth())), (int)(CamY + (SCREEN_HEIGHT * 3 / 4) - 10));
	if (font)
		font->DrawTextA(spriteHandler, information.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

}

void HUD::Unload()
{
	__instance = NULL;
}

HUD::~HUD()
{
}

void HUD::UpdateItem(int _type)
{
	if (TypeItem[0] == 0)
		TypeItem[0] = _type;
	else if (TypeItem[1] == 0)
		TypeItem[1] = _type;
	else if (TypeItem[2] == 0)
		TypeItem[2] = _type;
}

