#include "CGameObject.h"

CGameObject::CGameObject() 
    : myCollisionSide(CS_None)
{
    mySpeedPen = CreatePen(PS_SOLID, 1, RGB(0xff, 0x00, 0x00));
}

CGameObject::~CGameObject()
{
    DeleteObject(mySpeedPen);
    DeleteObject(myAccelerationPen);
}

void CGameObject::Update(float dt)
{
    if( (myCollisionSide & CS_Left) || (myCollisionSide & CS_Right) )
    {
        myVelocity.x = -myVelocity.x;
    }

    if( (myCollisionSide & CS_Top) || (myCollisionSide & CS_Bottom) )
    {
        myVelocity.y = -myVelocity.y;
    }
	
	myCollisionSide = CS_None;

	// Update the sprites position and speed
	myPosition += myVelocity * dt;
}

void CGameObject::Draw(HDC hdc) const
{ 
    HGDIOBJ old_obj = SelectObject(hdc, mySpeedPen);

    MoveToEx(hdc, myPosition.x, myPosition.y, NULL);
    LineTo(hdc, myPosition.x + myVelocity.x, myPosition.y + myVelocity.y);
   
    SelectObject(hdc, old_obj);
}

