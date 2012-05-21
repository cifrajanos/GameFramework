//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//	   such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

#ifndef _BRICK_H_
#define _BRICK_H_

//-----------------------------------------------------------------------------
// CPlayer Specific Includes
//-----------------------------------------------------------------------------
#include "Main.h"
#include "Sprite.h"
#include "AnimatedSprite.h"

//-----------------------------------------------------------------------------
// Main Class Definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : CPlayer (Class)
// Desc : Player class handles all player manipulation, update and management.
//-----------------------------------------------------------------------------
class Brick : public CGameObject
{
public:
	//-------------------------------------------------------------------------
	// Enumerators
	//-------------------------------------------------------------------------
	enum DIRECTION 
	{ 
		DIR_FORWARD		= 1, 
		DIR_BACKWARD	= 2, 
		DIR_LEFT		= 4, 
		DIR_RIGHT		= 8, 
	};

	enum ESpeedStates
	{
		SPEED_START,
		SPEED_STOP
	};

	//-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
			 Brick(char type);
	virtual ~Brick();

	//-------------------------------------------------------------------------
	// Public Functions for This Class.
	//-------------------------------------------------------------------------
	void					Init(HDC hdc, const Vec2& position);
	void					Update( float dt );
	GameObjectType			GetObjectType() const 
	{ 	
		return GOT_Brick;
	}
	GameObjectTypeSub			GetObjectTypeSub() const 
	{ 	
		switch(BrickType)
		{
			case '1': return GOT_Brickk; break;
			case 'u': return GOT_Bricku; break;
			case 'd': return GOT_Brickd; break;
			case 'a': return GOT_BrickGift; break;
			case 'b': return GOT_BrickGift; break;
			case 'c': return GOT_BrickGift; break;
			case 'e': return GOT_BrickGift; break;
			case 'f': return GOT_BrickGift; break;
		}
	}
	void					Draw(HDC hdc) const;
	void					Move(ULONG ulDirection);
	virtual int				GetWidth() const { return m_pSprite->GetWidth(); }
	virtual int				GetHeight() const { return m_pSprite->GetHeight(); }
	int						DecreaseDouble(){ --DoubleCollision; return DoubleCollision;};
	void					ChangeSprite(){ delete m_pSprite; m_pSprite = new Sprite("data/brick.bmp", RGB(0xff,0x00, 0xff)); }
	char					GetBrickType(){ return BrickType; }

private:
	//-------------------------------------------------------------------------
	// Private Variables for This Class.
	//-------------------------------------------------------------------------
	
	Sprite*					m_pSprite;
	ESpeedStates			m_eSpeedState;
	float					m_fTimer;
	char					BrickType;
	int						DoubleCollision;
	
};

#endif // _BRICK_H_