/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sat Mar 12 16:12:21 2005
 */
/* Compiler settings for C:\stefano\progetti\Algostar\AsCfgBuilder.Unicode\src\templates\ProjectConfigAtl\ProjectConfigAtl.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID LIBID_PROJECTCONFIGATLLib = {0x8DD3D207,0xB8CF,0x40C2,{0x98,0xA7,0x6D,0x6F,0xB4,0x0D,0xA3,0x37}};


#ifdef __cplusplus
}
#endif

