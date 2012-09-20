#pragma once
#include "atlcomtime.h"


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

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtCalcuEarn();
	afx_msg void OnBnClickedCancel();
	DECLARE_MESSAGE_MAP()

public:
	CString m_strCode;
	CString m_strName;
	int m_nSellAmount;
	float m_fSellPrice;
	float m_fEachEarn;
	COleDateTime m_fSellDate;

	/**
	 *	m_nHoldAmount is assigned from HoldModel.
	 *  The m_nSellAmount must be not greater than m_nHoldAmount.
	 */
	int m_nHoldAmount;

public:
	void SetHoldAmount(int holdAmount);

	virtual BOOL OnInitDialog();
};
