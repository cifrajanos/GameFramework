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
#include "CPlayer.h"

//-----------------------------------------------------------------------------
// Name : CPlayer () (Constructor)
// Desc : CPlayer Class Constructor
//-----------------------------------------------------------------------------
CPlayer::CPlayer()
{
	//m_pSprite = new Sprite("data/planeimg.bmp", "data/planemask.bmp");
	m_pSprite = new Sprite("data/bar.bmp", RGB(0xff,0x00, 0xff));
	m_eSpeedState = SPEED_STOP;
	m_fTimer = 0;


	m_pExplosionSprite	= new AnimatedSprite("data/explosion.bmp", "data/explosionmask.bmp");
	m_bExplosion		= false;
}

//-----------------------------------------------------------------------------
// Name : ~CPlayer () (Destructor)
// Desc : CPlayer Class Destructor
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
	delete m_pSprite;
}

void CPlayer::Init(HDC hdc, const Vec2& position)
{

	//m_pSprite->Setup(hdc);
	m_pSprite->Initialize(hdc);
	myPosition = position;


    // Animation frame crop rectangle
    RECT r;
    r.left = 0;
    r.top = 0;
    r.right = 128;
    r.bottom = 128;
	m_pExplosionSprite->Initialize(hdc, r, 16, 1 / 16.f);

}

void CPlayer::Update(float dt)
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

	// handle plane explosion
	if(m_bExplosion && !m_pExplosionSprite->IsPlaying())
	{
		m_bExplosion = false;
		myVelocity = Vec2(0,0);
		m_eSpeedState = SPEED_STOP;
	}


}

void CPlayer::Draw(HDC hdc) const
{
	//m_pSprite->Draw(hdc);

    if(!m_bExplosion)
		m_pSprite->Draw(hdc);
	else
		m_pExplosionSprite->Draw(hdc);

    CGameObject::Draw(hdc);
}

void CPlayer::Move(ULONG ulDirection)
{
	myAcceleration = Vec2();
	myVelocity=Vec2();

	if( ulDirection & CPlayer::DIR_LEFT )
		myVelocity.x = -150;

	if( ulDirection & CPlayer::DIR_RIGHT )
		myVelocity.x = 150;

}
