/**
 *	Used for calculate stock's hold_cost and even_price.
 */

#ifndef STOCK_RECORD_CALCULATO_H
#define STOCK_RECORD_CALCULATO_H

#define ROUND_4S5R	0	// 控制round函数4舍5入
#define ROUND_ZSBR	1	// 控制round函数只舍不入
#define ROUND_ZRBS	2	// 控制round函数只入不舍

float Round(float fValue, int precision, int roundway = ROUND_ZRBS);

class CStockFees
{
public:
	CStockFees(void);
	~CStockFees(void);

public:
	float m_fCommisionRate;
	float m_fStampTaxRate;
	float m_fTransferRate;

public:
	float m_fBuyStockMoney;
	float m_fBuyCommiMoney;
	float m_fBuyTransferMoney;

public:
	float m_fHoldCost;
	float m_fEvenPrice;

public:
	void StockHoldCalculate(bool bStockType, float fBuyPrice, int nBuyAmount);
};



#endif