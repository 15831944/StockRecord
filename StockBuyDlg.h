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
	DECLARE_MESSAGE_MAP()

public:
	CString m_strCode;
	CString m_strName;
	float m_fBuyPrice;
	int m_nBuyAmount;
	COleDateTime m_oleDataTime;

	enum STOCK_TYPE {
		STOCK_TYPE_SHANG_HAI = 0,
		STOCK_TYPE_SHEN_ZHEN
	} m_eStockType;

};
