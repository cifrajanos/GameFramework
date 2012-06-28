//-----------------------------------------------------------------------------
// File: Brick.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//       such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brick Specific Includes
//-----------------------------------------------------------------------------
#include "Brick.h"

//-----------------------------------------------------------------------------
// Name : Brick () (Constructor)
// Desc : Brick Class Constructor
//-----------------------------------------------------------------------------
Brick::Brick(char type)
{
	DoubleCollision = 2;
	BrickType = type;

	//m_pSprite = new Sprite("data/planeimg.bmp", "data/planemask.bmp");
	switch(BrickType)
	{
	case '1': m_pSprite = new Sprite("data/brick.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'u': m_pSprite = new Sprite("data/bricku.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'd': m_pSprite = new Sprite("data/brickd.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'a': m_pSprite = new Sprite("data/brick.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'b': m_pSprite = new Sprite("data/brick.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'c': m_pSprite = new Sprite("data/brick.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'e': m_pSprite = new Sprite("data/brick.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'f': m_pSprite = new Sprite("data/brick.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'k': m_pSprite = new Sprite("data/brick.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'g': m_pSprite = new Sprite("data/brick.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'h': m_pSprite = new Sprite("data/brick.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'i': m_pSprite = new Sprite("data/brick.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'j': m_pSprite = new Sprite("data/brick.bmp", RGB(0xff,0x00, 0xff)); break;
	case 'm': m_pSprite = new Sprite("data/brick.bmp", RGB(0xff,0x00, 0xff)); break;
	}

	m_eSpeedState = SPEED_STOP;
	m_fTimer = 0;

	m_pAnimateSprite	= new AnimatedSprite("data/brickanimate.bmp", "data/brickanimatemask.bmp");
	m_bAnimate		= false;

}

//-----------------------------------------------------------------------------
// Name : ~Brick () (Destructor)
// Desc : Brick Class Destructor
//-----------------------------------------------------------------------------
Brick::~Brick()
{
	delete m_pSprite;
	delete m_pAnimateSprite;
}

void Brick::Init(const Vec2& position)
{
	myPosition = position;

	// Animation frame crop rectangle
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = 50;
	r.bottom = 25;
	m_pAnimateSprite->Initialize(r, 5, 1 / 16.f);
}

void Brick::Update(float dt)
{
	CGameObject::Update(dt);

	// Update sprites
	m_pSprite->myPosition = myPosition;
	m_pAnimateSprite->Update(dt);

	// Get velocity
	double v = myVelocity.Magnitude();

	// handle brick animation
	if(m_bAnimate && !m_pAnimateSprite->IsPlaying())
	{
		m_bAnimate = false;
		myVelocity = Vec2(0,0);
		m_eSpeedState = SPEED_STOP;
	}

}

void Brick::Draw() const
{
	if(!m_bAnimate)
		m_pSprite->Draw();
	else
		m_pAnimateSprite->Draw();

	CGameObject::Draw();
}

void Brick::Animate()
{
	m_pAnimateSprite->myPosition = m_pSprite->myPosition;
	m_pAnimateSprite->Play();
	m_bAnimate = true;
}