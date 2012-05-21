//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//       such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CPlayer Specific Includes
//-----------------------------------------------------------------------------
#include "Gift.h"

//-----------------------------------------------------------------------------
// Name : CPlayer () (Constructor)
// Desc : CPlayer Class Constructor
//-----------------------------------------------------------------------------
Gift::Gift(char type)
{
	DoubleCollision=2;
	GiftType=type;
	//m_pSprite = new Sprite("data/planeimg.bmp", "data/planemask.bmp");
	switch(GiftType)
	{
		case 'a': m_pSprite = new Sprite("data/gift.bmp", RGB(0xff,0x00, 0xff)); break;
		case 'b': m_pSprite = new Sprite("data/gift.bmp", RGB(0xff,0x00, 0xff)); break;
		case 'c': m_pSprite = new Sprite("data/gift.bmp", RGB(0xff,0x00, 0xff)); break;
		case 'e': m_pSprite = new Sprite("data/gift.bmp", RGB(0xff,0x00, 0xff)); break;
		case 'f': m_pSprite = new Sprite("data/gift.bmp", RGB(0xff,0x00, 0xff)); break;
	}
	
	m_eSpeedState = SPEED_STOP;
	m_fTimer = 0;

	Move(Gift::DIR_BACKWARD);

}

//-----------------------------------------------------------------------------
// Name : ~CPlayer () (Destructor)
// Desc : CPlayer Class Destructor
//-----------------------------------------------------------------------------
Gift::~Gift()
{
	delete m_pSprite;
}

void Gift::Init(HDC hdc, const Vec2& position)
{
	//m_pSprite->Setup(hdc);
	m_pSprite->Initialize(hdc);
	myPosition = position;
}

void Gift::Update(float dt)
{
	CGameObject::Update(dt);

	// Update sprites
	m_pSprite->myPosition = myPosition;


	
	// Get velocity
	double v = myVelocity.Magnitude();

	// NOTE: for each async sound played Windows creates a thread for you
	// but only one, so you cannot play multiple sounds at once.
	// This creation/destruction of threads also leads to bad performance
	// so this method is not recommended to be used in complex projects.

	// update internal time counter used in sound handling (not to overlap sounds)
	m_fTimer += dt;

}

void Gift::Draw(HDC hdc) const
{
	m_pSprite->Draw(hdc);
}

void Gift::Move(ULONG ulDirection)
{
	myAcceleration = Vec2();
	myVelocity=Vec2();

	if( ulDirection & Gift::DIR_BACKWARD )
	{
		myVelocity.x = 0;
		myVelocity.y = 250;
	}

}