#include "stdafx.h"
#include "htoi.h"

BOOL htoi(const TCHAR *str,UINT *outValue)
{
    int value = 0,  /* the sum of the hexadecimal digits' equivalent
                       decimal values */
           weight,  /* the weight of a hexadecimal digit place */   
            digit,
            i = 0,  /* array index */
              len,  /* length of string */
                c;  /* character */            
   
    /* report error on empty string */
    if (str[i] == '\0')
        return FALSE;
        
    /* ignore 0x or 0X in hexidecimal string */
    if (str[i] == '0') {
        ++i;
        if (str[i] == 'x' || str[i] == 'X')
            ++i;
    }
    
    len = _tcslen(str);    
    
    /* calculate each hex character's decimal value */
    for ( ; i < len; ++i) {
        c = tolower(str[i]);
        if (c >= '0' && c <= '9')
            digit = c - '0';
        else if (c >= 'a' && c <= 'f') {
            switch (c) {
                case 'a':
                    digit = 10;
                    break;
                case 'b':
                    digit = 11;
                    break;
                case 'c':
                    digit = 12;
                    break;
                case 'd':
                    digit = 13;
                    break;
                case 'e':
                    digit = 14;
                    break;
                case 'f':
                    digit = 15;
                    break;
                default:
                    break;
            }
        }
        else
            return FALSE;  /* invalid input encountered */
        	
        weight = 1 << ((len - i - 1)*4);
        value += weight * digit;
    }
	if(outValue)
		*outValue = value;
    return TRUE;
}

