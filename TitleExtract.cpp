#include <vector>
#include <deque>
#include <fstream>

#include "TitleExtract.h"
#include "File.h"
#include "FileTinyXml.h"

using namespace std;


////////////////////////////////////////////////////////////////////////
//功能：标题提取的总调用
//参数：
//	pageurl:网站的url
//	root:网页树的根节点
//	title:返回的标题
//////////////////////////////////////////////////////////////////////////
void TitleExtract::extractTitle( string &pageUrl, TiXmlNode *const root, string &title )
{
	
	vector< pair< pair< string, string >, pair< string, string > > >::iterator iterT, iterT_end = m_vecRule.end();
	for( iterT = m_vecRule.begin(); iterT != iterT_end; ++iterT )
	{
		//查找网站url的关键词
		if( pageUrl.find( (*iterT).first.first ) != string::npos )
		{
			break;
		}
	}
	
	//对于特定网站利用规则
	if( iterT != iterT_end )
	{
		string temp = extrCertainUrl( pageUrl, root );
		
		if( !temp.empty() )				//如果从规则中提取出标题则返回，否则运用一般规则提取
		{
			title = temp;
			deleteWords( title );
			return;
		}
	}
	
	string tag = "title";
	vector< TiXmlNode * > vecNode;		//存放节点
	extractNode( root, tag, vecNode );
	int direct = 1;
	
	for( vector< TiXmlNode * >::iterator iterNode = vecNode.begin(); iterNode != vecNode.end(); ++iterNode )
	{
		if( *iterNode )
		{
			deque< string > deTitle;
			extractText( *iterNode, deTitle, direct );
			if( !deTitle.empty() )
			{	
				title = "";
				for( deque<string>::iterator iterTi = deTitle.begin(); iterTi != deTitle.end(); ++iterTi )
				{
					title.append( *iterTi );
				}
				deleteWords( title );
				break;
			}
		}
	}
	
}


void TitleExtract::extract( char *&title, char *&content)
{
	title = NULL;
	content = NULL;
	FileTidy tidyFile( m_srcFilePath );
	tidyFile.mainTidy();
	
	//使用tinyxml建立树形结构
	FileTinyXml tinyXmlFile( m_srcFilePath.c_str() );
	bool loadOk = tinyXmlFile.loadSrcFile();
	
	if( !loadOk )
		return;
	
	//提取文件的正文
	deque<string> queText;
	tinyXmlFile.extractContent( queText );
	
	string strContent;
	deque<string>::const_iterator dIter, dIter_end = queText.end();
	for( dIter = queText.begin(); dIter != dIter_end; ++dIter )
	{
		strContent += *dIter;
	}
	
	if( m_content != NULL )
		delete[] m_content;
	m_content = new char[strContent.size() + 1];
	strcpy( m_content, strContent.c_str() );
	//cout << m_content << endl;
	content = m_content;
	
	string reTitle;
	extractTitle( m_url, (TiXmlNode *)tinyXmlFile.getDocument().RootElement(), reTitle );
	
	if( m_title != NULL )
		delete[] m_title;
	
	m_title = new char[ reTitle.size() + 1 ];
	strcpy( m_title, reTitle.c_str() );
	
	title = m_title;
	
}
//新增链接提取函数
void TitleExtract::extractherf( string &pageUrl, TiXmlNode *const root, string &herf )
{
	vector< pair< pair< string, string >, pair< string, string > > >::iterator iterT, iterT_end = m_vecRule.end();	
	string tag = "a";
	vector< TiXmlNode * > veclinkNode;		//存放节点
    vector< TiXmlElement* > veclinkElement;
	extractNode( root, tag, veclinkNode );
	int direct = 1;
	
	for( vector< TiXmlNode * >::iterator iterNode = veclinkNode.begin(); iterNode != veclinkNode.end(); ++iterNode )
	{
		if( *iterNode )
		{
			TiXmlNode * const child = *iterNode;
			TiXmlElement *tet = ( TiXmlElement*)child; 
			string link;
			if(tet->Attribute("href")!=0)
			{
				link = tet->Attribute("href");
				if(link[0]=='h'||link[0]=='H' )
					herf=herf+link+'\n';
			}
			else break;
			//test=child->Value();
			
		}
	}
	herf=herf;
}
//新增链接提取函数
void TitleExtract::herfextract( char *&herf)
{
	herf=NULL;
	FileTidy tidyFile( m_srcFilePath );
	tidyFile.mainTidy();
	
	//使用tinyxml建立树形结构
	FileTinyXml tinyXmlFile( m_srcFilePath.c_str() );
	bool loadOk = tinyXmlFile.loadSrcFile();		
	if( !loadOk )
		return;
	//TiXmlDocument *myDocument = new TiXmlDocument(m_srcFilePath.c_str());
	//       myDocument->LoadFile();
	//TiXmlElement *RootElement = myDocument->RootElement();        
	//  cout << RootElement->Value() << endl;
	//herf=RootElement->Value();
	//	m_doc=
	string reherf;
	extractherf( m_url, (TiXmlNode *)tinyXmlFile.getDocument().RootElement(), reherf );
    if( m_herf != NULL )
		delete[] m_herf;
	m_herf = new char[reherf.size() + 1];
	strcpy( m_herf, reherf.c_str() );
	//cout << m_content << endl;
	herf = m_herf;
}

////////////////////////////////////////////////////////////////////////
//功能：将ruleFile与filterfile内容读入m_vecRule与m_vecWords中
//参数：
//	ruleFile:存放对于特定网站的正文提取路径
//  fliterFile:存放标题中要删除的特殊字符串
//////////////////////////////////////////////////////////////////////////
TitleExtract::TitleExtract( const char *path, const char *url, char * ruleFile, char *filterFile )
:FileTinyXml( path )
{
	//读取filter文件
	if( strlen(ruleFile) != 0 )
	{
		File inFile( filterFile );
		inFile.read( m_vecWords );
	}
	
	
	m_url = url;
	m_title = NULL; 
	m_content = NULL;
    m_herf=NULL;
	
	//读取rule文件
	if( strlen(filterFile) != 0 )
	{
		ifstream inFile( ruleFile );
		if ( !inFile ) 
			return;
		
		//存放规则中的分隔关键词
		string strTag;
		
		string textline;						//存放文件中每一行
		string url, tag, attribute, value;		//存放每一行中的各种信息内容
		//string::size_type pos, posBegin, posEnd;
		bool retOK;
		while( getline( inFile, textline ) )
		{
			
			//如果url为空则不处理						
			strTag = "<url>";
			retOK = selText( textline, strTag, url );
			if( !retOK )				//如果url没有内容则不处理
				continue;
			
			strTag = "<tag>";
			retOK = selText( textline, strTag, tag );
			if( !retOK )				//如果tag:后面没有内容则后面不再处理
			{
				tag = "";
				attribute = "";
				value = "";
			}
			else
			{
				strTag = "<attribute>";
				retOK = selText( textline, strTag, attribute );
				if( !retOK )
				{
					attribute = "";
					value = "";
				}
				else
				{
					strTag = "<value>";
					retOK = selText( textline, strTag, value );
					if( !retOK )
						value = "";
				}
			}
			m_vecRule.push_back( make_pair( make_pair< string, string >( url, tag ), make_pair< string, string >( attribute, value ) ) );
			
		}
	}
	
}

TitleExtract::~TitleExtract()
{
	
	if( m_title != NULL )
		delete[] m_title;
	
	if( m_content != NULL )
		delete[] m_content;
	
	
}

////////////////////////////////////////////////////////////////////////
//功能：从textline中提取出tag后面的内容并将textline5截短
//参数：
//	textline:要处理的字符串
//  tag:关键字
//  strRet:返回的内容
//////////////////////////////////////////////////////////////////////////
bool TitleExtract::selText( string &textline, string &tag, string &strRet )
{
	string::size_type pos, posBegin, posEnd;
	pos = textline.find( tag );
	if( pos == string::npos )					//如果没有找到则返回
		return false;
	
	posBegin = textline.find_first_not_of(  " \t", pos + tag.length() );
	if( posBegin == string::npos )				//如果标志后面没有内容则返回
		return false;
	
	posEnd = textline.find_first_of(  " \t", posBegin );		
	strRet = textline.substr( posBegin, posEnd - posBegin );		//截取字符串
	if( posEnd != string::npos )						//将textline截断
		textline = textline.substr( posEnd );
	return true;
}

////////////////////////////////////////////////////////////////////////
//功能：将标题中的特殊文字去掉。
//	其中filterwords文件中包含要去除的文字。若某一行不含<@>则删除标题中含有的此字符串
//	若含有<@>并且前后都有字符串w1与w2则在标题中删除w1与w2之间的字符串
//				若只是后面有字符串w则在标题中若含有此w则删除w两边较短的文字
//参数：
//	title:要处理的标题
//////////////////////////////////////////////////////////////////////////
void TitleExtract::deleteWords( string &title )
{
	if ( title.empty() || m_vecWords.empty() ) 
	{
		return;
	}
	
	//cout << m_vecWords.size() << endl;
	
	const string sepTag = "<@>";		//特殊分割符号
	const int maxLen = 8;
	
	vector<string>::iterator iter, iter_end = m_vecWords.end();
	string::size_type pos, posNext, posTemp;
	string strSep, strPre, strAft;
	//bool signal;
	for( iter = m_vecWords.begin(); iter != iter_end; ++iter )
	{
		strSep = strPre = strAft = "";		//置空
		pos = posNext = posTemp = 0;
		//提取出<@>前后的字符
		if( ( pos = iter->find( sepTag ) ) != string::npos )
		{
			posNext = iter->find_first_not_of( " \t" );		//找到第一个非空字符
			if( posNext == pos )		//sepTag前没有字符
			{
				posNext = iter->find_first_not_of( " \t", pos + sepTag.length() );	//找到之后的非空字符
				if( posNext == string::npos )			//如果sepTag前后没有字符则不处理
				{
					continue;
				}
				else					//sepTag前没有字符，后有字符
				{
					posTemp = iter->find_first_of( " \t", posNext );
					if( posTemp != string::npos )
						strAft = iter->substr( posNext, posTemp - posNext );	//提取出<@>后第一个非空字符与之后的第一个空字符之间内容
					else
						strAft = iter->substr( posNext );
				}
			}
			else						//sepTag前有字符
			{
				string::size_type temp = iter->find_first_of( " \t", posNext );		
				if( temp > pos )
				{
					temp = pos;
				}
				//提取前面的内容
				strPre = iter->substr( posNext, temp - posNext );
				
				
				posTemp = iter->find_first_not_of( " \t", pos + sepTag.length() );		//<@>后续第一个非空字符
				if( posTemp != string::npos )		//segTag前后都有字符
				{
					
					
					posNext = posTemp;
					posTemp = iter->find_first_of( " \t", posNext );
					if( posTemp != string::npos )
						strAft = iter->substr( posNext, posTemp - posNext );	//提取出<@>后第一个非空字符与之后的第一个空字符之间内容
					else
						strAft = iter->substr( posNext );
				}
				
			}
			if( !strAft.empty() && !strPre.empty() )	//如果前后字符都不为空则删除两个字符中间的词
			{
				pos = title.find( strPre );
				if( pos != string::npos )
				{
					posNext = title.find( strAft, pos + strPre.length() );
					if( posNext != string::npos && posNext - pos <= maxLen )
					{
						title.erase( pos, posNext + strAft.length() - pos );
					}
				}
			}
			else if( !strAft.empty() )			//如果后续字符不为空则找到此字符并删除两边较短的内容
			{
				pos = title.find( strAft );
				posNext = title.find_first_not_of( " \t" );		//第一个非空字符
				posTemp = title.find_last_not_of( " \t" );		//最后一个非空字符
				while( pos != string::npos )
				{
					if( pos - posNext < posTemp - pos - strAft.length() + 1 )	//如果分隔符后面字符多则删除前面内容
					{
						title = title.substr( pos + strAft.length() );
					}
					else
					{
						title = title.substr( posNext, pos - posNext );
					}
					
					pos = title.find( strAft );
					posNext = title.find_first_not_of( " \t" );		//第一个非空字符
					posTemp = title.find_last_not_of( " \t" );		//最后一个非空字符
				}
			}
		}
		else
		{			
			pos = title.find( *iter );
			if( pos != string::npos )
			{
				title.erase( pos, (*iter).length() );
			}
		}
	}
}


//将字符串转为小写
string TitleExtract::strToLower( string srcStr )
{
	for( int i = 0; i < srcStr.size(); ++i )
		if( srcStr[i] >= 0x41 && srcStr[i] <= 0x5a  )
			srcStr[i] += 0x20;
		return srcStr;
}

////////////////////////////////////////////////////////////////////////
//功能：对于指定的网站提取正文，m_vecRule文件存储的是：网站名、标题所在标签名、属性名、属性值
//参数：
//	pageurl:网站的url
//	root:网页树的根节点
//////////////////////////////////////////////////////////////////////////
string TitleExtract::extrCertainUrl( string &pageUrl, TiXmlNode *const root )
{
	
	//如果根节点为空或者提取规则为空则返回空字符串
	if( !root || m_vecRule.empty() )
		return "";
	
	//找到pageURl对应的规则
	vector< pair< pair< string, string >, pair< string, string > > >::iterator iter, iter_end = m_vecRule.end();
	for( iter = m_vecRule.begin(); iter != iter_end; ++iter )
	{
		//查找网站url的关键词
		if( pageUrl.find( (*iter).first.first ) != string::npos )
		{
			break;
		}
	}
	
	//对于一般网站不进行处理
	if( iter == iter_end )
		return "";
	
	//提取出文字
	string tag = strToLower( (*iter).first.second );
	string attribute = strToLower( (*iter).second.first );
	string value = strToLower( (*iter).second.second);
	
	
	//如果对于此网站的网页没有指定标题的标签则返回空字符串
	if( tag.empty() )
		return "";
	
	vector< TiXmlNode * > vecNodes;
	extractNode( root, tag, vecNodes );
	if( vecNodes.empty() )
		return "";
	
	//匹配每一个节点
	deque<string> deTitle;
	int direct = 1;
	vector< TiXmlNode * >::iterator iterNode, iterNode_end = vecNodes.end();
	for( iterNode = vecNodes.begin(); iterNode != iterNode_end; ++iterNode )
	{
		if( !(*iterNode) )
			continue;
		
		deTitle.clear();
		
		if( attribute.empty() )							//如果属性为空则提取此节点下的文字信息，如果文字为空则查询下一节点
		{
			extractText( *iterNode, deTitle, direct );
			if( !deTitle.empty() )
			{
				break;
			}			
		}
		else if( value.empty() )						//如果值为空则判断属性，如果此节点含有此属性并且含有文字信息则退出循环
		{
			if( ( (TiXmlElement *)(*iterNode) )->Attribute( attribute ) )
			{
				extractText( *iterNode, deTitle, direct );
				if( !deTitle.empty() )
				{
					break;
				}
			}
			
		}
		else										//如果值不为空，则如果此节点含有此属性并且值相等则退出循环
		{
			
			if(  ( (TiXmlElement *)(*iterNode) )->Attribute( attribute ) && ( (TiXmlElement *)(*iterNode) )->Attribute( attribute ) == value )
			{
				extractText( *iterNode, deTitle, direct );
				//	cout << "from" << endl;
				if( !deTitle.empty() )
				{
					break;
				}
			}
		}
		
	}	
	
	
	if( deTitle.empty() )
		return "";
	
	string strRe;
	for( deque<string>::iterator iterT = deTitle.begin(); iterT != deTitle.end(); ++iterT )
	{
		strRe.append( *iterT );
	}
	return strRe;
	
}

/*void TitleExtract::herfextract( char *&herf)
{
herf=NULL;
FileTidy tidyFile( m_srcFilePath );
tidyFile.mainTidy();
FileTinyXml tinyXmlFile( m_srcFilePath.c_str() );
bool loadOk = tinyXmlFile.loadSrcFile();		
if( !loadOk )
return;
//TiXmlDocument *myDocument = tinyXmlFile.getDocument().RootElement();	
//TiXmlDocument *myDocument = new TiXmlDocument(m_srcFilePath.c_str());
// myDocument->LoadFile();
//TiXmlElement *RootElement = myDocument->RootElement();

  //  cout << RootElement->Value() << endl;
  string reherf;
  //herf=RootElement->Value();
  
	const TiXmlElement* root = tinyXmlFile.getDocument().RootElement();
	
	  string test;
	  //const TiXmlNode* childt = root->FirstChild();
	  //const TiXmlNode* childtd1 = childt->NextSibling();
	  //const TiXmlNode* childtd = childtd1->NextSibling();
	  //const TiXmlElement *tet = (const TiXmlElement*)childtd; 
	  
		//test = childtd->Value(); 
		
		  for( const TiXmlNode* child = root->FirstChild();
		  child;
		  child=child->NextSibling())
		  { 
		  const TiXmlNode* child1=child;	
		  string te;
		  te=child1->FirstChild()->Value();
		  test=test+te;
		  
			if((child->Type() == TiXmlNode::ELEMENT) && (!strcmp(child->Value(),"a")))
			{ 
			const TiXmlElement *box = (const TiXmlElement*)child;   
			string link;
			
			  string link;
			  link = box->Attribute("href"); 
			  //test=child->Value();
			  reherf=reherf+link;
			  
				for(const TiXmlNode *sub_tag = child->FirstChild(); sub_tag; sub_tag = sub_tag->NextSibling() )
				{
				
				  if(sub_tag->Type() == TiXmlNode::ELEMENT)
				  {
				  const TiXmlElement *sub_element = (const TiXmlElement*)sub_tag; 
				  
					if(!strcmp(sub_tag->Value(),"a"))
					{
					link = (sub_element->Attribute("href"));
					reherf=reherf+link;
					}    
					}
					}   
					}
					} 
					if( m_herf != NULL )
					delete[] m_herf;
					m_herf = new char[test.size() + 1];
					strcpy( m_herf, test.c_str() );
					//cout << m_content << endl;
					herf = m_herf;
					}
*/