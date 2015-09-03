/**
* LICENSE PLACEHOLDER
*
* @file debug.h
* @package OpenPST Extras - QFPROM Kernel Module
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#ifndef _QFPROM_TCP_DEBUG_H
#define _QFPROM_TCP_DEBUG_H

#define log(fmt, args...)	printk("tz_qfprom : %s "fmt, __FUNCTION__, ##args)

#endif // _QFPROM_TCP_DEBUG_H