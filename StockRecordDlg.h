
// StockRecordDlg.h : 头文件
//

#pragma once

#include "GridCtrl_src/GridCtrl.h"
extern "C" {
	#include <sqlite3.h>	// SQLite support.
};

#include "StockModel.h"
#include "StockBuyDlg.h"

#include <vector>
using namespace std;

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
	afx_msg void OnMenuHoldRecord();
	afx_msg void OnMenuBuyRecord();
	afx_msg void OnMenuSellRecord();
	afx_msg void OnMenuMoneyRecord();
	afx_msg void OnBnClickedExit();
	afx_msg void OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnMenuRemoveRecord(UINT uid);
	afx_msg void OnStockbuyAdd();
	afx_msg void OnStockholdPlanSell();
	afx_msg void OnStockholdSell();
	afx_msg void OnStockmoneyInout();
	DECLARE_MESSAGE_MAP()

private:
	BOOL IsTableNamesValid(void);

	/** Get the left top item's data to display according opened table now */
	void SetLeftTopItemData(void);

	std::string GetActiveTableName(void);
	int GetActiveRecordIdBySeqNo(int seqNo);

	/** Check out if @cellRanged is valid */
	BOOL IsCellRangeValid(const CCellRange& cellRange);
	BOOL IsFocusedCellInSelectedRows(
		const CCellID& focusedCell, const CCellRange& cellRange);

	/* Return a real model, not model's reference. */
	CStockBuyModel	 ConvertDlgDataToBuyModel(const CStockBuyDlg& buyDlg);
	CStockHoldModel  ConvertBuyModelToHoldModel(const CStockBuyModel&);
//	CStockSellModel  ConvertHoldModelToSellModel(const CStockHoldModel&);
	CStockMoneyModel ConvertDlgDataToMoneyModel();

public:
	/** Set gird with data queried from sqlite3_get_table(). */
	int SetGirdData(int nRow, int nCol, char** result);

	/** Read records from tables. */
	int QueryRecordsByTableName(const char* tableName);

	/** Reload table records according to m_enumRecordTable */
	int ReloadRecords(void);

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

	enum RECORDTABLE{		// which table is displayed now.
		T_STOCKBUY = 0,		// stock_buy table
		T_STOCKHOLD,		// stock_hold table
		T_STOCKSELL,		// stock_sell table
		T_STOCKMONEY		// stock_money table
	} m_enumRecordTable;

	/**
	 *	Store table's ids, the seqNo is the index of vector.
	 */
	vector<int> m_vecStockBuyIds;
	vector<int> m_vecStockHoldIds;
	vector<int> m_vecStockSellIds;
	vector<int> m_vecStockMoneyIds;

	/** Clear related vector according by m_enumRecordTable to do a new display. */
	void ClearRecordIds(void);

	/**
	 *	Store record id. The 'seqNo' is sequence number, which can be 
	 *  determined by the index of vector. This will cause gird can not order.
	 *  This method may not be good, but it can work in this condition.
	 */
	void StoreRecordId(int id);

public:


};
