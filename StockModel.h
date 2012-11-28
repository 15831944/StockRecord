/**
 *	Define model class to hold values queried from or written to database.
 *  The model can be defined as class or struct.
 *  Also, provide some functions to manipulate the database.
 *
 *  NOTE: The model's members should store values converted into GB2312 from UTF8.
 */

#ifndef STOCK_RECORD_MODEL_H
#define STOCK_RECORD_MODEL_H

/**
 * Base model 
 */
class CRecordModelBase
{
public:
	CRecordModelBase(void);
	~CRecordModelBase(void);

public:
	/**
	 * id will be inited to -1, to indicate there is no record in db. 
	 * If id is not -1, the model represents a record which already exists in db.
	 * In this case, id is the value of actual record id in database.
	 */
	int id;	
	CString code;
	CString name;

private:
	enum EncodeStyle m_encodeStyle;

public:
	void SetEncodeStyle(int encodeStyle);
	int  GetEncodeStyle(void) const;
	virtual int ConvertEncodeFormat(int targetEncode);

};

/**
 *	Stock buy record model.
 */
class CStockModelBuy : public CRecordModelBase
{
public:
 	CStockModelBuy(void);
	~CStockModelBuy(void);

public:
	CString buy_price;
	CString buy_amount;
	CString buy_date;
	bool stock_type;
};

/**
 *	Stock hold record model.
 */
class CStockModelHold : public CRecordModelBase
{
public:
	CStockModelHold(void);
	~CStockModelHold(void);

public:	
	CString buy_price;
	CString hold_cost;
	CString hold_amount;
	CString even_price;
	CString buy_date;
};

/**
 *	Stock sell record model.
 */
class CStockModelSell : public CRecordModelBase
{
public:
	CStockModelSell(void);
	~CStockModelSell(void);

public:
	CString buy_price;
	CString sell_price;
	CString sell_amount;
	CString even_price;
	CString buy_date;
	CString sell_date;
	CString each_earn;
	CString total_earn;
};

/**
 *	Stock money record model.
 */
class CStockModelMoney
{

};

/**
 *	Future taken / hold / clsed record model.
 */
class CFutureTakenModel : public CRecordModelBase
{
public:
	CFutureTakenModel(void);
	~CFutureTakenModel(void);
public:
	CString taken_price;
	CString taken_amount;
	CString multiplier;
	CString margin_ratio;
	CString commision_ratio;
	CString taken_date;
};

class CFutureHoldModel : public CRecordModelBase
{
public:
	CFutureHoldModel(void);
	~CFutureHoldModel(void);
public:
	CString taken_price;
	CString hold_amount;
	CString hold_cost;
	CString even_price;
};

class CFutureClosedModel : public CRecordModelBase
{
public:
	CFutureClosedModel(void);
	~CFutureClosedModel(void);

};

#endif