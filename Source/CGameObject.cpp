#include "CGameObject.h"
#include "CGameApp.h"

CGameObject::CGameObject() 
	: myCollisionSide(CS_None)
{
	mySpeedPen = CreatePen(PS_SOLID, 1, RGB(0xff, 0x00, 0x00));
	myAccelerationPen = CreatePen(PS_SOLID, 1, RGB(0x00, 0xff, 0x00));
}

CGameObject::~CGameObject()
{
	DeleteObject(mySpeedPen);
	DeleteObject(myAccelerationPen);
}

void CGameObject::Update(float dt)
{

	myCollisionSide = CS_None;

	myPosition += myVelocity * dt;

}

void CGameObject::Draw() const
{ 
	HDC hdc = CGameApp::Get()->GetBackBufferDC();
	HGDIOBJ old_obj = SelectObject(hdc, mySpeedPen);

	MoveToEx(hdc, myPosition.x, myPosition.y, NULL);
	LineTo(hdc, myPosition.x + myVelocity.x, myPosition.y + myVelocity.y);

	SelectObject(hdc, myAccelerationPen);
	MoveToEx(hdc, myPosition.x, myPosition.y, NULL);
	LineTo(hdc, myPosition.x + myResultant.x, myPosition.y + myResultant.y);

	SelectObject(hdc, old_obj);
}