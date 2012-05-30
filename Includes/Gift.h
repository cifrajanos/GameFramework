//-----------------------------------------------------------------------------
// File: Gift.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//	   such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

#ifndef _GIFT_H_
#define _GIFT_H_

//-----------------------------------------------------------------------------
// Gift Specific Includes
//-----------------------------------------------------------------------------
#include "Main.h"
#include "Sprite.h"
#include "AnimatedSprite.h"

//-----------------------------------------------------------------------------
// Main Class Definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : Gift (Class)
// Desc : Player class handles all player manipulation, update and management.
//-----------------------------------------------------------------------------
class Gift : public CGameObject
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
	};

	enum ESpeedStates
	{
		SPEED_START,
		SPEED_STOP
	};

	//-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
	Gift(char type);
	virtual ~Gift();

	//-------------------------------------------------------------------------
	// Public Functions for This Class.
	//-------------------------------------------------------------------------
	void            Init(const Vec2& position);
	void            Update( float dt );
	GameObjectType  GetObjectType() const { return GOT_Brick; }
	GameObjectTypeSub	GetObjectTypeSub() const 
	{ 	
		switch(GiftType)
		{
		case 'a': return GOT_Gift100; break;
		case 'b': return GOT_Gift200; break;
		case 'c': return GOT_GiftUpLife; break;
		case 'e': return GOT_GiftDownLife; break;
		case 'f': return GOT_GiftUnstoppableBall; break;
		case 'g': return GOT_GiftIncreaseSpeed; break;
		case 'h': return GOT_GiftDecreaseSpeed; break;
		case 'i': return GOT_GiftStickyBar; break;
		case 'j': return GOT_GiftShrinkBar; break;
		}
		return GOT_InvalidSub;
	}
	void            Draw() const;
	void            Move(ULONG ulDirection);
	virtual int	    GetWidth() const { return m_pSprite->GetWidth(); }
	virtual int     GetHeight() const { return m_pSprite->GetHeight(); }

private:
	//-------------------------------------------------------------------------
	// Private Variables for This Class.
	//-------------------------------------------------------------------------
	Sprite*         m_pSprite;
	ESpeedStates    m_eSpeedState;
	float           m_fTimer;

	char			GiftType;
};

#endif // _Gift_H_