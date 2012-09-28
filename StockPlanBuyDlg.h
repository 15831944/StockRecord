#pragma once


// CStockPlanBuyDlg dialog

class CStockPlanBuyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStockPlanBuyDlg)

public:
	CStockPlanBuyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStockPlanBuyDlg();

// Dialog Data
	enum { IDD = IDD_PLANBUY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	DECLARE_MESSAGE_MAP()

private:
	CString m_sPlanBuyCode;
	float m_fPlanBuyPrice;
	int m_nPlanBuyAmount;
	int m_nPlanBuyHoldAmount;
	float m_fPlanBuyHoldCost;
	float m_fPlanBuyEvenPrice;

	bool m_bStockType;

	/* StockRecordDlg will pass its m_dbConn's address to this m_pDBCoon.
	 * Used for querying holdModel when plans to buy stock.
	 */
	void* m_pDBConn;

public:
	void SetDBConnection(void* pDBConn)		{m_pDBConn = pDBConn;}
	
private:
	void HandlePlanBuy(void);

};
