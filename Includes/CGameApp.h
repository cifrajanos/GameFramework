//-----------------------------------------------------------------------------
// File: CGameApp.h
//
// Desc: Game Application class, this is the central hub for all app processing
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

#ifndef _CGAMEAPP_H_
#define _CGAMEAPP_H_

//-----------------------------------------------------------------------------
// CGameApp Specific Includes
//-----------------------------------------------------------------------------
#include "Main.h"
#include "CTimer.h"
#include "CPlayer.h"
#include "Ball.h"
#include "Brick.h"
#include "Gift.h"
#include "CGameObject.h"
#include "BackBuffer.h"
#include "ResizableImage.h"
#include "Singleton.h"
#include "LoadLevel.h"
#include <vector>
#include <memory>

//-----------------------------------------------------------------------------
// Forward Declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Main Class Declarations
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : CGameApp (Class)
// Desc : Central game engine, initializes the game and handles core processes.
//-----------------------------------------------------------------------------
class CGameApp : public Singleton<CGameApp>
{
	SINGLETON_DECL(CGameApp);
public:
	//-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
	virtual ~CGameApp();

	//-------------------------------------------------------------------------
	// Public Functions for This Class
	//-------------------------------------------------------------------------
	LRESULT     DisplayWndProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );
	bool        InitInstance( LPCTSTR lpCmdLine, int iCmdShow );
	int         BeginGame( );
	bool        ShutDown( );
	HDC         GetBackBufferDC() const { return m_pBBuffer ? m_pBBuffer->getDC() : NULL; }

private:
	//-------------------------------------------------------------------------
	// Private Functions for This Class
	//-------------------------------------------------------------------------
	bool        BuildObjects	( );
	void        ReleaseObjects( );
	void        FrameAdvance	( );
	bool        CreateDisplay	( );
	void        ChangeDevice	( );
	void        SetupGameState( );
	void        AnimateObjects( );
	void        DrawObjects		( );
	void        ProcessInput	( );
	void        CollisionDetection();
	void		Score();
	void		Increase_Score(int k);
	void		Decrease_Score(int k);
	void		DrawScore(HDC hdc) const;
	void		Life();
	void		Decrease_Life();
	void		Increase_Life();
	void		Decrease_Life2();
	void		DrawLife(HDC hdc) const;
	void		DrawGame(HDC hdc,char type[20]);

	//-------------------------------------------------------------------------
	// Private Static Functions For This Class
	//-------------------------------------------------------------------------
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	//-------------------------------------------------------------------------
	// Private Variables For This Class
	//-------------------------------------------------------------------------
	CTimer              m_Timer;            // Game timer
	ULONG               m_LastFrameRate;    // Used for making sure we update only when fps changes.

	HWND                m_hWnd;             // Main window HWND
	HICON               m_hIcon;            // Window Icon
	HMENU               m_hMenu;            // Window Menu

	bool                m_bActive;          // Is the application active ?

	ULONG               m_nViewX;           // X Position of render viewport
	ULONG               m_nViewY;           // Y Position of render viewport
	ULONG               m_nViewWidth;       // Width of render viewport
	ULONG               m_nViewHeight;      // Height of render viewport

	POINT               m_OldCursorPos;     // Old cursor position for tracking
	HINSTANCE		    m_hInstance;		// Window instance

	CResizableImage     m_imgBackground;
	BackBuffer*         m_pBBuffer;

	int					nrLevel;
	int					countBricks;
	int					countGifts;
	int					nrBricks;
	int					countScore;
	int					countLife;
	float				m_fTimer;
	char				text[50];
	char				text2[50];
	bool				BricksExist;
	bool				FollowPlayer;
	bool				MouseOrKeyboard;
	bool				GameOver;
	bool				LevelChange;
	bool				StartGame;
	bool				UnstoppableBall;
	bool				StickyBar;
	bool				ShrinkBar;
	bool				MoveBall;
	Vec2				ballPos;
	LoadLevel			level;
	RECT				rect;
	RECT				rect2;
	CURSORINFO			cursor;


	// separate vectors to gain a little performance
	std::vector<std::shared_ptr<CGameObject>>	m_vGameObjectsAnimate;
	std::vector<std::shared_ptr<CGameObject>>	m_vGameObjectsGift;
	std::vector<std::shared_ptr<CGameObject>>   m_vGameObjects;
	std::weak_ptr<CPlayer>                      m_pPlayer;
	std::weak_ptr<Ball>							m_pBall;
};

#endif // _CGAMEAPP_H_