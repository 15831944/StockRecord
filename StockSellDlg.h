#pragma once
#include "atlcomtime.h"
#include "afxwin.h"

// CStockSellDlg 对话框

class CStockSellDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStockSellDlg)

public:
	CStockSellDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStockSellDlg();

// 对话框数据
	enum { IDD = IDD_SELL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtCalcuEarn();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSelchangeComboEarn();
	afx_msg void OnSelchangeComboLoss();
	DECLARE_MESSAGE_MAP()

public:
	CString m_strCode;
	CString m_strName;
	int m_nSellAmount;
	float m_fSellPrice;
	COleDateTime m_oleSellDate;

private:
	float m_fEachEarn;

	CString m_strEachEarn;
	CComboBox m_comboEarn;
	CString m_strEarnPrice;
	CComboBox m_comboLoss;
	CString m_strLossPrice;
public:
	float GetThresholdPriceFromCString(CString str);
	
private:
	/**
	 *	m_nHoldAmount is assigned from HoldModel.
	 *  The m_nSellAmount must be not greater than m_nHoldAmount.
	 */
	int m_nHoldAmount;

	/**
	 *	Assigned from holdModel, used when calculate each_earn and threshold_price.
	 */
	float m_fHoldCost;

	/* Its value is set from CStockRecordDlg::OnStockholdSell().
	 * If m_bIsPlanSell is true, then the sellDlg should disable
	 * "Sell" button in OnShowWindow() function before dlg is displayed.
	 */
	bool m_bIsPlanSell;

public:
	void SetHoldAmount(int holdAmount)	{m_nHoldAmount = holdAmount;}
	void SetHoldCost(float fHoldCost)	{m_fHoldCost = fHoldCost;}
	void SetIsPlanSell(bool isPlanSell)	{m_bIsPlanSell = isPlanSell;}
};
