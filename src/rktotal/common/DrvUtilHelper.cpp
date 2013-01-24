#include "stdafx.h"
#include "DrvUtilHelper.h"

ULONG DrvUtilHelper::GetSystemVersion()
{
   ULONG ulMajor = 0;
   ULONG ulMinor = 0;
   BOOL bRet = PsGetVersion(&ulMajor, &ulMinor, NULL, NULL);
   ULONG ulVersion = enumInvalidSystem;

   if (bRet)
   {
       return enumInvalidSystem;
   }

   switch(ulMajor)
   {
   case  5:
       {
           switch(ulMinor)
           {
           case 1:
               ulVersion = enumXpSystem;
               break;
           case 2:
               ulVersion = enum2003System;
               break;
           default:
               break;
           }
       }
       break;
   case 6:
       {
           switch(ulMinor)
           {
           case 1:
               ulVersion = enumVistaSystem;
               break;
           case 2:
               ulVersion = enumWin7System;
               break;
           }
       }
       break;
   case 7:
       {
           switch(ulMinor)
           {
           case 1:
               ulVersion = enumWin8System;
               break;
           default:
               break;
           }
       }
       break;
   default:
       break;
   }

   return ulVersion;
}