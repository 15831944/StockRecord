#include "StdAfx.h"
#include "StockCalculate.h"
#include <math.h>

CStockFees::CStockFees(void)
	: m_fCommisionRate(0.0015f)
	, m_fStampTaxRate(0.001f)
	, m_fTransferRate(0.001f)
	, m_fBuyStockMoney(0)
	, m_fBuyCommiMoney(0)
	, m_fBuyTransferMoney(0)
	, m_fHoldCost(0)
	, m_fEvenPrice(0)
{
}


CStockFees::~CStockFees(void)
{
}

void 
CStockFees::StockHoldCalculate( bool bStockType, float fBuyPrice, int nBuyAmount )
{
	m_fBuyStockMoney = Round(fBuyPrice * nBuyAmount, 2);
	m_fBuyCommiMoney = Round(fBuyPrice * nBuyAmount * m_fCommisionRate, 2);
	if (m_fBuyCommiMoney < 5.0f)
		m_fBuyCommiMoney = 5.0f;
	if (bStockType == STOCK_TYPE_SHANG_HAI) {
		m_fBuyTransferMoney = Round((float)nBuyAmount * m_fTransferRate, 2);
		if (m_fBuyTransferMoney < 1.0f)
			m_fBuyTransferMoney = 1.0f;
	} else {
		m_fBuyTransferMoney = 0.0f;
	}

	/* hold cost */
	m_fHoldCost = Round((m_fBuyStockMoney + m_fBuyCommiMoney +	\
		m_fBuyTransferMoney) / nBuyAmount, 3);

	/* even price */
	m_fEvenPrice = (m_fBuyCommiMoney + m_fBuyTransferMoney * 2 + m_fBuyStockMoney) \
		/ (1.0f - m_fStampTaxRate - m_fCommisionRate) / nBuyAmount;
	float fEvenSellCommiMoney = Round(m_fEvenPrice * nBuyAmount * m_fCommisionRate, 2);
	if (fEvenSellCommiMoney < 5.0f) {
		m_fEvenPrice = (m_fBuyCommiMoney + m_fBuyTransferMoney * 2 + m_fBuyStockMoney \
			+ 5.0f) / (1.0f - m_fStampTaxRate) / nBuyAmount;
	}

	m_fEvenPrice = Round(m_fEvenPrice, 3);
}

float Round( float fValue, int precision, int roundway /*= ROUND_ZRBS*/ )
{
	switch (roundway) {
	case ROUND_ZRBS:
		return long(fValue * pow(10.0f, precision) + 0.9) / float(pow(10.0f, precision));
		break;

	case ROUND_ZSBR:
		return long(fValue * pow(10.0f, precision)) / float(pow(10.0f, precision));
		break;

	case ROUND_4S5R:
		return long(fValue * pow(10.0f, precision) + 0.5) / float(pow(10.0f, precision));
		break;

	default:
		return long(fValue * pow(10.0f, precision) + 0.5) / float(pow(10.0f, precision));
		break;
	}

	return 0.0f;	// never run here
}