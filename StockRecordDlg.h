
// StockRecordDlg.h : 头文件
//

#pragma once

#include "GridCtrl_src/GridCtrl.h"
extern "C" {
	#include <sqlite3.h>	// SQLite support.
};

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
	afx_msg void OnClose(void);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

	
public:
	/** Read records from tables. */
	int ReadStockHoldRecords(void);

	/** Set up database and tables' names, must be called at startup. */
	int SetupDBTableNames(void);

	/** Open database, better be called at startup. */
	int OpenDatabase(void);

	/** Close database, must be called when dialog goes away. */
	int CloseDatabase(void);

private:
	
	CGridCtrl m_GridCtrl;

	/** names of database and tables */
	string m_strDBName;
	string m_strBuyTableName;
	string m_StrHoldTableName;
	string m_strSellTableName;
	string m_strMoneyTableName;

	sqlite3* m_pDatabase;
public:
	
	
};
