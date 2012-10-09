// StockRecordLoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../StockRecord.h"
#include "StockRecordLoginDlg.h"
#include "afxdialogex.h"


// CStockRecordLoginDlg dialog

IMPLEMENT_DYNAMIC(CStockRecordLoginDlg, CDialogEx)

CStockRecordLoginDlg::CStockRecordLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStockRecordLoginDlg::IDD, pParent)
	, m_strUserName(_T(""))
	, m_strPassword(_T(""))
	, m_strInputUserName(_T("loxin"))
	, m_strInputPassword(_T(""))
{
}

CStockRecordLoginDlg::~CStockRecordLoginDlg()
{
}

void CStockRecordLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strInputUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strInputPassword);
}


BEGIN_MESSAGE_MAP(CStockRecordLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CStockRecordLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CStockRecordLoginDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

// CStockRecordLoginDlg message handlers

void CStockRecordLoginDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	
	if (m_strInputUserName.IsEmpty() || m_strInputPassword.IsEmpty()) {
		MessageBox("用户名/密码不能为空，请重新输入。", "Oops.");
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_USERNAME);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
		return ;
	}

	if (m_strInputUserName.Compare(GetUserName()) != 0 ||
		m_strInputPassword.Compare(GetPassword()) != 0) {
		MessageBox("用户名/密码不正确，请重新输入。", "Oops.");
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_USERNAME);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
		return ;
	}

	/* Username and password have matched, return IDOK . */
	CDialogEx::OnOK();
}

void CStockRecordLoginDlg::OnBnClickedCancel()
{
	// Exit application.
	CDialogEx::OnCancel();
}

BOOL CStockRecordLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	/* Init username and password, better to get values from database. */
	SetUserName("loxin");
	SetPassword("8851");

	/* Set focus to username input. */
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PASSWORD);
	pEdit->SetSel(0, -1);
	pEdit->SetFocus();

	return FALSE;  /* Return FALSE to set focus on input edit. */
	
	// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CStockRecordLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if(pMsg->wParam == VK_RETURN) {
			UINT nID = GetFocus()->GetDlgCtrlID();
			switch (nID) {
			/* When enter is pressed, move the focus to password edit only if 
			 * username edit is not empty.
			 */
			case IDC_EDIT_USERNAME:
				UpdateData(TRUE);
				if (!m_strInputUserName.IsEmpty() && m_strInputPassword.IsEmpty()) {
					NextDlgCtrl();
					return TRUE;
				}
				break;

			default:
				break;
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
