/* Copyright (C) 2006-2006 Fujitsu Technology Solutions
   Thilo Cestonaro
   This file is part of sobcmos

   sobcmos is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   sobcmos is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with sobcmos; see the file COPYING. If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/
/************************************************************************************
                                  S Y S T E M . H
	 
    Project:    DeskFlash for Linux
    Created:    Fujitsu Technology Solutions / PT                            
    Purpose:    
    Version:    1.30
    Implements: 
	Changes:    05.10.2001, FS: Layout
******************************************+*****************************************/
#ifndef __SYSTEM_H__
#define __SYSTEM_H__

/*===================================================================================
                         C O N S T A N T  I D E N T I F I E R S
==========================================+========================================*/
#define  CMOS_INDEX   0x70
#define  CMOS_DATA    0x71
#define  NMIOFF       0x80  // disables NMIs if OR'ed with standard CMOS index

/*----------------------------------------+------------------------------------------
                         c o n s t a n t  i d e n t i f i e r s
-----------------------------------------------------------------------------------*/

/*===================================================================================
                                      M A C R O S
==========================================+========================================*/

/*----------------------------------------+------------------------------------------
                                      m a c r o s
-----------------------------------------------------------------------------------*/

/*===================================================================================
                             D A T A  S T R U C T U R E S
==========================================+========================================*/

/*----------------------------------------+------------------------------------------
                             d a t a  s t r u c t u r e s
-----------------------------------------------------------------------------------*/

/*===================================================================================
                    F U N C T I O N  P R O T O T Y P E S
==========================================+========================================*/
USHORT SysCmosWrite(UCHAR *ucBuffer, ULONG ulOffset, ULONG ulCount);
USHORT SysCmosRead(UCHAR *ucBuffer, ULONG ulOffset, ULONG ulCount);

/*----------------------------------------+------------------------------------------
                    f u n c t i o n  p r o t o t y p e s
-----------------------------------------------------------------------------------*/
#endif	// __SYSTEM_H__

