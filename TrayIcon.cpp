/////////////////////////////////////////////////////////////////////////////
// TrayIcon.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TrayIcon.h"

IMPLEMENT_DYNAMIC(CTrayIcon, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CTrayIcon construction/creation/destruction

CTrayIcon::CTrayIcon()
	: m_bEnabled(FALSE)
	, m_bHidden(FALSE)
	, m_nMenuID(-1)
{
	memset(&m_nid, 0, sizeof(m_nid));
}

CTrayIcon::CTrayIcon(	
			CWnd* pWnd,		// 接收任务栏图标区域通告消息的窗口句柄
			UINT uCallbackMessage,// 应用程序定义的消息标示。 
			LPCTSTR szToolTip,	// 提示信息
			HICON hIcon,	// 显示，修改，删除的图标
			UINT nMenuID,	// 右击鼠标时弹出的菜单（如果有的话）
			UINT uID		// 应用程序定义的任务栏图标的标识符。
		 )
{
	Initialize(pWnd, uCallbackMessage, szToolTip, hIcon, nMenuID, uID);
	m_bHidden = FALSE;
}

BOOL CTrayIcon::Initialize(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szToolTip, 
					   HICON hIcon, UINT nMenuID, UINT uID)
{
	// Create an invisible window for the Tray Icon
   // CTrayIcon::CreateEx(0, AfxRegisterWndClass(0), _T(""), WS_POPUP, 0,0,0,0, NULL, 0);

	// this is only for Windows 95 (or higher)
	VERIFY(m_bEnabled = ( GetVersion() & 0xff ) >= 4);
	if (!m_bEnabled) return FALSE;

	//Make sure Notification window is valid
	VERIFY(m_bEnabled = (pWnd && ::IsWindow(pWnd->GetSafeHwnd())));
	if (!m_bEnabled) return FALSE;
	
	//Make sure we avoid conflict with other messages
	ASSERT(uCallbackMessage >= WM_USER);

	//Tray only supports tooltip text up to 64 characters
	ASSERT(_tcslen(szToolTip) <= 64);

	// load up the NOTIFYICONDATA structure
	m_nid.cbSize = sizeof(NOTIFYICONDATA);
	m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_nid.hWnd	 = pWnd->GetSafeHwnd();	// 窗口的句柄。标示的窗口用来接收与托盘图标相关的消息。
	m_nid.uID	 = uID;		// 应用程序定义的任务栏图标的标识符。
	m_nid.hIcon  = hIcon;	
	// uCallbackMessage 应用程序定义的消息标示。系统将使用此标示向由hWnd成员标示的窗口发送消息。
	// wParam参数标示了消息事件发生的任务栏图标，
	// lParam参数根据事件的不同，包含了鼠标或键盘的具体消息。
	m_nid.uCallbackMessage = uCallbackMessage;
	lstrcpy (m_nid.szTip, szToolTip);

	m_nMenuID = nMenuID;

	// Set the tray icon
	VERIFY(m_bEnabled = Shell_NotifyIcon(NIM_ADD, &m_nid));
	return m_bEnabled;
}

CTrayIcon::~CTrayIcon()
{
	RemoveIcon();
}

BOOL CTrayIcon::SetMenu(UINT nMenuID)
{
	m_nMenuID = nMenuID;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTrayIcon icon manipulation

void CTrayIcon::RemoveIcon()
{
	if (!m_bEnabled) return;
	m_nid.uFlags = 0;
    Shell_NotifyIcon(NIM_DELETE, &m_nid);
    m_bEnabled = FALSE;
}

void CTrayIcon::HideIcon()
{
	if (m_bEnabled && !m_bHidden) {
		m_nid.uFlags = NIF_ICON;
		Shell_NotifyIcon (NIM_DELETE, &m_nid);
		m_bHidden = TRUE;
	}
}

void CTrayIcon::ShowIcon()
{
	if (m_bEnabled && m_bHidden) {
		m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		Shell_NotifyIcon(NIM_ADD, &m_nid);
		m_bHidden = FALSE;
	}
}

BOOL CTrayIcon::SetIcon(HICON hIcon)
{
	if (!m_bEnabled) return FALSE;
	m_nid.uFlags = NIF_ICON;
	m_nid.hIcon = hIcon;
	return Shell_NotifyIcon(NIM_MODIFY, &m_nid);
}

BOOL CTrayIcon::SetIcon(LPCTSTR lpszIconName)
{
	HICON hIcon = AfxGetApp()->LoadIcon(lpszIconName);
	return SetIcon(hIcon);
}

BOOL CTrayIcon::SetIcon(UINT nIDResource)
{
	HICON hIcon = AfxGetApp()->LoadIcon(nIDResource);
	return SetIcon(hIcon);
}

BOOL CTrayIcon::SetStandardIcon(LPCTSTR lpIconName)
{
	HICON hIcon = LoadIcon(NULL, lpIconName);
	return SetIcon(hIcon);
}

BOOL CTrayIcon::SetStandardIcon(UINT nIDResource)
{
	HICON hIcon = LoadIcon(NULL, MAKEINTRESOURCE(nIDResource));
	return SetIcon(hIcon);
}
 
HICON CTrayIcon::GetIconHandle() const
{
	HICON hIcon = NULL;
	if (m_bEnabled)
		hIcon = m_nid.hIcon;
	return hIcon;
}

/////////////////////////////////////////////////////////////////////////////
// CTrayIcon tooltip text manipulation

BOOL CTrayIcon::SetTooltipText(LPCTSTR pszTip)
{
	if (!m_bEnabled) return FALSE;
	m_nid.uFlags = NIF_TIP;
	lstrcpy(m_nid.szTip, pszTip);
	return Shell_NotifyIcon(NIM_MODIFY, &m_nid);
}

BOOL CTrayIcon::SetTooltipText(UINT nID)
{
	CString strText(_T(""));
	VERIFY(strText.LoadString(nID));
	return SetTooltipText(strText);
}

CString CTrayIcon::GetTooltipText() const
{
	CString strText(_T(""));
	if (m_bEnabled)
		strText = m_nid.szTip;
	return strText;
}

/////////////////////////////////////////////////////////////////////////////
// CTrayIcon notification window stuff

BOOL CTrayIcon::SetNotificationWnd(CWnd* pWnd)
{
	if (!m_bEnabled) return FALSE;
	//Make sure Notification window is valid
	ASSERT(pWnd && ::IsWindow(pWnd->GetSafeHwnd()));
	m_nid.hWnd = pWnd->GetSafeHwnd();
	m_nid.uFlags = 0;
	return Shell_NotifyIcon(NIM_MODIFY, &m_nid);
}

CWnd* CTrayIcon::GetNotificationWnd() const
{
	return CWnd::FromHandle(m_nid.hWnd);
}

/////////////////////////////////////////////////////////////////////////////
// CTrayIcon implentation of OnTrayNotification
// wParam参数标示了消息事件发生的任务栏图标，
// lParam参数根据事件的不同，包含了鼠标或键盘的具体消息。
LRESULT CTrayIcon::OnTrayNotification(WPARAM wParam, LPARAM lParam) 
{
/*	//Return quickly if its not for this tray icon
	if ((UINT)wParam != m_nid.uID)
		return 0L;

	CMenu menu, *pSubMenu;

	// Clicking with right button brings up a context menu
	if (LOWORD(lParam) == WM_RBUTTONUP)
	{	
		if (!menu.LoadMenu(m_nMenuID)) return 0;
		if (!(pSubMenu = menu.GetSubMenu(0))) return 0;

		// Make first menu item the default (bold font)
		::SetMenuDefaultItem(pSubMenu->m_hMenu, 0, TRUE);

		//Display and track the popup menu
		CPoint pos;
		GetCursorPos(&pos);
		::SetForegroundWindow(m_nid.hWnd);  
		::TrackPopupMenu(pSubMenu->m_hMenu, 0, pos.x, pos.y, 0, m_nid.hWnd, NULL);

		//pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this, NULL);
		menu.DestroyMenu();
	} 
	else if (LOWORD(lParam) == WM_LBUTTONDBLCLK) 
	{
		AfxGetMainWnd()->ShowWindow(SW_MINIMIZE);
		AfxGetMainWnd()->ShowWindow(SW_RESTORE);	// 显示主窗口
		AfxGetMainWnd()->BringWindowToTop();
		//AfxGetMainWnd()->ShowWindow(SW_SHOW);	// 显示主窗口
// 		RemoveIcon();	// 删除任务栏的图标
	}

*/	return 1L;

}

UINT CTrayIcon::GetTrayIconID() const
{
	return m_nid.uID;
}