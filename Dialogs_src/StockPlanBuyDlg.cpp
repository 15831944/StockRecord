#include "stdafx.h"
#include "../StockRecord.h"
#include "StockPlanBuyDlg.h"
#include "afxdialogex.h"

#include "../StockModel.h"
#include "../StockDBConnection.h"
#include "../StockCalculate.h"

// CStockPlanBuyDlg dialog

IMPLEMENT_DYNAMIC(CStockPlanBuyDlg, CDialogEx)

CStockPlanBuyDlg::CStockPlanBuyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStockPlanBuyDlg::IDD, pParent)
	, m_sPlanBuyCode(_T(""))
	, m_fPlanBuyPrice(0)
	, m_nPlanBuyAmount(0)
	, m_nPlanBuyHoldAmount(0)
	, m_fPlanBuyHoldCost(0)
	, m_fPlanBuyEvenPrice(0)
	, m_pDBConn(NULL)
	, m_bStockType(STOCK_TYPE_SHANG_HAI)
{
}

CStockPlanBuyDlg::~CStockPlanBuyDlg()
{
	m_pDBConn = NULL;
}

void CStockPlanBuyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PLANBUY_CODE, m_sPlanBuyCode);
	DDV_MaxChars(pDX, m_sPlanBuyCode, 6);
	DDX_Text(pDX, IDC_EDIT_PLANBUY_PRICE, m_fPlanBuyPrice);
	DDV_MinMaxFloat(pDX, m_fPlanBuyPrice, (float)0.01, (float)999999.0);
	DDX_Text(pDX, IDC_EDIT_PLANBUY_AMOUT, m_nPlanBuyAmount);
	DDV_MinMaxInt(pDX, m_nPlanBuyAmount, 1, 9999999);
	DDX_Text(pDX, IDC_EDIT_PLANBUY_HOLD_AMOUNT, m_nPlanBuyHoldAmount);
	DDX_Text(pDX, IDC_EDIT_PLANBUY_HOLD_COST, m_fPlanBuyHoldCost);
	DDX_Text(pDX, IDC_EDIT_PLANBUY_EVEN_PRICE, m_fPlanBuyEvenPrice);
}

BEGIN_MESSAGE_MAP(CStockPlanBuyDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CStockPlanBuyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CStockPlanBuyDlg::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CStockPlanBuyDlg message handlers

BOOL CStockPlanBuyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(FALSE);

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PLANBUY_CODE);
	pEdit->SetSel(0, -1);
	pEdit->SetFocus();

	return FALSE;  
	// return TRUE, unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CStockPlanBuyDlg::OnBnClickedOk()
{
	if (!UpdateData(TRUE))
		return;

	/* Code must have 6 chars, and the first one char must be '0' or '6'. */
	if (m_sPlanBuyCode.IsEmpty() || m_sPlanBuyCode.GetLength() != 6 || 
		!(m_sPlanBuyCode.GetAt(0) == '0' || m_sPlanBuyCode.GetAt(0) == '6' 
		|| m_sPlanBuyCode.GetAt(0) == '3')) {
			
		MessageBox("股票代码不正确", "Oops");
		CEdit* pEdit = (CEdit*) GetDlgItem(IDC_EDIT_PLANBUY_CODE);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
		return ;
	}

	/* Check whether stock is ShangHai or ShenZhen */
	if (m_sPlanBuyCode.GetAt(0) == '6')
		m_bStockType = STOCK_TYPE_SHANG_HAI;
	else
		m_bStockType = STOCK_TYPE_SHEN_ZHEN;

	HandlePlanBuy();

	// CDialogEx::OnOK();
}

void CStockPlanBuyDlg::OnBnClickedCancel()
{
	/* Clear all the input & output of edits, if necessary. */

	CDialogEx::OnCancel();
}

void CStockPlanBuyDlg::HandlePlanBuy( void )
{
	if (!m_pDBConn)
		return ;

	/* Select holdModel by the plan buy stock code to see if the stock exists. */
	CDataBaseConnection* pDBConn = (CDataBaseConnection*)m_pDBConn;
	CStockModelHold holdModel = 
		pDBConn->SelectStockHoldModelByCode((LPCTSTR)m_sPlanBuyCode);
	bool isHoldTableHasSameStock = holdModel.id > 0 ? true : false;

	/* Calculate hold cost and even price. */
	CStockFees stockFees;
	m_nPlanBuyHoldAmount = m_nPlanBuyAmount;
	m_fPlanBuyHoldCost = stockFees.CalculateHoldCostByBuy(m_bStockType,\
		m_fPlanBuyPrice, m_nPlanBuyAmount);
	m_fPlanBuyEvenPrice = stockFees.CalcuEvenPriceByHold(m_bStockType, \
		m_fPlanBuyHoldCost, m_nPlanBuyHoldAmount);

	if (isHoldTableHasSameStock) {
		float fPreHoldCost = (float)atof((LPCTSTR)holdModel.hold_cost);
		int nPreHoldAmount = atoi((LPCTSTR)holdModel.hold_amount);
		
		m_nPlanBuyHoldAmount = nPreHoldAmount + m_nPlanBuyAmount;
		m_fPlanBuyHoldCost = (fPreHoldCost * nPreHoldAmount
			+ m_fPlanBuyHoldCost * m_nPlanBuyAmount) / m_nPlanBuyHoldAmount;

		m_fPlanBuyEvenPrice = stockFees.CalcuEvenPriceByHold(m_bStockType,
			m_fPlanBuyHoldCost, m_nPlanBuyHoldAmount);
	}

	m_fPlanBuyHoldCost = Round(m_fPlanBuyHoldCost, 3);
	m_fPlanBuyEvenPrice = Round(m_fPlanBuyEvenPrice, 2);

	UpdateData(FALSE);
}

void CStockPlanBuyDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	UpdateData(FALSE);
	/* Make the plan buy code edit focused */
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PLANBUY_CODE);
	pEdit->SetSel(0, -1);
	pEdit->SetFocus();
}

/**
 *	Overwrite this function to SetFocus to m_sPlanBuyCode(edit).
 */
BOOL CStockPlanBuyDlg::ShowWindow( int nCmdSHow )
{
	m_fPlanBuyPrice = 0.0f;
	m_nPlanBuyHoldAmount = 0;
	m_fPlanBuyHoldCost = 0;
	m_fPlanBuyEvenPrice = 0;

	BOOL ret = CDialogEx::ShowWindow(nCmdSHow);
	UpdateData(FALSE);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PLANBUY_CODE);
	pEdit->SetSel(0, -1);
	pEdit->SetFocus();

	return ret;
}
