#include "StdAfx.h"
#include "..\StockRecord.h"
#include "StockSellDlg.h"
#include "afxdialogex.h"
#include "..\StockCalculate.h"

// CStockSellDlg 对话框

IMPLEMENT_DYNAMIC(CStockSellDlg, CDialogEx)

CStockSellDlg::CStockSellDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStockSellDlg::IDD, pParent)
	, m_strCode(_T(""))
	, m_strName(_T(""))
	, m_nSellAmount(0)
	, m_fSellPrice(0)
	, m_fEachEarn(0)
	, m_oleSellDate(COleDateTime::GetCurrentTime())
	, m_nHoldAmount(0)
	, m_fHoldCost(0.0f)
	, m_strEachEarn(_T("0"))
	, m_bIsPlanSell(false)
	, m_strWndTitle(_T(""))
	, m_strEarnPrice(_T(""))
	, m_strLossPrice(_T(""))
{
}

CStockSellDlg::~CStockSellDlg()
{
	m_fHoldCost = 0.0f;
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
	DDV_MinMaxFloat(pDX, m_fSellPrice, 0.01f, 100000.0f);
	DDX_Text(pDX, IDC_EDIT_EACH_EARN, m_strEachEarn);
	DDX_DateTimeCtrl(pDX, IDC_SELL_DATE, m_oleSellDate);
	DDX_Control(pDX, IDC_COMBO_EARN, m_comboEarn);
	DDX_Control(pDX, IDC_COMBO_LOSS, m_comboLoss);
	DDX_Text(pDX, IDC_EDIT_SELL_EARN_PRICE, m_strEarnPrice);
	DDX_Text(pDX, IDC_EDIT_SELL_LOSS_PRICE, m_strLossPrice);
}

BEGIN_MESSAGE_MAP(CStockSellDlg, CDialogEx)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDOK, &CStockSellDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_BT_CALCU_EARN, &CStockSellDlg::OnBnClickedBtCalcuEarn)
	ON_BN_CLICKED(IDCANCEL, &CStockSellDlg::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO_EARN, &CStockSellDlg::OnSelchangeComboEarn)
	ON_CBN_SELCHANGE(IDC_COMBO_LOSS, &CStockSellDlg::OnSelchangeComboLoss)
END_MESSAGE_MAP()

// CStockSellDlg 消息处理程序

void CStockSellDlg::OnBnClickedOk()
{
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

/**
 *	m_pHoldModel MUST be an valid pointer, which points a holdModel object.
 */
void CStockSellDlg::OnBnClickedBtCalcuEarn()
{
	if (m_fHoldCost <= 0.0f || !UpdateData(TRUE))
		return;

	if (m_nSellAmount > m_nHoldAmount) {
		MessageBox("卖出数量超过持仓数量。", "Oops.");
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SELL_AMOUNT);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
		return;
	}

	/* calculate each earn */
	bool bStockType = ('6' == m_strCode.GetAt(0)) ?	\
		STOCK_TYPE_SHANG_HAI : STOCK_TYPE_SHEN_ZHEN;
	CStockFees stockFees;
	m_fEachEarn = 
		stockFees.CalcuEachEarn(bStockType, m_fHoldCost, m_nSellAmount, m_fSellPrice);

	/* calculate rate(%) of each earn. may not be exact. */
	float fEarnRate = (m_fSellPrice - m_fHoldCost) / m_fHoldCost * 100.0f;
	m_strEachEarn.Format("%.2f(%.2f%%)", m_fEachEarn, Round(fEarnRate, 2));

	UpdateData(FALSE);
}

void CStockSellDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

BOOL CStockSellDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_comboEarn.ResetContent();
	m_comboLoss.ResetContent();

	CString strValue("");
	for (int i = 10; i >= 1; --i) {
		strValue.Empty();
		strValue.Format("%d%%", i);
		m_comboEarn.AddString((LPCTSTR)strValue);
	}

	for (int i = 10; i >= 1; --i) {
		strValue.Empty();
		strValue.Format("-%d%%", i);
		m_comboLoss.AddString((LPCTSTR)strValue);
	}
	
	m_comboEarn.SetCurSel(5);
	m_comboLoss.SetCurSel(5);
	OnSelchangeComboEarn();
	OnSelchangeComboLoss();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CStockSellDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	/* Make the sell price edit focused */
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SELL_PRICE);
	pEdit->SetSel(0, -1);
	pEdit->SetFocus(); 

	/* Disable "Sell" button if m_bIsPlanSell is true. */
	if (m_bIsPlanSell) {
		m_strWndTitle = _T("计划卖出");
		CButton* pButton = (CButton*)GetDlgItem(IDOK);
		if (pButton)
			pButton->EnableWindow(FALSE);

		m_bIsPlanSell = false;/* Make it be false regardless of its origin value. */
	} else {
		m_strWndTitle = _T("卖出股票");
	}

	SetWindowText((LPCTSTR)m_strWndTitle);
}

/**
 *	str will be like "5%" or "-5%".
 *  The returned value will be : m_fHoldCost * (1.0f + "5%").
 */
float CStockSellDlg::GetThresholdPriceFromCString( CString strPercent )
{
	strPercent.Trim();
	if (strPercent.IsEmpty())
		return 0.0f;

	strPercent.Trim('%');		// Get rid of the last '%'
	float fPercent = (float)atof((LPCTSTR)strPercent) / 100;
	float fPrice = m_fHoldCost * (fPercent + 1.0f);

	return fPrice;
}

void CStockSellDlg::OnSelchangeComboEarn()
{
	CString strComboEarn;
	m_comboEarn.GetWindowTextA(strComboEarn);
	float fEarnPrice = GetThresholdPriceFromCString(strComboEarn);
	m_strEarnPrice.Format("%.2f", Round(fEarnPrice, 2, ROUND_4S5R));
	UpdateData(FALSE);
}

void CStockSellDlg::OnSelchangeComboLoss()
{
	CString strComboLoss;
	m_comboLoss.GetWindowTextA(strComboLoss);
	float fLossPrice = GetThresholdPriceFromCString(strComboLoss);
	m_strLossPrice.Format("%.2f", Round(fLossPrice, 2, ROUND_4S5R));
	UpdateData(FALSE);
}

void CStockSellDlg::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	CRect rectEarnEdit, rectLossEdit;
	bool isDbClickOnPriceEdit = false;

	/*
	 * Assign m_fSellPrice with value from m_strEarnPrice(edit) or
	 * m_strLossProce(edit) when left double click is triggered.
	 */
	GetDlgItem(IDC_EDIT_SELL_EARN_PRICE)->GetWindowRect(rectEarnEdit);
	ScreenToClient(&rectEarnEdit);
	GetDlgItem(IDC_EDIT_SELL_LOSS_PRICE)->GetWindowRect(rectLossEdit);
	ScreenToClient(&rectLossEdit);

	if (rectEarnEdit.PtInRect(point)) {
		m_fSellPrice = (float)atof((LPCTSTR)m_strEarnPrice);
		isDbClickOnPriceEdit = true;
	} else if (rectLossEdit.PtInRect(point)) {
		m_fSellPrice = (float)atof((LPCTSTR)m_strLossPrice);
		isDbClickOnPriceEdit = true;
	}

	if (isDbClickOnPriceEdit) {
		UpdateData(FALSE);	/* member variable -> edit */
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SELL_PRICE);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
		return ;
	}

	CDialogEx::OnLButtonDblClk(nFlags, point);
}

