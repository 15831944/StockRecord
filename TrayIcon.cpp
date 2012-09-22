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
			CWnd* pWnd,		// ����������ͼ������ͨ����Ϣ�Ĵ��ھ��
			UINT uCallbackMessage,// Ӧ�ó��������Ϣ��ʾ�� 
			LPCTSTR szToolTip,	// ��ʾ��Ϣ
			HICON hIcon,	// ��ʾ���޸ģ�ɾ����ͼ��
			UINT nMenuID,	// �һ����ʱ�����Ĳ˵�������еĻ���
			UINT uID		// Ӧ�ó������������ͼ��ı�ʶ����
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
	m_nid.hWnd	 = pWnd->GetSafeHwnd();	// ���ڵľ������ʾ�Ĵ�����������������ͼ����ص���Ϣ��
	m_nid.uID	 = uID;		// Ӧ�ó������������ͼ��ı�ʶ����
	m_nid.hIcon  = hIcon;	
	// uCallbackMessage Ӧ�ó��������Ϣ��ʾ��ϵͳ��ʹ�ô˱�ʾ����hWnd��Ա��ʾ�Ĵ��ڷ�����Ϣ��
	// wParam������ʾ����Ϣ�¼�������������ͼ�꣬
	// lParam���������¼��Ĳ�ͬ��������������̵ľ�����Ϣ��
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
// wParam������ʾ����Ϣ�¼�������������ͼ�꣬
// lParam���������¼��Ĳ�ͬ��������������̵ľ�����Ϣ��
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
		AfxGetMainWnd()->ShowWindow(SW_RESTORE);	// ��ʾ������
		AfxGetMainWnd()->BringWindowToTop();
		//AfxGetMainWnd()->ShowWindow(SW_SHOW);	// ��ʾ������
// 		RemoveIcon();	// ɾ����������ͼ��
	}

*/	return 1L;

}

UINT CTrayIcon::GetTrayIconID() const
{
	return m_nid.uID;
}