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
	GOT_Brickk,
	GOT_Bricku,
	GOT_Brickd,
	GOT_BrickGift,
	GOT_Gift100,
	GOT_Gift200,
	GOT_GiftUpLife,
	GOT_GiftDownLife,
	GOT_GiftUnstoppableBall,

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
	virtual void Draw(HDC hdc) const = 0;
	virtual bool Expired() const { return false; }
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;

	virtual bool AffectedByGravity() {
		return false;
	};
	virtual Vec2 PlayerGetPosition( Vec2 myPosition ){ return myPosition; };
	virtual void Init(HDC hdc, const Vec2& position) { };
	virtual int	DecreaseDouble(){ return 0; };
	virtual void ChangeSprite(){ };


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
	DrawFunctor(HDC hdc) : myHDC(hdc) { }
	void operator()(const std::shared_ptr<CGameObject>& object)
	{
		object->Draw(myHDC);
	}
protected:
	HDC myHDC;
};

#endif // GAME_OBJECT_H