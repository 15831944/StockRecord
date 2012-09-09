
// StockRecordDlg.h : 头文件
//

#pragma once

#include "GridCtrl_src/GridCtrl.h"
#include "e:\vc++\stockapps\stockrecord\gridctrl_src\gridctrl.h"

// CStockRecordDlg 对话框
class CStockRecordDlg : public CDialogEx
{
// 构造
public:
	CStockRecordDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_STOCKRECORD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CGridCtrl m_GridCtrl;
};
