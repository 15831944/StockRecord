#pragma once
#include "atlcomtime.h"


// CStockBuyDlg 对话框

class CStockBuyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStockBuyDlg)

public:
	CStockBuyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStockBuyDlg();

// 对话框数据
	enum { IDD = IDD_BUY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

public:
	CString m_strCode;
	CString m_strName;
	float m_fBuyPrice;
	int m_nBuyAmount;
	COleDateTime m_oleDataTime;

	bool m_bStockType;

};
