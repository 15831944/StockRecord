#pragma once


// CStockRecordLoginDlg dialog

class CStockRecordLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStockRecordLoginDlg)

public:
	CStockRecordLoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStockRecordLoginDlg();

// Dialog Data
	enum { IDD = IDD_LOGIN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	DECLARE_MESSAGE_MAP()

private:
	/* values from dialog input. */
	CString m_strInputUserName;
	CString m_strInputPassword;

	/* values from database ... */
	CString m_strUserName;
	CString m_strPassword;

private:
	void SetUserName(CString userName)		{m_strUserName = userName;}
	CString GetUserName(void)				{return m_strUserName;}
	void SetPassword(CString password)		{m_strPassword = password;}
	CString GetPassword(void)				{return m_strPassword;}
public:
	
};
