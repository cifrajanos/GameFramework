//-----------------------------------------------------------------------------
// File: Ball.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//       such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Ball Specific Includes
//-----------------------------------------------------------------------------
#include "Ball.h"

//-----------------------------------------------------------------------------
// Name : Ball () (Constructor)
// Desc : Ball Class Constructor
//-----------------------------------------------------------------------------
Ball::Ball()
{
	//m_pSprite = new Sprite("data/planeimg.bmp", "data/planemask.bmp");
	m_pSprite = new Sprite("data/ball.bmp", RGB(0xff,0x00, 0xff));
	m_eSpeedState = SPEED_STOP;
	m_fTimer = 0;

}

//-----------------------------------------------------------------------------
// Name : ~Ball () (Destructor)
// Desc : Ball Class Destructor
//-----------------------------------------------------------------------------
Ball::~Ball()
{
	delete m_pSprite;
}

void Ball::Init(const Vec2& position)
{
	myPosition = position;
}

void Ball::Update(float dt)
{
	CGameObject::Update(dt);

	// Update sprites
	m_pSprite->myPosition = myPosition;

	// Get velocity
	double v = myVelocity.Magnitude();

}

void Ball::Draw() const
{
	m_pSprite->Draw();
	CGameObject::Draw();
}

void Ball::Move(ULONG ulDirection, bool FollowPlayer)
{
	if(FollowPlayer == true){
		myAcceleration = Vec2();
		myVelocity=Vec2();
	}

	if( ulDirection & Ball::DIR_START ){
		myVelocity.y = -200;
		myVelocity.x = -200;
	}

	if(FollowPlayer == true){

		if( ulDirection & Ball::DIR_LEFT )
		{
			myVelocity.x = -300;
		}

		if( ulDirection & Ball::DIR_RIGHT )
		{
			myVelocity.x = 300;
			myVelocity.y = 0;
		}
	}
}

void Ball::Increase_Speed()
{
	myVelocity.x = myVelocity.x / 2 * 2.5;
	myVelocity.y = myVelocity.x / 2 * 2.5;
}

void Ball::Decrease_Speed()
{
	myVelocity.x = myVelocity.x / 2.5 * 2;
	myVelocity.y = myVelocity.x / 2.5 * 2;
}