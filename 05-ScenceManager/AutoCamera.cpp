//#include "AutoCamera.h"
//#include "Mario.h"
//#include "Camera.h"	
//
//AutoCamera::AutoCamera()
//{
//	if (_Mario->nx > 0) {
//		stateName = WALKING_RIGHT;
//	}
//	else {
//		stateName = WALKING_LEFT;
//	}
//	if (_Mario->isSitting) {
//		if (player->nx > 0) {
//			stateName = SITTING_RIGHT;
//		}
//		else {
//			stateName = SITTING_LEFT;
//		}
//	}
//}
//
//void AutoCamera::Update(DWORD dt)
//{
//	this->HandleKeyBoard();
//	if (_Mario->x < _Camera->cam_x) 
//	{
//		_Mario->x = _Camera->cam_x;
//	}
//}
//
//void AutoCamera::Render()
//{
//}
//
//void AutoCamera::HandleKeyBoard()
//{
//	if (keyCode[DIK_RIGHT]) {
//		player->nx = 1;
//		player->ChangeState(new PlayerWalkingState());
//	}
//	else if (keyCode[DIK_LEFT]) {
//		player->nx = -1;
//		player->ChangeState(new PlayerWalkingState());
//	}
//	else if (keyCode[DIK_S]) {
//		player->ChangeState(new PlayerJumpingState());
//	}
//	else if (keyCode[DIK_DOWN] && player->Allow[SITTING]) {
//		player->ChangeState(new PlayerSittingState());
//	}
//}
