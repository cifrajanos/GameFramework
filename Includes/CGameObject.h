/*! 
*  \brief     CGameObject class.
*  \details   This class is used as a base class for all game objects.
*  \author    Mihai Popescu
*  \date      27/03/2012
*  \copyright GNU Public License.
*/

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "Vec2.h"
#include "main.h"
//#include "AnimatedSprite.h"
#include <memory>

enum GameObjectType
{
	GOT_Invalid = -1,
	GOT_Player,
	GOT_Ball,
	GOT_Brick,
};


enum GameObjectTypeSub
{
	GOT_InvalidSub = -1,
	GOT_Ballk,
	GOT_BrickNormal,
	GOT_BrickUnbreakable,
	GOT_BrickDouble,
	GOT_BrickGift,
	GOT_Gift100,
	GOT_Gift200,
	GOT_GiftUpLife,
	GOT_GiftDownLife,
	GOT_GiftIncreaseSpeed,
	GOT_GiftDecreaseSpeed,
	GOT_GiftUnstoppableBall,
	GOT_GiftStickyBar,
	GOT_GiftShrinkBar,
	GOT_Animate,
	GOT_GiftSlowMouse,



};

enum CollisionSide
{
	CS_None = 0,
	CS_Left = 1,
	CS_Right = 2,
	CS_Top = 4,
	CS_Bottom = 8
};

class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();

	virtual GameObjectType GetObjectType() const { return GOT_Invalid; }
	virtual GameObjectTypeSub GetObjectTypeSub() const { return GOT_InvalidSub; }
	virtual char GetBrickType(){ return GOT_Invalid; }
	virtual void Update(float dt);
	virtual void Draw() const = 0;
	virtual bool Expired() const { return false; }
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual bool AffectedByGravity() const { return false; }
	virtual void Init(const Vec2& position) { };
	virtual int	DecreaseDouble(){ return 0; };
	virtual void ChangeSprite(){ };
	virtual void Animate(){ };

public:
	// Keep these public because they need to be
	// modified externally frequently.
	Vec2 myPosition;
	Vec2 myVelocity;
	Vec2 myAcceleration;
	int myCollisionSide;

private:
	HPEN mySpeedPen;
	HPEN myAccelerationPen;
	Vec2 myResultant;
};


class ExpiredPredicate
{
public:
	bool operator()(const std::shared_ptr<CGameObject>& object) const
	{
		return object->Expired();
	}
};

class UpdateFunctor
{
public:
	UpdateFunctor(float dt) : myDt(dt) { }
	void operator()(std::shared_ptr<CGameObject>& object)
	{
		object->Update(myDt);
	}
protected:
	float myDt;
};

class DrawFunctor
{
public:
	DrawFunctor() { }
	void operator()(const std::shared_ptr<CGameObject>& object)
	{
		object->Draw();
	}
};

#endif // GAME_OBJECT_H