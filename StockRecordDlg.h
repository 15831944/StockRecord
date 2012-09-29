
// StockRecordDlg.h : 头文件
//

#pragma once

#include "GridCtrl_src/GridCtrl.h"

#include "StockDBConnection.h"
#include "StockModel.h"
#include "Dialogs_Src/StockBuyDlg.h"
#include "Dialogs_Src/StockSellDlg.h"
#include "TrayIcon.h"

#include <vector>
using namespace std;

// CStockRecordDlg 对话框
class CStockRecordDlg : public CDialogEx
{
// 构造
public:
	CStockRecordDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CStockRecordDlg(void);

// 对话框数据
	enum { IDD = IDD_STOCKRECORD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
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
	afx_msg void OnGridDBClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnMenuRemoveRecord(UINT uid);
	afx_msg void OnStockbuyAdd();
	afx_msg void OnStockholdPlanSell();
	afx_msg void OnStockholdSell();
	afx_msg void OnStockmoneyInout();
	afx_msg void OnBnClickedBtAbout();
	afx_msg void OnMenuAbout();
	afx_msg void OnMenuAlwaysTop();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg HRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);// 响应 TrayIcon 的消息
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMenuTrayiconShowwd();
	afx_msg void OnMenuTrayiconExit();
	DECLARE_MESSAGE_MAP()

private:
	void MakeMenuItemCheckedByActiveTable(void);

	/** Get the left top item's data to display according opened table now */
	void SetLeftTopItemData(void);
	int GetActiveRecordIdBySeqNo(int seqNo);

	/** Check out if @cellRanged is valid */
	BOOL IsCellRangeValid(const CCellRange& cellRange);
	BOOL IsFocusedCellInSelectedRows(
		const CCellID& focusedCell, const CCellRange& cellRange);

	CString ConvertOleDateTimeToDateStr(const COleDateTime& datetime);

	/* Return a real model, not model's reference. */
	CStockModelBuy	 ConvertDlgDataToBuyModel(const CStockBuyDlg& buyDlg);
	CStockModelHold  ConvertBuyModelToHoldModel(const CStockModelBuy&);
	CStockModelSell  ConvertToSellModel(const CStockModelHold&holdModel, 
										const CStockSellDlg& sellDlg);
	CStockModelMoney ConvertDlgDataToMoneyModel();

public:
	/** Set gird with data queried from sqlite3_get_table(). */
	int GetAndShowTableData(ENUMACTIVETABLE whichTable);
	int SetGridData(int nRow, int nCol, char** result);

	/** Reload table records according to m_enumRecordTable */
	int ReloadRecords(void);

	/** Set up database and tables' names, must be called at startup. */
	int SetupDBTableNames(void);

private:
	
	CGridCtrl m_GridCtrl;
	CStockDBConnection m_dbConn;
	CTrayIcon* m_pTrayIcon;

	bool m_bIsWndHidden;
	bool m_bIsPlanSell;

	/**
	 *	Store table's ids, the seqNo is the index of vector.
	 */
	vector<int> m_vecStockBuyIds;
	vector<int> m_vecStockHoldIds;
	vector<int> m_vecStockSellIds;
	vector<int> m_vecStockMoneyIds;

private:
	/** Clear related vector according by m_enumRecordTable to do a new display. */
	void ClearRecordIds(void);

	/**
	 *	Store record id. The 'seqNo' is sequence number, which can be 
	 *  determined by the index of vector. This will cause gird can not order.
	 *  This method may not be good, but it can work in this condition.
	 */
	void StoreRecordId(int id);
	
public:
	afx_msg void OnMenuPlanbuy();
};
