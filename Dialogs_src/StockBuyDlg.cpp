// StockBuyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../StockRecord.h"
#include "StockBuyDlg.h"
#include "afxdialogex.h"


// CStockBuyDlg �Ի���

IMPLEMENT_DYNAMIC(CStockBuyDlg, CDialogEx)

CStockBuyDlg::CStockBuyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStockBuyDlg::IDD, pParent)
	, m_strCode(_T(""))
	, m_strName(_T(""))
	, m_fBuyPrice(0)
	, m_nBuyAmount(0)
	, m_oleDataTime(COleDateTime::GetCurrentTime())
	, m_bStockType(STOCK_TYPE_SHANG_HAI)
{

}

CStockBuyDlg::~CStockBuyDlg()
{
}

void CStockBuyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BUY_CODE, m_strCode);
	DDV_MaxChars(pDX, m_strCode, 6);
	DDX_Text(pDX, IDC_EDIT_BUY_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 10);
	DDX_Text(pDX, IDC_EDIT_BUY_PRICE, m_fBuyPrice);
	DDV_MinMaxFloat(pDX, m_fBuyPrice, (float)0.01, (float)10000.0);
	DDX_Text(pDX, IDC_EDIT_BUY_AMOUNT, m_nBuyAmount);
	DDV_MinMaxInt(pDX, m_nBuyAmount, 1, 1000000);
	DDX_DateTimeCtrl(pDX, IDC_BUY_DATE, m_oleDataTime);
}


BEGIN_MESSAGE_MAP(CStockBuyDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CStockBuyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CStockBuyDlg::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CStockBuyDlg ��Ϣ�������


void CStockBuyDlg::OnBnClickedOk()
{
	/** Get data from dialog, and check the data. */
	if (!UpdateData(TRUE))
		return;

	/* Code must have 6 chars, and the first one char must be '0' or '6'. */
	if (m_strCode.IsEmpty() || m_strCode.GetLength() != 6 || 
		!(m_strCode.GetAt(0) == '0' || m_strCode.GetAt(0) == '6' 
			|| m_strCode.GetAt(0) == '3')) {
		MessageBox("��Ʊ���벻��ȷ", "Oops");
		CEdit* pEdit = (CEdit*) GetDlgItem(IDC_EDIT_BUY_CODE);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
		return ;
	}

	/* Check whether stock is ShangHai or ShenZhen */
	if (m_strCode.GetAt(0) == '6')
		m_bStockType = STOCK_TYPE_SHANG_HAI;
	else
		m_bStockType = STOCK_TYPE_SHEN_ZHEN;
	
	/* Check name */
	if (m_strName.IsEmpty()) {
		MessageBox("��Ʊ���Ʋ���Ϊ�գ�","Oops!");
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_BUY_NAME);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
		return ;
	}

	CDialogEx::OnOK();
}


void CStockBuyDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CStockBuyDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	/* Make the sell price edit focused */
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_BUY_CODE);
	pEdit->SetSel(0, -1);
	pEdit->SetFocus(); 
}
