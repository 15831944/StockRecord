// StockSellDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StockRecord.h"
#include "StockSellDlg.h"
#include "afxdialogex.h"


// CStockSellDlg 对话框

IMPLEMENT_DYNAMIC(CStockSellDlg, CDialogEx)

CStockSellDlg::CStockSellDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStockSellDlg::IDD, pParent)
	, m_strCode(_T(""))
	, m_strName(_T(""))
	, m_nSellAmount(0)
	, m_fSellPrice(0)
	, m_fEachEarn(0)
	, m_fSellDate(COleDateTime::GetCurrentTime())
	, m_nHoldAmount(0)
{
}

CStockSellDlg::~CStockSellDlg()
{
}

void CStockSellDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SELL_CODE, m_strCode);
	DDV_MaxChars(pDX, m_strCode, 6);
	DDX_Text(pDX, IDC_EDIT_SELL_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_SELL_AMOUNT, m_nSellAmount);
	DDV_MinMaxInt(pDX, m_nSellAmount, 1, 10000000);
	DDX_Text(pDX, IDC_EDIT_SELL_PRICE, m_fSellPrice);
	DDV_MinMaxFloat(pDX, m_fSellPrice, 0.01, 100000.0);
	DDX_Text(pDX, IDC_EDIT_EACH_EARN, m_fEachEarn);
	DDX_DateTimeCtrl(pDX, IDC_SELL_DATE, m_fSellDate);
}


BEGIN_MESSAGE_MAP(CStockSellDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CStockSellDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_BT_CALCU_EARN, &CStockSellDlg::OnBnClickedBtCalcuEarn)
	ON_BN_CLICKED(IDCANCEL, &CStockSellDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CStockSellDlg 消息处理程序


void CStockSellDlg::OnBnClickedOk()
{
	// TODO: OnBnClickedOk
	if (!UpdateData(TRUE))
		return;

	if (m_nSellAmount > m_nHoldAmount) {
		MessageBox("卖出数量超过持仓数量。", "Oops.");
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SELL_AMOUNT);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
		return;
	}

	CDialogEx::OnOK();
}

void CStockSellDlg::OnBnClickedBtCalcuEarn()
{
}

void CStockSellDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CStockSellDlg::SetHoldAmount( int holdAmount )
{
	m_nHoldAmount = holdAmount;
}

BOOL CStockSellDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	/* Make the sell price edit focused */
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SELL_PRICE);
	pEdit->SetSel(0, -1);
	pEdit->SetFocus();
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

