#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Sprites.h"
#include "Animations.h"
#include "Utils.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

#define LEFT -1
#define RIGHT 1

class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};

enum class CATEGORY
{
	PLAYER,
	ENEMY,
	WEAPON,
	ITEM,
	GROUND,
	OBJECT,
	EFFECT,
	PORTAL
};

class CGameObject
{
public:
	float x; 
	float y;
	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt
	float vx;
	float vy;
	int nx;	 
	int state;
	int TimeToDie;
	bool isDie; // chết nhưng chưa xóa, vẫn còn render
	bool isDisappear; // có thể xóa sau khi chết, ví dụ rùa đã chết nhưng sau khi rớt xuống đất mới xóa
	bool canDelete; // có thể xóa sau khi chết, ví dụ rùa đã chết nhưng sau khi rớt xuống đất mới xóa
	bool isInit;
	DWORD dt; 
	LPANIMATION_SET animation_set;
	int ObjType;
	CATEGORY Category;
	float Width, Height;
	float StartX, StartY;
public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y;}
	void SetStartPosition(float x, float y) { this->StartX = x, this->StartY = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	int GetState() { return this->state; }

	void RenderBoundingBox();

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny, 
		float &rdx, 
		float &rdy);
	bool IsCollision(RECT rect1, RECT rect2);
	RECT GetRect();
	CGameObject();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	virtual void CollisionWithEnemy(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny) {};
	virtual void CollisionWithObject(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny) {};
	virtual void CollisionWithItem(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny) {};
	virtual void CollisionWithWeapon(LPCOLLISIONEVENT e, float min_tx, float min_ty, float nx, float ny) {};

	~CGameObject();
};

