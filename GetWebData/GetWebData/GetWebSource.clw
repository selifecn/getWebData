; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CGetWebSourceDlg
LastTemplate=CView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "GetWebSource.h"

ClassCount=4
Class1=CGetWebSourceApp
Class2=CGetWebSourceDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=Cwebdata
Resource3=IDD_GETWEBSOURCE_DIALOG

[CLS:CGetWebSourceApp]
Type=0
HeaderFile=GetWebSource.h
ImplementationFile=GetWebSource.cpp
Filter=N

[CLS:CGetWebSourceDlg]
Type=0
HeaderFile=GetWebSourceDlg.h
ImplementationFile=GetWebSourceDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CGetWebSourceDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=GetWebSourceDlg.h
ImplementationFile=GetWebSourceDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_GETWEBSOURCE_DIALOG]
Type=1
Class=CGetWebSourceDlg
ControlCount=36
Control1=IDC_EDADDRESS,edit,1350631552
Control2=IDC_EDWEBSOURCE,edit,1353781444
Control3=IDC_STATIC,static,1342308352
Control4=IDC_BTGET2,button,1342242816
Control5=IDC_BTGET3,button,1342242816
Control6=IDC_COOKIES,button,1476460544
Control7=IDC_USER,button,1476460544
Control8=IDC_JSWEB,button,1476460544
Control9=IDC_NAME,edit,1350631552
Control10=IDC_PASS,edit,1350631584
Control11=IDC_LIST3,SysListView32,1350631425
Control12=IDC_delinf,button,1476460544
Control13=IDC_ID,edit,1350631552
Control14=IDC_BUTTON1,button,1342242816
Control15=IDC_EDI_SQL,edit,1350631552
Control16=IDC_SQL,button,1476460544
Control17=IDC_STATIC,button,1342177287
Control18=IDC_STATIC,button,1342177287
Control19=IDC_BUTUPdata2,button,1342242816
Control20=IDC_inf,button,1342242816
Control21=IDC_STATIC,button,1342177287
Control22=IDC_STATIC,button,1342177287
Control23=IDC_BUT_NEW,button,1342242816
Control24=IDC_cook,button,1342242816
Control25=IDC_buser,button,1476460544
Control26=IDC_bjsweb,button,1476460544
Control27=IDC_log,button,1476460544
Control28=IDC_EDIT1,edit,1350631552
Control29=IDC_dell,button,1476460544
Control30=IDC_STATIC,button,1342177287
Control31=IDC_STATIC,button,1342177287
Control32=IDC_BUT_NEW2,button,1342242816
Control33=IDC_getshellcode,button,1476460544
Control34=IDC_getshellcode2,button,1476460544
Control35=IDC_readweb,button,1476460544
Control36=IDC_getshellcodedll,button,1342242816

[CLS:Cwebdata]
Type=0
HeaderFile=webdata.h
ImplementationFile=webdata.cpp
BaseClass=CView
Filter=C

