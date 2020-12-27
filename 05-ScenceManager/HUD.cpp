#include "HUD.h"
#include "Mario.h"
#include "Camera.h"
#include "Textures.h"
#include "Game.h"
#include "Goomba.h"

#define HEIGHT_SCORE_BROAD 28
#define ID_TEX_HUB 12


#define CAM_Y_HUD_ITEM	_Camera->cam_y + (SCREEN_HEIGHT * 3 / 4) - 10

HUD* HUD::__instance = NULL;


HUD* HUD::GetInstance()
{
	if (__instance == NULL)
		__instance = new HUD();
	return __instance;
}

HUD::HUD() 
{
	isDrawPush = true;
	CamX = CamY = 0.f;
	MarioLife = 4;
	Score = 0;
	PlayTime = 300;
	Money = 0;
	countPlayTime = 0;
	font = NULL;
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
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void HUD::UpdateScore(LPGAMEOBJECT e)
{
	switch (e->Category)
	{
		case CATEGORY::ITEM:
		{
			if (e->ObjType == OBJECT_TYPE_COIN)
			{
				this->Score += COIN_SCORE;
				this->Money++;
			}
			else if (e->ObjType == OBJECT_TYPE_QUESTIONBRICKITEM)
			{
				this->Score += QUESTIONBRICKITEM_SCORE;
			}
		}
		break;

		case CATEGORY::ENEMY:
		{
			if (e->ObjType == OBJECT_TYPE_GOOMBA)
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e);
				this->Score += goomba->score;
			}
			else if (e->ObjType == OBJECT_TYPE_QUESTIONBRICKITEM)
			{
				this->Score += QUESTIONBRICKITEM_SCORE;
			}
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

	CamX = _Camera->cam_x + 60;
	CamY = _Camera->cam_y + (SCREEN_HEIGHT * 3 / 4) - 5;
	// chiều dài màn hình
	int ScreenWidth = CGame::GetInstance()->GetScreenWidth();

	SetRect(&rect, CamX + 39, CamY, CamX + static_cast<float>(ScreenWidth), CamY + (SCREEN_HEIGHT * 3 / 4) - 10);

	auto sprites = CSprites::GetInstance();
	HUB = sprites->Get(90000);
	Item1 = sprites->Get(90002);
	Item2 = sprites->Get(90003);
	Item3 = sprites->Get(90004);
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

	CamX = _Camera->cam_x + 60;
	CamY = _Camera->cam_y + (SCREEN_HEIGHT * 3 / 4) - 5;
	// chiều dài màn hình
	int ScreenWidth = CGame::GetInstance()->GetScreenWidth();

	SetRect(&rect, CamX + 39, CamY, CamX + static_cast<float>(ScreenWidth), CamY + (SCREEN_HEIGHT * 3 / 4) - 10);

	auto sprites = CSprites::GetInstance();
	HUB = sprites->Get(90000);
	Item1 = sprites->Get(90002);
	Item2 = sprites->Get(90003);
	Item3 = sprites->Get(90004);
	typePlayer = sprites->Get(92001);
	speed = sprites->Get(91001);
	push = sprites->Get(91002);

	PlayTime = playtime;
}

void HUD::Update(float dt)
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
	
	NumSpeed = int(_Mario->level_of_running / (MAX_LEVEL_OF_RUNNING/7));
	information = scene;
	if(money.size() == 1)
		information += "                                  " + money + "\n";
	else if (money.size() == 2)
		information += "                                   " + money + "\n";

	information += life + "   " + scoregame + "	            " + timeString + "\n";

}

void HUD::Render()
{
	CamX = _Camera->cam_x;
	CamY = _Camera->cam_y + (SCREEN_HEIGHT * 3 / 4) - 5;
	//================ Vẽ HUD ======================
	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(12);
	// khung đen sau HUD
	_Game->Draw(CamX, CamY - 25, bbox, 0, 0, _Camera->GetWidth(), _Camera->GetHeight(), 255);
	//DebugOut(L"Camera camx %f, camera camy %f\n", _Camera->cam_x, _Camera->cam_y);
	//DebugOut(L"camx %f, camy %f\n", CamX, CamY);
	//DebugOut(L"left = %i, top = %i, right = %i, bottom = %i\n", rect.left, rect.top, rect.right, rect.bottom);

	// Thanh HUD
	CamX = CamX + 60;
	HUB->Draw(CamX, CAM_Y_HUD_ITEM);
	// 3 item hình thẻ bài
	Item1->Draw(CamX + 190, CAM_Y_HUD_ITEM);
	Item2->Draw(CamX + 215, CAM_Y_HUD_ITEM);
	Item3->Draw(CamX + 240, CAM_Y_HUD_ITEM);
	// icon số mạng mario
	typePlayer->Draw(CamX + 9, CamY + 9);
	// thanh tốc độ

	for (int i = 0; i < NumSpeed - 1; i++)
		speed->Draw(CamX + 55 + (i * 8), CamY + 1);

	if (NumSpeed > 6 && isDrawPush)
	{
		push->Draw(CamX + 57 + (6 * 8), CamY + 1);
	}

	//SetRect(&rect, CamX + 39, CamY, CamX + static_cast<float>(CGame::GetInstance()->GetScreenWidth()), CamY + (SCREEN_HEIGHT * 3 / 4) - 10);
	LPD3DXSPRITE spriteHandler = _Game->GetSpriteHandler();
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
