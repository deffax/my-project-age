#include "GameCodeStd.h"
#include "HumanView.h"

#include <DXUTgui.h>


#include "../Graphics3D/D3DRenderer.h"

#include "../Mainloop/Process.h"

#include "../Utilities/String.h"
#include "../LUAScripting/LuaStateManager.h"


const unsigned int SCREEN_REFRESH_RATE(1000/60);
const GameViewId gc_InvalidGameView = 0xffffffff;


const int kCursorBlinkTimeMS = 500;
char const* const kExitString = "exit";
char const* const kClearString = "clear";

HumanView::HumanView(shared_ptr<IRenderer> renderer)
{
	m_pProcessManager = GCC_NEW ProcessManager;
	m_ViewId = gc_InvalidGameView;
}

HumanView::~HumanView()
{
	SAFE_DELETE(m_pProcessManager);
}


void HumanView::VOnRender(double fTime, float fElapsedTime)
{
	m_currTick = timeGetTime();
	if(m_currTick == m_lastDraw)
		return;
	
	if(m_runFullSpeed || ((m_currTick - m_lastDraw) > SCREEN_REFRESH_RATE))
	{
		if(g_pApp->m_Renderer->VPreRender())
		{
			m_ScreenElements.sort(SortBy_SharedPtr_Content<IScreenElement>());
			for(ScreenElementList::iterator i = m_ScreenElements.begin();
				i != m_ScreenElements.end(); ++i)
			{
				if((*i)->VIsVisible())
				{
					(*i)->VOnRender(fTime, fElapsedTime);
				}
			}
			VRenderText();
			m_Console.Render();

			m_lastDraw = m_currTick;
		}
		g_pApp->m_Renderer->VPostRender();
	}
}


void HumanView::VOnRestore()
{
	
	for(ScreenElementList::iterator i = m_ScreenElements.begin();
				i != m_ScreenElements.end(); ++i)
	{
		(*i)->VOnRestore();
	}
	
}

void HumanView::VOnLostDevice()
{
	
	for(ScreenElementList::iterator i = m_ScreenElements.begin();
				i != m_ScreenElements.end(); ++i)
	{
		(*i)->VOnLostDevice();
	}
	
}


void HumanView::VOnUpdate(const int deltaMilliseconds)
{
	m_pProcessManager->UpdateProcesses(deltaMilliseconds);

	m_Console.Update(deltaMilliseconds);

	
	for(ScreenElementList::iterator i=m_ScreenElements.begin(); i!=m_ScreenElements.end(); ++i)
	{
		(*i)->VOnUpdate(deltaMilliseconds);
	}
}


LRESULT CALLBACK HumanView::VOnMsgProc(AppMsg msg)
{
	for(ScreenElementList::iterator i = m_ScreenElements.begin();
				i != m_ScreenElements.end(); ++i)
			{
				if((*i)->VIsVisible())
				{
					if((*i)->VOnMsgProc(msg))
					{
						return 1;
					}
				}
			}

	LRESULT result = 0;
	switch(msg.m_uMsg)
	{
	case WM_KEYDOWN:
		if(m_Console.IsActive()){}
		else if(m_KeyboardHandler)
		{
			result = m_KeyboardHandler->VOnKeyDown(static_cast<const BYTE>(msg.m_wParam));
		}
		break;

	case WM_KEYUP:
		if(m_Console.IsActive()){}
		else if(m_KeyboardHandler)
		{
			result = m_KeyboardHandler->VOnKeyUp(static_cast<const BYTE>(msg.m_wParam));
		}
		break;

	case WM_MOUSEMOVE:
		if(m_PointerHandler)
			result = m_PointerHandler->VOnPointerMove(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)),1);
		break;

	case WM_LBUTTONDOWN:
			if (m_PointerHandler)
			{
				SetCapture(msg.m_hWnd);
				result = m_PointerHandler->VOnPointerButtonDown(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerLeft");
			}	
			break;

		case WM_LBUTTONUP:
			if (m_PointerHandler)
			{
				SetCapture(NULL);
				result = m_PointerHandler->VOnPointerButtonUp(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerLeft");
			}
			break;

		case WM_RBUTTONDOWN:
			if (m_PointerHandler)
			{
				SetCapture(msg.m_hWnd);
				result = m_PointerHandler->VOnPointerButtonDown(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerRight");
			}
			break;

		case WM_RBUTTONUP:
			if (m_PointerHandler)
			{
				SetCapture(NULL);
				result = m_PointerHandler->VOnPointerButtonUp(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerRight");
			}
			break;

		case WM_CHAR:
			if (m_Console.IsActive())
			{
				const unsigned int oemScan = int( msg.m_lParam & ( 0xff << 16 ) ) >> 16;
				m_Console.HandleKeyboardInput( msg.m_wParam, MapVirtualKey( oemScan, 1 ), true );
			}
			else
			{
				
				if (('~'==msg.m_wParam) || ('|'==msg.m_wParam)) //('`'==msg.m_wParam))
				{
					m_Console.SetActive(true);
				}
			}
			break;
		default:
			return 0;
	}

	return 0;
}

void HumanView::VPushElement(shared_ptr<IScreenElement> pElement)
{
	m_ScreenElements.push_front(pElement);
}

void HumanView::VRemoveElement(shared_ptr<IScreenElement> pElement)
{
	m_ScreenElements.remove(pElement);
}








HumanView::Console::Console()
	: m_bActive(false),
	m_bExecuteStringOnUpdate(false)
{	
	m_ConsoleInputSize = 48;
	m_CurrentInputString = std::string("");
	m_InputColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_OutputColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_CursorBlinkTimer = kCursorBlinkTimeMS;
	m_bCursorOn = true;
}

HumanView::Console::~Console()
{}


void HumanView::Console::AddDisplayText(const std::string& newText)
{
	m_CurrentOutputString += newText;
	m_CurrentOutputString += '\n';
}

void HumanView::Console::SetDisplayText(const std::string& newText)
{
	m_CurrentOutputString = newText;
}


void HumanView::Console::Update(const int deltaMilliseconds)
{
	if(!m_bActive)
	{
		return;
	}

	if(m_bExecuteStringOnUpdate)
	{
		const std::string renderedInputString = std::string(">")  + m_CurrentInputString;
		if(0 == m_CurrentInputString.compare(kExitString))
		{
			SetActive(false);
			m_CurrentInputString.clear();
		}
		else if(0 == m_CurrentInputString.compare(kClearString))
		{
			m_CurrentInputString = renderedInputString;
			m_CurrentInputString.clear();
		}
		else
		{
			AddDisplayText(renderedInputString);
			const int retVal = true;
			if(!m_CurrentInputString.empty())
				LuaStateManager::Get()->VExecuteString(m_CurrentInputString.c_str());

			m_CurrentInputString.clear();
		}
		m_bExecuteStringOnUpdate = false;
		SetActive(false);
	}
	m_CursorBlinkTimer -= deltaMilliseconds;
	if(m_CursorBlinkTimer < 0)
	{
		m_CursorBlinkTimer = 0;
		m_bCursorOn =!m_bCursorOn;
		m_CursorBlinkTimer = kCursorBlinkTimeMS;
	}
}


void HumanView::Console::Render( )
{
	//Don't do anything if not active.
	if ( !m_bActive )
	{
		return;	//Bail!
	}

	D3DRenderer::g_pTextHelper->Begin();
	const D3DXCOLOR white( 1.0f, 1.0f, 1.0f, 1.0f );
	const D3DXCOLOR black( 0.0f, 0.0f, 0.0f, 1.0f );
	RECT inputTextRect, outputTextRect, shadowRect;

	//Display the console text at screen top, below the other text displayed.
	const std::string finalInputString = std::string( ">" ) + m_CurrentInputString + ( m_bCursorOn ? '\xa0' : '_' );
	inputTextRect.left = 10;
	inputTextRect.right = g_pApp->GetScreenSize().x - 10;
	inputTextRect.top = 100;
	inputTextRect.bottom = g_pApp->GetScreenSize().y - 10;

	const int kNumWideChars = 4096;
	WCHAR wideBuffer[ kNumWideChars ];
	AnsiToWideCch( wideBuffer, finalInputString.c_str(), kNumWideChars );

	D3DRenderer::g_pTextHelper->DrawTextLine( inputTextRect, DT_LEFT | DT_TOP | DT_CALCRECT, wideBuffer );

	//Draw with shadow first.
	shadowRect = inputTextRect;
	++shadowRect.left;
	++shadowRect.top;
	D3DRenderer::g_pTextHelper->SetForegroundColor( black );
	D3DRenderer::g_pTextHelper->DrawTextLine( shadowRect, DT_LEFT | DT_TOP, wideBuffer );

	//Now bright text.
	D3DRenderer::g_pTextHelper->SetForegroundColor( white );
	D3DRenderer::g_pTextHelper->DrawTextLine( inputTextRect, DT_LEFT | DT_TOP, wideBuffer );

	//Now display the output text just below the input text.
	outputTextRect.left = inputTextRect.left + 15;
	outputTextRect.top = inputTextRect.bottom + 15;
	outputTextRect.right = g_pApp->GetScreenSize().x - 10;
	outputTextRect.bottom = g_pApp->GetScreenSize().y - 10;
	AnsiToWideCch( wideBuffer, m_CurrentOutputString.c_str(), kNumWideChars );

	//Draw with shadow first.
	shadowRect = outputTextRect;
	++shadowRect.left;
	++shadowRect.top;
	D3DRenderer::g_pTextHelper->SetForegroundColor( black );
	D3DRenderer::g_pTextHelper->DrawTextLine( shadowRect, DT_LEFT | DT_TOP, wideBuffer );

	//Now bright text.
	D3DRenderer::g_pTextHelper->SetForegroundColor( white );
	D3DRenderer::g_pTextHelper->DrawTextLine( outputTextRect, DT_LEFT | DT_TOP, wideBuffer );

	D3DRenderer::g_pTextHelper->End();
}

void HumanView::Console::HandleKeyboardInput( const unsigned int keyVal, const unsigned int oemKeyVal, const bool bKeyDown )
{
	if ( true == m_bExecuteStringOnUpdate )
	{
		//We've already got a string to execute; ignore.
		return;
	}

	//See if it's a valid key press that we care about.
	switch( oemKeyVal )
	{
	    case VK_BACK:
		{
			const size_t strSize = m_CurrentInputString.size();
			if ( strSize > 0 )
			{
				m_CurrentInputString.erase((strSize - 1), 1);
			}
            break;
		}

    	case VK_RETURN:
		{
			m_bExecuteStringOnUpdate = true;	//Execute this string.
            break;
		}

    	default:
        {
		    m_CurrentInputString += (char)keyVal;
		    break;
        }
	}
}
