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
#include "Brick.h"

//-----------------------------------------------------------------------------
// Name : CPlayer () (Constructor)
// Desc : CPlayer Class Constructor
//-----------------------------------------------------------------------------
Brick::Brick()
{
	//m_pSprite = new Sprite("data/planeimg.bmp", "data/planemask.bmp");
	m_pSprite = new Sprite("data/brick.bmp", RGB(0xff,0x00, 0xff));
	m_eSpeedState = SPEED_STOP;
	m_fTimer = 0;


}

//-----------------------------------------------------------------------------
// Name : ~CPlayer () (Destructor)
// Desc : CPlayer Class Destructor
//-----------------------------------------------------------------------------
Brick::~Brick()
{
	delete m_pSprite;
}

void Brick::Init(HDC hdc, const Vec2& position)
{
	//m_pSprite->Setup(hdc);
	m_pSprite->Initialize(hdc);
	myPosition = position;
}

void Brick::Update(float dt)
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

void Brick::Draw(HDC hdc) const
{
	m_pSprite->Draw(hdc);
}

void Brick::Move(ULONG ulDirection)
{
	myAcceleration = Vec2();
	myVelocity=Vec2();

	if( ulDirection & Brick::DIR_LEFT )
		myVelocity.x = -150;

	if( ulDirection & Brick::DIR_RIGHT )
		myVelocity.x = 150;

}