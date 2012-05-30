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

	m_pAnimateSprite	= new AnimatedSprite("data/baranimate.bmp", "data/baranimatemask.bmp");
	m_bAnimate		= false;
}

//-----------------------------------------------------------------------------
// Name : ~CPlayer () (Destructor)
// Desc : CPlayer Class Destructor
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
	delete m_pSprite;
	delete m_pAnimateSprite;
}

void CPlayer::Init(const Vec2& position)
{
	myPosition = position;

	// Animation frame crop rectangle
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = 132;
	r.bottom = 33;
	m_pAnimateSprite->Initialize(r, 3, 1 / 16.f);
}

void CPlayer::Update(float dt)
{
	CGameObject::Update(dt);

	// Update sprites
	m_pSprite->myPosition = myPosition;
	m_pAnimateSprite->Update(dt);

	// Get velocity
	double v = myVelocity.Magnitude();

	// handle player animation
	if(m_bAnimate && !m_pAnimateSprite->IsPlaying())
	{
		m_bAnimate = false;
		myVelocity = Vec2(0,0);
		m_eSpeedState = SPEED_STOP;
	}

}

void CPlayer::Draw() const
{
	if(!m_bAnimate)
		m_pSprite->Draw();
	else
		m_pAnimateSprite->Draw();

	CGameObject::Draw();
}

void CPlayer::Move(ULONG ulDirection)
{
	myAcceleration = Vec2();
	myVelocity=Vec2();

	if( ulDirection & CPlayer::DIR_LEFT )
		myVelocity.x = -300;

	if( ulDirection & CPlayer::DIR_RIGHT )
		myVelocity.x = 300;
}

void CPlayer::Animate()
{
	m_pAnimateSprite->myPosition = m_pSprite->myPosition;
	m_pAnimateSprite->Play();
	//PlaySound("data/explosion.wav", NULL, SND_FILENAME | SND_ASYNC);
	m_bAnimate = true;
}

void CPlayer::Shrink_Bar()
{
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = 66;
	r.bottom = 33;

	delete m_pSprite;
	delete m_pAnimateSprite;
	m_pSprite = new Sprite("data/barshrink.bmp", RGB(0xff,0x00, 0xff));
	m_pAnimateSprite = new AnimatedSprite("data/barshrinkanimate.bmp", "data/barshrinkanimatemask.bmp");
	m_pAnimateSprite->Initialize(r, 3, 1 / 16.f);
}

void CPlayer::Normal_Bar()
{
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = 132;
	r.bottom = 33;

	delete m_pSprite;
	delete m_pAnimateSprite;
	m_pSprite = new Sprite("data/bar.bmp", RGB(0xff,0x00, 0xff));
	m_pAnimateSprite = new AnimatedSprite("data/baranimate.bmp", "data/baranimatemask.bmp");
	m_pAnimateSprite->Initialize(r, 3, 1 / 16.f);
}