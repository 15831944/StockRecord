/////////////////////////////////////////////////////////////////////////////
// TrayIcon.h : header file

#ifndef _INCLUDED_TRAYICON_H_
#define _INCLUDED_TRAYICON_H_
#define WM_ICON_NOTIFY	WM_USER + 10	// �û���Ҫ�Ը���Ϣ������Ӧ��

class CTrayIcon : public CWnd
{
// Construction/destruction
public:
	CTrayIcon();
	CTrayIcon(	CWnd* pWnd,		// ����������ͼ������ͨ����Ϣ�Ĵ��ھ��
				UINT uCallbackMessage,// Ӧ�ó��������Ϣ��ʾ�� 
				LPCTSTR szTip,	// ��ʾ��Ϣ
				HICON hIcon,	// ��ʾ���޸ģ�ɾ����ͼ��
				UINT nMenuID,	// �һ����ʱ�����Ĳ˵�������еĻ���
				UINT uID		// Ӧ�ó������������ͼ��ı�ʶ����
			 );
	virtual ~CTrayIcon();

// Operations
public:
	BOOL Enabled() { return m_bEnabled; }
	BOOL Visible() { return !m_bHidden; }

	//Create the tray icon
	int Initialize(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, UINT nMenuID, UINT uID);

	// Change or retrieve the Tooltip text
	BOOL    SetTooltipText(LPCTSTR pszTooltipText);
	BOOL    SetTooltipText(UINT nID);
	CString GetTooltipText() const;

	// Change or retrieve the icon displayed
	BOOL  SetIcon(HICON hIcon);
	BOOL  SetIcon(LPCTSTR lpIconName);
	BOOL  SetIcon(UINT nIDResource);
	BOOL  SetStandardIcon(LPCTSTR lpIconName);
	BOOL  SetStandardIcon(UINT nIDResource);
	HICON GetIconHandle() const;
	UINT  GetTrayIconID() const;
	void  HideIcon();
	void  ShowIcon();
	void  RemoveIcon();

	// ��������ͼ�����Ҽ������Ĳ˵�ID
	BOOL  SetMenu(UINT nMenuID);

	// Change or retrieve the window to send notification messages to
	BOOL  SetNotificationWnd(CWnd* pNotifyWnd);
	CWnd* GetNotificationWnd() const;

	// Default handler for tray notification message
	LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrayIcon)
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL			m_bEnabled;	// does O/S support tray icon?
	BOOL			m_bHidden;	// Has the icon been hidden?
	NOTIFYICONDATA	m_nid;
	UINT			m_nMenuID;	

	DECLARE_DYNAMIC(CTrayIcon)
};


#endif

/////////////////////////////////////////////////////////////////////////////
