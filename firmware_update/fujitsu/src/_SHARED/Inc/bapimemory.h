/* Copyright (C) 2006-2006 Fujitsu Technology Solutions
   Thilo Cestonaro
   This file is part of some kernel drivers of Fujitsu Technology Solutions

   It is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   It is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with this file; see the file COPYING. If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/
typedef struct tagBiosApiCtrlOut
/************************************************************************************
	Author:		Fujitsu Technology Solutions / PT
    Date:       30.07.1999
******************************************+*****************************************/
{
   USHORT            usAllocatedLength;
   ULONG             ulServiceSignature;
   struct
   {
      ULONG   CompletionFlag   : 4;
      ULONG   Reserved         : 28;
   } ulApiAction;
   UCHAR             ucStatus;
   USHORT            usErrorCode;
   UCHAR             ucReserved1;
   UCHAR             ucReserved2;
   UCHAR             ucReserved3;
   UCHAR             ucReserved4[1008];
} __attribute__ ((packed)) BIOS_API_CTRL_OUT, *PBIOS_API_CTRL_OUT;
/**
    <<purpose>>
*/
typedef struct tagBiosApiCtrlIn
/************************************************************************************
    Author:		Fujitsu Technology Solutions / PT
    Date:       06.07.1999
    Changes:
    28.03.2002, PT: Added ulSniNv
******************************************+*****************************************/
{
    USHORT            usAllocatedLength;   // Size of buffer allocated by the application
    ULONG             ulServiceSignature;  // Signature of the BIOS service
    union
    {
	    ULONG ulSniBs;

	    struct
        {
            ULONG   UpdateAreaMask   : 16;
            ULONG   Reserved2        : 8;
            ULONG   PreserveSicrypt  : 1;    // Preserve Sicrypt Settings
            ULONG   Reserved1        : 7;
        } __attribute__ ((packed)) ulSniFu;                           // flash update
/*HR SNIBS does have service codes !!! (Signum of queried service)
      struct
      // Item is implemented for test purposes
      // Normally SNIBS does not have an action code
      {
         ULONG   SmiDelay         : 8;    // Delay of SMI in seoconds before it returns
         ULONG   Reserved         : 24;
      } ulSniBs;
 */

        ULONG ulSniOn;
        ULONG ulSniSm;
        ULONG ulSniFa;
        ULONG ulSniNv;
        ULONG ulSniAc;
        ULONG ulSni0w;                     // 0 = GetDeviceInfo; pair of even/uneven number per intervall (e.g. 1 for Get and 2 for Set of first intervall)
        ULONG ulValue;                     // General access item
    } ServiceCode;
   union
   {
       struct
       {
           USHORT       usCode;            // Function control data of the function called
           UCHAR        ucReserved1[4];    // Reserved
       } __attribute__ ((packed)) Generic;

       struct
       {
           ULONG        ulCallerId;	        // 0x064496350="PC_IDENT.EXE", 0="other App"
           UCHAR        ucReserved1[2];     // Reserved
       } __attribute__ ((packed)) SniOd;

       struct 
       {
           // naming comes from spec!!
           USHORT       us1; // Function control 1
           USHORT       us2; // Function control 2
           USHORT       us3; // Function control 3
       } __attribute__ ((packed)) SniNv;
   } FunctionControl;
   UCHAR           ucScratch[1008];     // Scratch buffer, which may be randomly used
} __attribute__ ((packed)) BIOS_API_CTRL_IN, *PBIOS_API_CTRL_IN;
