
// StockRecordDlg.h : ͷ�ļ�
//

#pragma once

#include "GridCtrl_src/GridCtrl.h"
#include "e:\vc++\stockapps\stockrecord\gridctrl_src\gridctrl.h"

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
	DECLARE_MESSAGE_MAP()

private:
	CGridCtrl m_GridCtrl;
};
