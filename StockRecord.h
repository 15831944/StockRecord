
// StockRecord.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CStockRecordApp:
// �йش����ʵ�֣������ StockRecord.cpp
//

class CStockRecordApp : public CWinApp
{
public:
	CStockRecordApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CStockRecordApp theApp;