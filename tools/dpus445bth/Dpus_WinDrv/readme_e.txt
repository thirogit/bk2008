Seiko Instruments Inc.
Thermal Printers

DPU-S Series Printer Driver


 
[[[ INTRODUCTION ]]]
Seiko Instruments Inc. provides the printer drivers 
for DPU-S series printer.
Those drivers work on following Microsoft Windows version. 



[[[ SYSTEM REQUIREMENTS ]]]
Supported host computer
#  PC-AT compatible

Supported operating system
#  Microsoft Windows XP
#  Microsoft Windows Vista
#  Microsoft Windows 7 

Supported interface
#  Serial Communication (RS-232C)
#  USB Communication 
* When a communication method other than the above is used,
  the function using the communication library and 
  the bidirectional communication are out of supports.



[[[ TARGET DEVICE ]]]
#  DPU-S245-00A-E
#  DPU-S245-01A-E
#  DPU-S445-00A-E
#  DPU-S445-01A-E

 
[[[ FILE LIST ]]]
#  Operations manual (Manual_e.pdf in manuals folder) 
#  Printer driver Install Program (win_xp_7 folder)
     * Standard print (InstDrv.exe in Standard folder)
     * Reduction print (InstDrv.exe in Reduce folder)(For only DPU-S445)
   Cannot use Standard and Reduction at the same time.
#  Sample program for communication library (sdk folder)
#  This file (Readme_e.txt)



[[[ Note ]]]
#  Operations manual is portable document format(PDF). If you read the document, then you need Adobe Reader.
   Adobe Reader is available for download at download page of Adobe Systems Incorporated.   


   
[[[ VERSION ]]] 
Ver1.01 (Apr 2008) 
# First public release. 
Ver1.02 (May 2008) 
# Modified the trouble of the print position.
# Improved the uninstaller.
# Improved the exception handling in the function setting.
# Changed the waiting time of the printing to longer.
# Changed the choice of the halftone feature.
Ver1.03 (Sep 2008)
# Modified the control of time-out in the SetSiiPrinterData function of the communication library.
# Improved the some process in the DLL function.
# Improved the exception control for serial port synchronizing.
Ver1.05 (Feb 2009)
# Modified the feature of the print mode switched by [Print Mode] option.
# Improved the ASB response during the printing.
# Expanded the input range of the custom paper size.
# Added DPU-S245 model.
# Improved the problem caused by disconnect of USB cable.
Ver1.05a (Mar 2009)
#  Operations manual (Rev3 -> Rev4) 
Ver1.06 (Apr 2009)
# Improved the problem that more  null data are received by calling 
   GetSiiPrinterData() function in GET_RAW_DATA_CMD_RESP parameter after the reset of printer on USB port.
# Improved the problem that the print that exceeds time-out may break print image on USB port.
Ver1.10 (Sep 2009)
# Added communication DLL name as SII_DPUS_API.DLL newly.(A legacy file name [SII_DPUS445_API.DLL] can be used.)
# Changed the INF file name to "SII_DPUS.INF".
#  Modified Operations manual(Rev5).
Ver1.10a (Oct 2009) 
#  Modified Operations manual (Rev6).
Ver1.20 (Sep 2010)
# Supports Windows 7 (x86).
# Modified the method of installation.
# Modified Operations manual (Rev7).
Ver2.20 (May 2011) 
#  Modified driver's installer.
#  Modified the modules of ui,utility and communication library to a common architecture. 
#  Expanded the functions of communication library.
#  Modified Operations Manual(Rev8).
Ver2.30 (Aug 2011) 
#  Supports Windows 7 (x64).
#  Modified Operations Manual(Rev9).
Ver2.40 (Dec 2011) 
#  Modified the memory problem of usb communication.
#  Modified the minimum timeout value of SetSiiPrinterData().  



[[[ CONTACT US ]]]
Seiko Instruments Inc.
Web: http://www.sii.co.jp/sps/eg/


Copyright (c) 2008-2011 by Seiko Instruments Inc. All rights reserved.

The information contained herein shall not be reproduced in whole or in part without the prior
written approval of SII.
SII reserves the right to make changes without notice the specifications and materials
contained herein.
SII is not liable for any damages, losses, caused by or relating to the use of this product, nor
for any expenses incurred for defraying such.
SII is a trademark of Seiko Instruments Inc.
Microsoft Windows are registered trademarks of Microsoft Corporation in the United States
and/or her countries.
Other application software mentioned herein may be trademarks and/or registered
trademarks of their respective companies.