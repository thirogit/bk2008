#include "stdafx.h"
#include "mysql_func.h"

ulong escape_string_for_FB(char *to, ulong to_length,
                               const char *from, ulong length)
 {
   const char *to_start= to;
   const char *end, *to_end=to_start + (to_length ? to_length-1 : 2*length);
   my_bool overflow = FALSE;
 
   for (end= from + length; from < end; from++)
   {
     
     if (*from == '\'')
     {
       if (to + 2 > to_end)
       {
         overflow= TRUE;
         break;
       }
       *to++= '\'';
       *to++= '\'';
     }
     else
     {
       if (to + 1 > to_end)
       {
         overflow= TRUE;
         break;
       }
       *to++= *from;
     }
   }
   *to= 0;
   return overflow ? (ulong)~0 : (ulong) (to - to_start);
 
}
