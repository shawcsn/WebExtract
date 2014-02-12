#include <vector>
#include <deque>
#include <fstream>

#include "TitleExtract.h"
#include "File.h"
#include "FileTinyXml.h"

using namespace std;


////////////////////////////////////////////////////////////////////////
//���ܣ�������ȡ���ܵ���
//������
//	pageurl:��վ��url
//	root:��ҳ���ĸ��ڵ�
//	title:���صı���
//////////////////////////////////////////////////////////////////////////
void TitleExtract::extractTitle( string &pageUrl, TiXmlNode *const root, string &title )
{
	
	vector< pair< pair< string, string >, pair< string, string > > >::iterator iterT, iterT_end = m_vecRule.end();
	for( iterT = m_vecRule.begin(); iterT != iterT_end; ++iterT )
	{
		//������վurl�Ĺؼ���
		if( pageUrl.find( (*iterT).first.first ) != string::npos )
		{
			break;
		}
	}
	
	//�����ض���վ���ù���
	if( iterT != iterT_end )
	{
		string temp = extrCertainUrl( pageUrl, root );
		
		if( !temp.empty() )				//����ӹ�������ȡ�������򷵻أ���������һ�������ȡ
		{
			title = temp;
			deleteWords( title );
			return;
		}
	}
	
	string tag = "title";
	vector< TiXmlNode * > vecNode;		//��Žڵ�
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
	
	//ʹ��tinyxml�������νṹ
	FileTinyXml tinyXmlFile( m_srcFilePath.c_str() );
	bool loadOk = tinyXmlFile.loadSrcFile();
	
	if( !loadOk )
		return;
	
	//��ȡ�ļ�������
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
//����������ȡ����
void TitleExtract::extractherf( string &pageUrl, TiXmlNode *const root, string &herf )
{
	vector< pair< pair< string, string >, pair< string, string > > >::iterator iterT, iterT_end = m_vecRule.end();	
	string tag = "a";
	vector< TiXmlNode * > veclinkNode;		//��Žڵ�
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
//����������ȡ����
void TitleExtract::herfextract( char *&herf)
{
	herf=NULL;
	FileTidy tidyFile( m_srcFilePath );
	tidyFile.mainTidy();
	
	//ʹ��tinyxml�������νṹ
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
//���ܣ���ruleFile��filterfile���ݶ���m_vecRule��m_vecWords��
//������
//	ruleFile:��Ŷ����ض���վ��������ȡ·��
//  fliterFile:��ű�����Ҫɾ���������ַ���
//////////////////////////////////////////////////////////////////////////
TitleExtract::TitleExtract( const char *path, const char *url, char * ruleFile, char *filterFile )
:FileTinyXml( path )
{
	//��ȡfilter�ļ�
	if( strlen(ruleFile) != 0 )
	{
		File inFile( filterFile );
		inFile.read( m_vecWords );
	}
	
	
	m_url = url;
	m_title = NULL; 
	m_content = NULL;
    m_herf=NULL;
	
	//��ȡrule�ļ�
	if( strlen(filterFile) != 0 )
	{
		ifstream inFile( ruleFile );
		if ( !inFile ) 
			return;
		
		//��Ź����еķָ��ؼ���
		string strTag;
		
		string textline;						//����ļ���ÿһ��
		string url, tag, attribute, value;		//���ÿһ���еĸ�����Ϣ����
		//string::size_type pos, posBegin, posEnd;
		bool retOK;
		while( getline( inFile, textline ) )
		{
			
			//���urlΪ���򲻴���						
			strTag = "<url>";
			retOK = selText( textline, strTag, url );
			if( !retOK )				//���urlû�������򲻴���
				continue;
			
			strTag = "<tag>";
			retOK = selText( textline, strTag, tag );
			if( !retOK )				//���tag:����û����������治�ٴ���
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
//���ܣ���textline����ȡ��tag��������ݲ���textline5�ض�
//������
//	textline:Ҫ������ַ���
//  tag:�ؼ���
//  strRet:���ص�����
//////////////////////////////////////////////////////////////////////////
bool TitleExtract::selText( string &textline, string &tag, string &strRet )
{
	string::size_type pos, posBegin, posEnd;
	pos = textline.find( tag );
	if( pos == string::npos )					//���û���ҵ��򷵻�
		return false;
	
	posBegin = textline.find_first_not_of(  " \t", pos + tag.length() );
	if( posBegin == string::npos )				//�����־����û�������򷵻�
		return false;
	
	posEnd = textline.find_first_of(  " \t", posBegin );		
	strRet = textline.substr( posBegin, posEnd - posBegin );		//��ȡ�ַ���
	if( posEnd != string::npos )						//��textline�ض�
		textline = textline.substr( posEnd );
	return true;
}

////////////////////////////////////////////////////////////////////////
//���ܣ��������е���������ȥ����
//	����filterwords�ļ��а���Ҫȥ�������֡���ĳһ�в���<@>��ɾ�������к��еĴ��ַ���
//	������<@>����ǰ�����ַ���w1��w2���ڱ�����ɾ��w1��w2֮����ַ���
//				��ֻ�Ǻ������ַ���w���ڱ����������д�w��ɾ��w���߽϶̵�����
//������
//	title:Ҫ����ı���
//////////////////////////////////////////////////////////////////////////
void TitleExtract::deleteWords( string &title )
{
	if ( title.empty() || m_vecWords.empty() ) 
	{
		return;
	}
	
	//cout << m_vecWords.size() << endl;
	
	const string sepTag = "<@>";		//����ָ����
	const int maxLen = 8;
	
	vector<string>::iterator iter, iter_end = m_vecWords.end();
	string::size_type pos, posNext, posTemp;
	string strSep, strPre, strAft;
	//bool signal;
	for( iter = m_vecWords.begin(); iter != iter_end; ++iter )
	{
		strSep = strPre = strAft = "";		//�ÿ�
		pos = posNext = posTemp = 0;
		//��ȡ��<@>ǰ����ַ�
		if( ( pos = iter->find( sepTag ) ) != string::npos )
		{
			posNext = iter->find_first_not_of( " \t" );		//�ҵ���һ���ǿ��ַ�
			if( posNext == pos )		//sepTagǰû���ַ�
			{
				posNext = iter->find_first_not_of( " \t", pos + sepTag.length() );	//�ҵ�֮��ķǿ��ַ�
				if( posNext == string::npos )			//���sepTagǰ��û���ַ��򲻴���
				{
					continue;
				}
				else					//sepTagǰû���ַ��������ַ�
				{
					posTemp = iter->find_first_of( " \t", posNext );
					if( posTemp != string::npos )
						strAft = iter->substr( posNext, posTemp - posNext );	//��ȡ��<@>���һ���ǿ��ַ���֮��ĵ�һ�����ַ�֮������
					else
						strAft = iter->substr( posNext );
				}
			}
			else						//sepTagǰ���ַ�
			{
				string::size_type temp = iter->find_first_of( " \t", posNext );		
				if( temp > pos )
				{
					temp = pos;
				}
				//��ȡǰ�������
				strPre = iter->substr( posNext, temp - posNext );
				
				
				posTemp = iter->find_first_not_of( " \t", pos + sepTag.length() );		//<@>������һ���ǿ��ַ�
				if( posTemp != string::npos )		//segTagǰ�����ַ�
				{
					
					
					posNext = posTemp;
					posTemp = iter->find_first_of( " \t", posNext );
					if( posTemp != string::npos )
						strAft = iter->substr( posNext, posTemp - posNext );	//��ȡ��<@>���һ���ǿ��ַ���֮��ĵ�һ�����ַ�֮������
					else
						strAft = iter->substr( posNext );
				}
				
			}
			if( !strAft.empty() && !strPre.empty() )	//���ǰ���ַ�����Ϊ����ɾ�������ַ��м�Ĵ�
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
			else if( !strAft.empty() )			//��������ַ���Ϊ�����ҵ����ַ���ɾ�����߽϶̵�����
			{
				pos = title.find( strAft );
				posNext = title.find_first_not_of( " \t" );		//��һ���ǿ��ַ�
				posTemp = title.find_last_not_of( " \t" );		//���һ���ǿ��ַ�
				while( pos != string::npos )
				{
					if( pos - posNext < posTemp - pos - strAft.length() + 1 )	//����ָ��������ַ�����ɾ��ǰ������
					{
						title = title.substr( pos + strAft.length() );
					}
					else
					{
						title = title.substr( posNext, pos - posNext );
					}
					
					pos = title.find( strAft );
					posNext = title.find_first_not_of( " \t" );		//��һ���ǿ��ַ�
					posTemp = title.find_last_not_of( " \t" );		//���һ���ǿ��ַ�
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


//���ַ���תΪСд
string TitleExtract::strToLower( string srcStr )
{
	for( int i = 0; i < srcStr.size(); ++i )
		if( srcStr[i] >= 0x41 && srcStr[i] <= 0x5a  )
			srcStr[i] += 0x20;
		return srcStr;
}

////////////////////////////////////////////////////////////////////////
//���ܣ�����ָ������վ��ȡ���ģ�m_vecRule�ļ��洢���ǣ���վ�����������ڱ�ǩ����������������ֵ
//������
//	pageurl:��վ��url
//	root:��ҳ���ĸ��ڵ�
//////////////////////////////////////////////////////////////////////////
string TitleExtract::extrCertainUrl( string &pageUrl, TiXmlNode *const root )
{
	
	//������ڵ�Ϊ�ջ�����ȡ����Ϊ���򷵻ؿ��ַ���
	if( !root || m_vecRule.empty() )
		return "";
	
	//�ҵ�pageURl��Ӧ�Ĺ���
	vector< pair< pair< string, string >, pair< string, string > > >::iterator iter, iter_end = m_vecRule.end();
	for( iter = m_vecRule.begin(); iter != iter_end; ++iter )
	{
		//������վurl�Ĺؼ���
		if( pageUrl.find( (*iter).first.first ) != string::npos )
		{
			break;
		}
	}
	
	//����һ����վ�����д���
	if( iter == iter_end )
		return "";
	
	//��ȡ������
	string tag = strToLower( (*iter).first.second );
	string attribute = strToLower( (*iter).second.first );
	string value = strToLower( (*iter).second.second);
	
	
	//������ڴ���վ����ҳû��ָ������ı�ǩ�򷵻ؿ��ַ���
	if( tag.empty() )
		return "";
	
	vector< TiXmlNode * > vecNodes;
	extractNode( root, tag, vecNodes );
	if( vecNodes.empty() )
		return "";
	
	//ƥ��ÿһ���ڵ�
	deque<string> deTitle;
	int direct = 1;
	vector< TiXmlNode * >::iterator iterNode, iterNode_end = vecNodes.end();
	for( iterNode = vecNodes.begin(); iterNode != iterNode_end; ++iterNode )
	{
		if( !(*iterNode) )
			continue;
		
		deTitle.clear();
		
		if( attribute.empty() )							//�������Ϊ������ȡ�˽ڵ��µ�������Ϣ���������Ϊ�����ѯ��һ�ڵ�
		{
			extractText( *iterNode, deTitle, direct );
			if( !deTitle.empty() )
			{
				break;
			}			
		}
		else if( value.empty() )						//���ֵΪ�����ж����ԣ�����˽ڵ㺬�д����Բ��Һ���������Ϣ���˳�ѭ��
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
		else										//���ֵ��Ϊ�գ�������˽ڵ㺬�д����Բ���ֵ������˳�ѭ��
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