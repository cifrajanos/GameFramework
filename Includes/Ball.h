//-----------------------------------------------------------------------------
// File: Ball.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//	   such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

#ifndef _BALL_H_
#define _BALL_H_

//-----------------------------------------------------------------------------
// Ball Specific Includes
//-----------------------------------------------------------------------------
#include "Main.h"
#include "Sprite.h"
#include "AnimatedSprite.h"

//-----------------------------------------------------------------------------
// Main Class Definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : Ball (Class)
// Desc : Player class handles all player manipulation, update and management.
//-----------------------------------------------------------------------------
class Ball : public CGameObject
{
public:
	//-------------------------------------------------------------------------
	// Enumerators
	//-------------------------------------------------------------------------
	enum DIRECTION 
	{ 
		DIR_FORWARD     = 1, 
		DIR_BACKWARD    = 2, 
		DIR_LEFT        = 4, 
		DIR_RIGHT       = 8, 
		DIR_START		= 10,
	};

	enum ESpeedStates
	{
		SPEED_START,
		SPEED_STOP
	};

	//-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
	Ball();
	virtual ~Ball();

	//-------------------------------------------------------------------------
	// Public Functions for This Class.
	//-------------------------------------------------------------------------
	void            Init(const Vec2& position);
	void            Update( float dt );
	GameObjectType  GetObjectType() const { return GOT_Ball; }
	void            Draw() const;
	void            Move(ULONG ulDirection, bool FollowPlayer);
	virtual int	    GetWidth() const { return m_pSprite->GetWidth(); }
	virtual int     GetHeight() const { return m_pSprite->GetHeight(); }
	void			Increase_Speed();
	void			Decrease_Speed();

private:
	//-------------------------------------------------------------------------
	// Private Variables for This Class.
	//-------------------------------------------------------------------------
	Sprite*         m_pSprite;
	ESpeedStates    m_eSpeedState;
	float           m_fTimer;

};

#endif // _BALL_H_