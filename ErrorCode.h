/**
 *	Define error code of stock record app.
 */

#ifndef STOCK_RECORD_ERROR_CODE_H
#define STOCK_RECORD_ERROR_CODE_H

#define OK					0
/** General error, my be specified in details in future. */
#define ERR					-1

#define BAD_DB_TABLE_NAME				0x10

/** Database status related */
#define DB_STATUS_OPENED				0x20
#define DB_STATUS_CLOSED				0x21
#define DB_STATUS_DOESNOT_EXIST			0x22
#define DB_STATUS_NOT_INITED			0x23

#endif