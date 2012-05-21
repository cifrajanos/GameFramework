//-----------------------------------------------------------------------------
// File: CGameApp.cpp
//
// Desc: Game Application class, this is the central hub for all app processing
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CGameApp Specific Includes
//-----------------------------------------------------------------------------
#include "CGameApp.h"
#include "CPlayer.h"
#include <algorithm>


extern HINSTANCE g_hInst;

#define PLAYER_START_X 100
#define PLAYER_START_Y 400
#define PARALLAX_BACKGROUND_SPEED 64


//-----------------------------------------------------------------------------
// CGameApp Member Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : CGameApp () (Constructor)
// Desc : CGameApp Class Constructor
//-----------------------------------------------------------------------------
CGameApp::CGameApp()
{
	// Reset / Clear all required values
	m_hWnd			= NULL;
	m_hIcon			= NULL;
	m_hMenu			= NULL;
	m_pBBuffer		= NULL;
    //m_pParallax     = NULL;
	m_LastFrameRate = 0;
	FollowPlayer	= 1;
	GameOver		= 0;
	BricksExist		= 0;
	UnstoppableBall	= 0;
	Score();
	Life();
}

//-----------------------------------------------------------------------------
// Name : ~CGameApp () (Destructor)
// Desc : CGameApp Class Destructor
//-----------------------------------------------------------------------------
CGameApp::~CGameApp()
{
	// Shut the engine down
	ShutDown();
}

//-----------------------------------------------------------------------------
// Name : InitInstance ()
// Desc : Initializes the entire Engine here.
//-----------------------------------------------------------------------------
bool CGameApp::InitInstance( LPCTSTR lpCmdLine, int iCmdShow )
{
	// Create the primary display device
	if (!CreateDisplay()) { ShutDown(); return false; }
	
	m_pBBuffer = new BackBuffer(m_hWnd, m_nViewWidth, m_nViewHeight);

		if(!m_imgBackground.LoadBitmapFromFile("data/background2.bmp", GetDC(m_hWnd)))
		return false;

	m_imgBackground.SetFilter(new CBoxFilter());
	m_imgBackground.Resample(m_nViewWidth, m_nViewHeight);

	// Build Objects
	/*if (!BuildObjects()) 
	{ 
		MessageBox( 0, _T("Failed to initialize properly. Reinstalling the application may solve this problem.\nIf the problem persists, please contact technical support."), _T("Fatal Error"), MB_OK | MB_ICONSTOP);
		ShutDown(); 
		return false; 
	}

	// Set up all required game states
	SetupGameState();*/

	// Success!
	return true;
}

//-----------------------------------------------------------------------------
// Name : CreateDisplay ()
// Desc : Create the display windows, devices etc, ready for rendering.
//-----------------------------------------------------------------------------
bool CGameApp::CreateDisplay()
{

	LPTSTR			WindowTitle		= _T("GameFramework");
	LPCSTR			WindowClass		= _T("GameFramework_Class");
	USHORT			Width			= 800;
	USHORT			Height			= 600;
	RECT			rc;
	WNDCLASSEX		wcex;


	wcex.cbSize				= sizeof(WNDCLASSEX);
	wcex.style				= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc		= CGameApp::StaticWndProc;
	wcex.cbClsExtra			= 0;
	wcex.cbWndExtra			= 0;
	wcex.hInstance			= g_hInst;
	wcex.hIcon				= LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground		= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName		= 0;
	wcex.lpszClassName		= WindowClass;
	wcex.hIconSm			= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON));


	if(RegisterClassEx(&wcex)==0)
		return false;

	// Retrieve the final client size of the window
	::GetClientRect( m_hWnd, &rc );
	m_nViewX        = rc.left;
	m_nViewY        = rc.top;
	m_nViewWidth    = rc.right - rc.left;
	m_nViewHeight   = rc.bottom - rc.top;


	//m_hWnd = CreateWindow(WindowClass, WindowTitle, WS_OVERLAPPEDWINDOW,

	m_hWnd = CreateWindow(WindowClass, WindowTitle, WS_OVERLAPPED,CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, NULL, NULL, g_hInst, this);

	if (!m_hWnd)
		return false;

	// Show the window
	ShowWindow(m_hWnd, SW_SHOW);

	// Success!!
	return true;
}

//-----------------------------------------------------------------------------
// Name : BeginGame ()
// Desc : Signals the beginning of the physical post-initialization stage.
//		From here on, the game engine has control over processing.
//-----------------------------------------------------------------------------
int CGameApp::BeginGame()
{
	MSG		msg;
	BeginGameS=0;
	// Start main loop
	while(true) 
	{
		// Did we receive a message, or are we idling ?
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) 
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage( &msg );
			DispatchMessage ( &msg );
		} 
		else 
		{
			cursor.cbSize = sizeof(CURSORINFO);
			GetCursorInfo(&cursor);

			// Advance Game Frame.
			if(BeginGameS==0)
			{				
				if(cursor.ptScreenPos.x > 600 && cursor.ptScreenPos.y < 220)
				{
					if (!BuildObjects()) 
					{ 
						MessageBox( 0, _T("Failed to initialize properly. Reinstalling the application may solve this problem.\nIf the problem persists, please contact technical support."), _T("Fatal Error"), MB_OK | MB_ICONSTOP);
						ShutDown(); 
						return false; 
					}

					// Set up all required game states
					SetupGameState();
					BeginGameS=1;
				}

				if(cursor.ptScreenPos.x > 600 && cursor.ptScreenPos.y > 580)
				{
					PostQuitMessage(0);
				}
			}
			cursor.cbSize = sizeof(CURSORINFO);
			FrameAdvance();
		} // End If messages waiting
	
	} // Until quit message is received

	return 0;
}

//-----------------------------------------------------------------------------
// Name : ShutDown ()
// Desc : Shuts down the game engine, and frees up all resources.
//-----------------------------------------------------------------------------
bool CGameApp::ShutDown()
{
	// Release any previously built objects
	ReleaseObjects ( );
	
	// Destroy menu, it may not be attached
	if ( m_hMenu ) DestroyMenu( m_hMenu );
	m_hMenu = NULL;

	// Destroy the render window
	SetMenu( m_hWnd, NULL );
	if ( m_hWnd ) DestroyWindow( m_hWnd );
	m_hWnd = NULL;
	
	// Shutdown Success
	return true;
}

//-----------------------------------------------------------------------------
// Name : StaticWndProc () (Static Callback)
// Desc : This is the main message pump for ALL display devices, it captures
//		the appropriate messages, and routes them through to the application
//		class for which it was intended, therefore giving full class access.
// Note : It is VITALLY important that you should pass your 'this' pointer to
//		the lpParam parameter of the CreateWindow function if you wish to be
//		able to pass messages back to that app object.
//-----------------------------------------------------------------------------
LRESULT CALLBACK CGameApp::StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	// If this is a create message, trap the 'this' pointer passed in and store it within the window.
	if ( Message == WM_CREATE ) SetWindowLong( hWnd, GWL_USERDATA, (LONG)((CREATESTRUCT FAR *)lParam)->lpCreateParams);

	// Obtain the correct destination for this message
	CGameApp *Destination = (CGameApp*)GetWindowLong( hWnd, GWL_USERDATA );
	
	// If the hWnd has a related class, pass it through
	if (Destination) return Destination->DisplayWndProc( hWnd, Message, wParam, lParam );
	
	// No destination found, defer to system...
	return DefWindowProc( hWnd, Message, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Name : DisplayWndProc ()
// Desc : The display devices internal WndProc function. All messages being
//		passed to this function are relative to the window it owns.
//-----------------------------------------------------------------------------
LRESULT CGameApp::DisplayWndProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
	ULONG		Direction = 0;
	// Determine message type
	switch (Message)
	{
		case WM_CREATE:
			break;
		
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		
		case WM_SIZE:
			if ( wParam == SIZE_MINIMIZED )
			{
				// App is inactive
				m_bActive = false;
			
			} // App has been minimized
			else
			{
				// App is active
				m_bActive = true;

				// Store new viewport sizes
				m_nViewWidth  = LOWORD( lParam );
				m_nViewHeight = HIWORD( lParam );
			} // End if !Minimized
			break;

		/*case WM_LBUTTONDOWN:
			// Capture the mouse
			SetCapture( m_hWnd );
			GetCursorPos( &m_OldCursorPos );
			break;*/

		case WM_LBUTTONDOWN:
			FollowPlayer=0;
			LevelChange = 0;
			Direction |= Ball::DIR_START;
			m_pBall.lock()->Move(Direction,0);
			SetCapture( m_hWnd );
			GetCursorPos( &m_OldCursorPos );
			break;

		case WM_LBUTTONUP:
			// Release the mouse
			ReleaseCapture( );
			break;

		case WM_KEYDOWN:
			switch(wParam)
			{
			case VK_ESCAPE:
				_CrtDumpMemoryLeaks();
				PostQuitMessage(0);
				break;
			case VK_SPACE:
				FollowPlayer=0;
				LevelChange = 0;
				Direction |= Ball::DIR_START;
				m_pBall.lock()->Move(Direction,0);
				break;
			}
			break;

		case WM_COMMAND:
			break;

		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);

	} // End Message Switch
	
	return 0;
}

//-----------------------------------------------------------------------------
// Name : BuildObjects ()
// Desc : Build our demonstration meshes, and the objects that instance them
//-----------------------------------------------------------------------------
bool CGameApp::BuildObjects()
{
	m_pBBuffer = new BackBuffer(m_hWnd, m_nViewWidth, m_nViewHeight);

	auto pPlayer = std::make_shared<CPlayer>();
	m_vGameObjects.push_back(pPlayer);
	m_pPlayer = pPlayer;

	auto pBall = std::make_shared<Ball>();
	m_vGameObjects.push_back(pBall);
	m_pBall = pBall;

	nrlevel=1;
	level.LoadBricks(nrlevel);

	string::iterator it;
	for(it=level.bricks.begin(); it < level.bricks.end(); it++)
	{
		if(*it == '-')
		{
			continue;
		}
		auto pBrick = std::make_shared<Brick>(*it);
		m_vGameObjects.push_back(pBrick);
	}
	BricksExist=1;


	if(!m_imgBackground.LoadBitmapFromFile("data/background.bmp", GetDC(m_hWnd)))
		return false;

	m_imgBackground.SetFilter(new CBoxFilter());
	m_imgBackground.Resample(m_nViewWidth, m_nViewHeight);

   // m_pParallax = new ParallaxLayer("data/tile.bmp");

	// Success!
	return true;
}

//-----------------------------------------------------------------------------
// Name : SetupGameState ()
// Desc : Sets up all the initial states required by the game.
//-----------------------------------------------------------------------------
void CGameApp::SetupGameState()
{

	int x=65;
	int y=100;
	nBricks=0;
	m_pPlayer.lock()->Init(m_pBBuffer->getDC(), Vec2((int)m_nViewWidth/2, (int)m_nViewHeight-m_pPlayer.lock()->GetHeight()));
	m_pBall.lock()->Init(m_pBBuffer->getDC(), Vec2((int)m_nViewWidth/2,(int)m_nViewHeight-m_pBall.lock()->GetHeight()-m_pPlayer.lock()->GetHeight()-1));
	for(auto it = m_vGameObjects.begin(); it != m_vGameObjects.end(); ++it)
	{
		CGameObject * pGameObj = it->get();
		if(pGameObj->GetObjectType() == GOT_Brick)
		{
			for(int i=0;i<sizeof(level.bricks);i++)
			{
				if(level.bricks[nBricks]=='-')
				{
					x+=pGameObj->GetWidth()+1;
					if(x>=750)
					{
						x=65;
						y+=pGameObj->GetHeight()+1;
					}
				}
				else
				{
					nBricks++;
					break; 
				}
				nBricks++;
			}

			pGameObj->Init(m_pBBuffer->getDC(), Vec2(x,y));
			x+=pGameObj->GetWidth()+1;
			if(x>=750)
			{
				x=65;
				y+=pGameObj->GetHeight()+1;
			}
		}
	}


	//m_pPlayer.lock()->Init(m_pBBuffer->getDC(), Vec2(PLAYER_START_X, PLAYER_START_Y));
    
//    m_pParallax->myPosition = Vec2(m_pParallax->GetWidth()/2, m_pParallax->GetHeight()/2);
//    m_pParallax->Initialize(m_pBBuffer->getDC(), ParallaxLayer::AXIS_VERTICAL | ParallaxLayer::AXIS_HORIZONTAL, PARALLAX_BACKGROUND_SPEED, m_nViewWidth, m_nViewHeight);

}

//-----------------------------------------------------------------------------
// Name : ReleaseObjects ()
// Desc : Releases our objects and their associated memory so that we can
//		rebuild them, if required, during our applications life-time.
//-----------------------------------------------------------------------------
void CGameApp::ReleaseObjects( )
{
	// this will automatically call the d-tors for each shared pointer objects
	m_vGameObjects.clear();
	m_vGameObjectsGift.clear();

	SAFE_DELETE(m_pBBuffer);
    // SAFE_DELETE(m_pParallax);
}

//-----------------------------------------------------------------------------
// Name : FrameAdvance () (Private)
// Desc : Called to signal that we are now rendering the next frame.
//-----------------------------------------------------------------------------
void CGameApp::FrameAdvance()
{
	static TCHAR FrameRate[ 50 ];
	static TCHAR TitleBuffer[ 255 ];

	// Advance the timer
	m_Timer.Tick( 15 );

	// Skip if app is inactive
	if ( !m_bActive ) return;
	
	// Get / Display the framerate
	if ( m_LastFrameRate != m_Timer.GetFrameRate() )
	{
		m_LastFrameRate = m_Timer.GetFrameRate( FrameRate, 50 );
		sprintf_s( TitleBuffer, _T("Game : %s"), FrameRate );
		SetWindowText( m_hWnd, TitleBuffer );
	} // End if Frame Rate Altered

	if(BeginGameS==1)
	{
	// Poll & Process input devices
	ProcessInput();

	// Collision detection between game objects
	BricksExist=0;
	CollisionDetection();
		if(BricksExist==0)
		{
			int x=65;
			int y=100;

			CountBricks=0;
			CountGifts=0;
			nBricks=0;

			for(auto it = m_vGameObjects.begin(); it != m_vGameObjects.end(); ++it)
			{
				CGameObject * pGameObj = it->get();
				
				if(pGameObj->GetObjectTypeSub() == GOT_Bricku)
				{
					CountBricks++;
				}
			}

			for(int i=0;i<=CountBricks;i++)
			{
				for(auto it2 = m_vGameObjects.begin(); it2 != m_vGameObjects.end(); ++it2)
				{
					CGameObject * pGameObj = it2->get();
					if(pGameObj->GetObjectTypeSub() == GOT_Bricku)
					{
						m_vGameObjects.erase(it2);
						break;
					}
				}
			}

			for(auto it = m_vGameObjectsGift.begin(); it != m_vGameObjectsGift.end(); ++it)
			{
				CGameObject * pGameObj = it->get();
				
				CountGifts++;

			}

			for(int i=0;i<=CountGifts;i++)
			{
				for(auto it2 = m_vGameObjectsGift.begin(); it2 != m_vGameObjectsGift.end(); ++it2)
				{
					CGameObject * pGameObj = it2->get();
					
					m_vGameObjectsGift.erase(it2);
					break;
					
				}
			}

			m_pBall.lock()->myVelocity.x=0;
			m_pBall.lock()->myVelocity.y=0;
			m_pBall.lock()->myPosition.x=m_pPlayer.lock()->myPosition.x;
			m_pBall.lock()->myPosition.y=m_nViewHeight-m_pBall.lock()->GetHeight()-m_pPlayer.lock()->GetHeight()-1;
			FollowPlayer=1;
			UnstoppableBall=0;
			nrlevel++;
			level.LoadBricks(nrlevel);

			string::iterator it;
			for(it=level.bricks.begin(); it < level.bricks.end(); it++)
			{
				if(*it == '-')
				{
					continue;
				}
				auto pBrick = std::make_shared<Brick>(*it);
				m_vGameObjects.push_back(pBrick);
			}

			for(auto it = m_vGameObjects.begin(); it != m_vGameObjects.end(); ++it)
			{
				CGameObject * pGameObj = it->get();
				if(pGameObj->GetObjectType() == GOT_Brick)
				{
					for(int i=0;i<sizeof(level.bricks);i++)
					{
						if(level.bricks[nBricks]=='-')
						{
							x+=pGameObj->GetWidth()+1;
							if(x>=750)
							{
								x=65;
								y+=pGameObj->GetHeight()+1;
							}
						}
						else
						{
							nBricks++;
							break; 
						}
						nBricks++;
					}

					pGameObj->Init(m_pBBuffer->getDC(), Vec2(x,y));
					x+=pGameObj->GetWidth()+1;
					if(x>=750)
					{
						x=65;
						y+=pGameObj->GetHeight()+1;
					}
				}
			}
			LevelChange=1;
		}
		
		// Animate the game objects
		AnimateObjects();
	}

	// Drawing the game objects
	DrawObjects();
}

//-----------------------------------------------------------------------------
// Name : ProcessInput () (Private)
// Desc : Simply polls the input devices and performs basic input operations
//-----------------------------------------------------------------------------
void CGameApp::ProcessInput( )
{
	static UCHAR pKeyBuffer[ 256 ];
	ULONG		Direction = 0;
	POINT		CursorPos;
	float		X = 0.0f, Y = 0.0f;

	// Get cursor position
	//cursor.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&cursor);

	// Retrieve keyboard state
	if ( !GetKeyboardState( pKeyBuffer ) ) return;

	// Check the relevant keys
	//if ( pKeyBuffer[ VK_UP	] & 0xF0 ) Direction |= CPlayer::DIR_FORWARD;
	//if ( pKeyBuffer[ VK_DOWN  ] & 0xF0 ) Direction |= CPlayer::DIR_BACKWARD;
	if ( pKeyBuffer[ VK_LEFT  ] & 0xF0 ) Direction |= CPlayer::DIR_LEFT;
	if ( pKeyBuffer[ VK_RIGHT ] & 0xF0 ) Direction |= CPlayer::DIR_RIGHT;

	//SetCursor( NULL );

	if ( GetCapture() == m_hWnd )
	{
		MouseOrKeyboard=0;
	}
	
	if(MouseOrKeyboard==0){
		m_pPlayer.lock()->myPosition.x=cursor.ptScreenPos.x;
	}

	if(FollowPlayer==1){
	m_pBall.lock()->Move(Direction,1);
		if(MouseOrKeyboard==0){
			m_pBall.lock()->myPosition.x=cursor.ptScreenPos.x;
		}
	}

	m_pPlayer.lock()->Move(Direction);

	if(Direction==CPlayer::DIR_FORWARD || Direction==CPlayer::DIR_BACKWARD || Direction==CPlayer::DIR_LEFT || Direction==CPlayer::DIR_RIGHT){
		MouseOrKeyboard=1;
	}

	// Now process the mouse (if the button is pressed)
	if ( GetCapture() == m_hWnd )
	{
		// Hide the mouse pointer
		SetCursor( NULL );

		// Retrieve the cursor position
		GetCursorPos( &CursorPos );

		// Reset our cursor position so we can keep going forever :)
		SetCursorPos( m_OldCursorPos.x, m_OldCursorPos.y );

	} // End if Captured
}

//-----------------------------------------------------------------------------
// Name : AnimateObjects () (Private)
// Desc : Animates the objects we currently have loaded.
//-----------------------------------------------------------------------------
void CGameApp::AnimateObjects()
{
	ExpiredPredicate expiredPred;
	m_vGameObjects.erase(std::remove_if(m_vGameObjects.begin(), m_vGameObjects.end(), expiredPred), m_vGameObjects.end());
	m_vGameObjectsGift.erase(std::remove_if(m_vGameObjectsGift.begin(), m_vGameObjectsGift.end(), expiredPred), m_vGameObjectsGift.end());

    float dt = m_Timer.GetTimeElapsed();

	if(GameOver == 0 && level.Winner == 0 && LevelChange == 0)
	{
		UpdateFunctor updateFn(dt);
		std::for_each(m_vGameObjects.begin(), m_vGameObjects.end(), updateFn);
		std::for_each(m_vGameObjectsGift.begin(), m_vGameObjectsGift.end(), updateFn);
	}

   // m_pParallax->Move(CPlayer::DIR_FORWARD);
   // m_pParallax->Update(dt);
}

//-----------------------------------------------------------------------------
// Name : DrawObjects () (Private)
// Desc : Draws the game objects
//-----------------------------------------------------------------------------
void CGameApp::DrawObjects()
{
	m_pBBuffer->reset();

	HDC hdc = m_pBBuffer->getDC();

    m_imgBackground.Paint(hdc, 0, 0);

  //  m_pParallax->Draw(hdc);
	
	if(GameOver == 0 && level.Winner == 0 && LevelChange == 0)
	{
		DrawFunctor drawFn(hdc);
		std::for_each(m_vGameObjects.begin(), m_vGameObjects.end(), drawFn);
		std::for_each(m_vGameObjectsGift.begin(), m_vGameObjectsGift.end(), drawFn);
	}
	else
	{
		if(LevelChange == 1)
		DrawGame(hdc,"level");
		if(level.Winner == 1)
		DrawGame(hdc,"winner");
		if(GameOver == 1)
		DrawGame(hdc,"gameover");
	}

	DrawScore(hdc);
	DrawLife(hdc);

	m_pBBuffer->present();
}

//-----------------------------------------------------------------------------
// Name : CollisionDetection () (Private)
// Desc : Detects and handles collision
//-----------------------------------------------------------------------------
void CGameApp::CollisionDetection()
{	
	// collision detection with the main frame
	
	HDC hdc = m_pBBuffer->getDC();


	for(auto it = m_vGameObjects.begin(); it != m_vGameObjects.end(); ++it)
	{

		CGameObject * pGameObj = it->get();
		Vec2 pos = pGameObj->myPosition;

        pGameObj->myCollisionSide = CS_None;

		if(pGameObj->GetObjectTypeSub() == GOT_Brickk || pGameObj->GetObjectTypeSub() == GOT_Brickd || pGameObj->GetObjectTypeSub() == GOT_BrickGift)
		{
			BricksExist=1;
		}

		if(pGameObj->GetObjectType() == GOT_Ball || pGameObj->GetObjectType() == GOT_Player)
		{
			int dx = (int)pos.x - pGameObj->GetWidth() / 2;
			if( dx < 0 )
			{
				pGameObj->myCollisionSide |= CS_Left;
				if(pGameObj->GetObjectType() == GOT_Player)
				{
				pGameObj->myPosition.x=pGameObj->GetWidth()/2;
					if(FollowPlayer==1)
					{
						m_pBall.lock()->myPosition.x=pGameObj->GetWidth()/2;
					}
				}
			}


		 dx = (int)pos.x - (m_nViewWidth - pGameObj->GetWidth() / 2);
			if( dx > 0 )
			{
				pGameObj->myCollisionSide |= CS_Right;
				if(pGameObj->GetObjectType() == GOT_Player)
				{
				pGameObj->myPosition.x=m_nViewWidth - pGameObj->GetWidth() / 2;
					if(FollowPlayer==1)
					{
						m_pBall.lock()->myPosition.x=m_nViewWidth - pGameObj->GetWidth() / 2;
					}
				}
			}
		
			int dy = (int)pos.y - pGameObj->GetHeight() / 2;
			if( dy < 0 )
			{

				pGameObj->myCollisionSide |= CS_Top;
			}

			dy = (int)pos.y - (m_nViewHeight - pGameObj->GetHeight() / 2);
			if( dy > 0 )
			{
				pGameObj->myCollisionSide |= CS_Bottom;
				if(pGameObj->GetObjectType() == GOT_Ball)
				{
					Decrease_Life();
					UnstoppableBall = 0;
					if(life<1)
					{
						GameOver = 1;
					}
				}
			}
		}

		if(pGameObj->GetObjectType() == GOT_Ball)
		for(auto it2 = m_vGameObjects.begin(); it2 != m_vGameObjects.end(); ++it2)
		{
			CGameObject * pGameObj2 = it2->get();
			Vec2 pos2 = pGameObj2->myPosition;


			if(pGameObj->GetObjectType() == pGameObj2->GetObjectType())
			continue;
			if(pGameObj2->GetObjectType() == GOT_Player || pGameObj2->GetObjectType() == GOT_Brick)
			{
				if(abs(pos.y - pos2.y) < pGameObj->GetHeight()/2+pGameObj2->GetHeight()/2 && abs(pos.x - pos2.x) < pGameObj->GetWidth()/2+pGameObj2->GetWidth()/2)
				{		

					if(pGameObj2->GetObjectType() == GOT_Player)
					{
						pGameObj->myVelocity.y = -pGameObj->myVelocity.y;
					}
					else
					{
						if(UnstoppableBall != 1)
						if(pos.x-pos2.x>35)
						{
							pGameObj->myVelocity.x = -pGameObj->myVelocity.x;
						}else{pGameObj->myVelocity.y = -pGameObj->myVelocity.y;}
					}

					if(pGameObj2->GetObjectTypeSub() == GOT_Brickk || pGameObj2->GetObjectTypeSub() == GOT_Brickd || pGameObj2->GetObjectTypeSub() == GOT_BrickGift)
					{
						if(pGameObj2->GetObjectTypeSub() == GOT_Brickd)
						{						
							int CheckDouble=pGameObj2->DecreaseDouble();
							if(CheckDouble == 1)
								{
									Increase_Score(12);
									pGameObj2->ChangeSprite();
								}
							if(CheckDouble == 0)
							{
								Increase_Score(12);
								DrawScore(hdc);
								m_vGameObjects.erase(it2);
								break;
							}
						}
						else
						{
							if(pGameObj2->GetObjectTypeSub() == GOT_BrickGift)
							{
								auto pGift = std::make_shared<Gift>(pGameObj2->GetBrickType());
								pGift->Init(m_pBBuffer->getDC(), Vec2(pos2.x,pos2.y));
								m_vGameObjectsGift.push_back(pGift);
							}
							Increase_Score(12);
							DrawScore(hdc);
							m_vGameObjects.erase(it2);
							break;
						}
					}
				}
			}
		}

		if(pGameObj->GetObjectType() == GOT_Player)
		for(auto it2 = m_vGameObjectsGift.begin(); it2 != m_vGameObjectsGift.end(); ++it2)
		{
			CGameObject * pGameObj2 = it2->get();
			Vec2 pos2 = pGameObj2->myPosition;

			if(abs(pos.y - pos2.y) < pGameObj->GetHeight()/2+pGameObj2->GetHeight()/2 && abs(pos.x - pos2.x) < pGameObj->GetWidth()/2+pGameObj2->GetWidth()/2)
			{
				if(pGameObj2->GetObjectTypeSub() == GOT_Gift100)
				{
				Increase_Score(100);
				DrawScore(hdc);
				m_vGameObjectsGift.erase(it2);
				break;
				}
				if(pGameObj2->GetObjectTypeSub() == GOT_Gift200)
				{
				Increase_Score(200);
				DrawScore(hdc);
				m_vGameObjectsGift.erase(it2);
				break;
				}
				if(pGameObj2->GetObjectTypeSub() == GOT_GiftUpLife)
				{
				Increase_Life();
				DrawLife(hdc);
				m_vGameObjectsGift.erase(it2);
				break;
				}
				if(pGameObj2->GetObjectTypeSub() == GOT_GiftDownLife)
				{
				Decrease_Life2();
				DrawLife(hdc);
				m_vGameObjectsGift.erase(it2);
				break;
				}
				if(pGameObj2->GetObjectTypeSub() == GOT_GiftUnstoppableBall)
				{
				UnstoppableBall=1;
				m_vGameObjectsGift.erase(it2);
				break;
				}
			}

			int dy = (int)pos2.y - (m_nViewHeight - pGameObj2->GetHeight() / 2);
			if( dy > 0 )
			{
				m_vGameObjectsGift.erase(it2);
				break;
			}
		}
	}
}



void CGameApp::Score(){
	rect.left = 10;
	rect.top = 10;
	rect.right = 100;
	rect.bottom = 30;
	score=0;
	strcpy(text,"0");
}

void CGameApp::Increase_Score(int k){
	score+=k;
	itoa(score, text, 10);
}

void CGameApp::Decrease_Score(int k){
	score-=k;
	itoa(score, text, 10);
}

void CGameApp::DrawScore(HDC hdc) const
{
	SetBkColor(hdc, RGB(0xff, 0xff, 0xff));
	SetTextColor(hdc, RGB(0x00, 0, 0));
	SetBkMode(hdc, OPAQUE);
	DrawText(hdc, text, -1, (LPRECT)&rect, DT_WORDBREAK);
}

void CGameApp::Life(){
	rect2.left = 10;
	rect2.top = 50;
	rect2.right = 100;
	rect2.bottom = 100;
	life=3;
	strcpy(text2,"3");
}

void CGameApp::Decrease_Life(){
	life-=1;
	itoa(life, text2, 10);
	m_pBall.lock()->myVelocity.x=0;
	m_pBall.lock()->myVelocity.y=0;
	m_pBall.lock()->myPosition.x=m_pPlayer.lock()->myPosition.x;
	m_pBall.lock()->myPosition.y=m_nViewHeight-m_pBall.lock()->GetHeight()-m_pPlayer.lock()->GetHeight()-1;
	FollowPlayer=1;
}

void CGameApp::Increase_Life(){
	life+=1;
	itoa(life, text2, 10);
}

void CGameApp::Decrease_Life2(){
	life-=1;
	itoa(life, text2, 10);
}

void CGameApp::DrawLife(HDC hdc) const
{
	SetBkColor(hdc, RGB(0xff, 0xff, 0xff));
	SetTextColor(hdc, RGB(0x00, 0, 0));
	SetBkMode(hdc, OPAQUE);
	DrawText(hdc, text2, -1, (LPRECT)&rect2, DT_WORDBREAK);
}

void CGameApp::DrawGame(HDC hdc,char type[20])
{
	RECT rect;

	rect.left = 350;
	rect.top = 280;
	rect.right = 470;
	rect.bottom = 320;

	SetBkColor(hdc, RGB(0xff, 0xff, 0xff));
	SetTextColor(hdc, RGB(0x00, 0, 0));
	SetBkMode(hdc, OPAQUE);
	if(type == "level")
	{
		char nlevel[5];
		string levels;
		itoa(nrlevel,nlevel,10);
		levels="Level ";
		levels+=nlevel;
		DrawText(hdc, levels.c_str(), -1, (LPRECT)&rect, DT_WORDBREAK);
	}
	if(type == "winner")
	{
		DrawText(hdc, "WINNNERRR!!! Press ESC to exit", -1, (LPRECT)&rect, DT_WORDBREAK);
		BricksExist=1;
	}
	if(type == "gameover")
	{
		DrawText(hdc, "GAME OVER!!! Press ESC to exit", -1, (LPRECT)&rect, DT_WORDBREAK);
		BricksExist=1;
	}
}