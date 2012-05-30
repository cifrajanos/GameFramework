//-----------------------------------------------------------------------------
// File: Gift.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//       such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Gift Specific Includes
//-----------------------------------------------------------------------------
#include "Gift.h"

//-----------------------------------------------------------------------------
// Name : Gift () (Constructor)
// Desc : Gift Class Constructor
//-----------------------------------------------------------------------------
Gift::Gift(char type)
{
	GiftType = type;

	//m_pSprite = new Sprite("data/planeimg.bmp", "data/planemask.bmp");
	switch(GiftType)
	{
	case 'a': m_pSprite = new Sprite("data/gift.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'b': m_pSprite = new Sprite("data/gift.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'c': m_pSprite = new Sprite("data/gift.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'e': m_pSprite = new Sprite("data/gift.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'f': m_pSprite = new Sprite("data/gift.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'g': m_pSprite = new Sprite("data/gift.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'h': m_pSprite = new Sprite("data/gift.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'i': m_pSprite = new Sprite("data/gift.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'j': m_pSprite = new Sprite("data/gift.bmp", RGB(0xff,0x00, 0xff)); break;
	}
	m_eSpeedState = SPEED_STOP;
	m_fTimer = 0;

	Move(Gift::DIR_BACKWARD);
}

//-----------------------------------------------------------------------------
// Name : ~Gift () (Destructor)
// Desc : Gift Class Destructor
//-----------------------------------------------------------------------------
Gift::~Gift()
{
	delete m_pSprite;
}

void Gift::Init(const Vec2& position)
{
	myPosition = position;

}

void Gift::Update(float dt)
{
	CGameObject::Update(dt);

	// Update sprites
	m_pSprite->myPosition = myPosition;

	// Get velocity
	double v = myVelocity.Magnitude();

}

void Gift::Draw() const
{
	m_pSprite->Draw();
	CGameObject::Draw();
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
