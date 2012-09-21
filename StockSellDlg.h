#pragma once
#include "atlcomtime.h"

#include "StockModel.h"


// CStockSellDlg �Ի���

class CStockSellDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStockSellDlg)

public:
	CStockSellDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStockSellDlg();

// �Ի�������
	enum { IDD = IDD_SELL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

private:
	/**
	 *	m_nHoldAmount is assigned from HoldModel.
	 *  The m_nSellAmount must be not greater than m_nHoldAmount.
	 */
	int m_nHoldAmount;

	/**
	 *	Pointer point to stockHoldModel object.
	 *  Used for calculate even price with object's hold_cost.
	 */
	CStockHoldModel* m_pHoldModel;

public:
	void SetHoldAmount(int holdAmount);
	void SetHoldModel(CStockHoldModel* pHoldModel);

	virtual BOOL OnInitDialog();
};
