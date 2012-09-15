
// StockRecordDlg.h : ͷ�ļ�
//

#pragma once

#include "GridCtrl_src/GridCtrl.h"
extern "C" {
	#include <sqlite3.h>	// SQLite support.
};

// CStockRecordDlg �Ի���
class CStockRecordDlg : public CDialogEx
{
// ����
public:
	CStockRecordDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_STOCKRECORD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose(void);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

private:
	BOOL IsTableNamesValid(void);

public:
	/** Read records from tables. */
	int QeuryRecordsByTableName(const char* tableName);

	/** Set up database and tables' names, must be called at startup. */
	int SetupDBTableNames(void);

	/** Open database, if it doesn't exist, create & init database. */
	int OpenDatabase(void);

	/** Close database, must be called when dialog goes away. */
	int CloseDatabase(void);

	/** Initiate database's tables, should be called only once. */
	int InitDatabaseTables(void);

private:
	
	CGridCtrl m_GridCtrl;

	/** names of database and tables */
	string	m_strDBName;
	string	m_strBuyTableName;
	string	m_StrHoldTableName;
	string	m_strSellTableName;
	string	m_strMoneyTableName;

	sqlite3* m_pDatabase;
	int		m_nDBStatus;
public:
	
	
};
