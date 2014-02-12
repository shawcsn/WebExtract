#include <string>
#include <iostream>
#include <vector>
#include "FileTinyXml.h"

using namespace std;


bool FileTinyXml::loadSrcFile()
{
	if ( m_srcFilePath.empty() ) 
		return false;	
	m_doc = TiXmlDocument( m_srcFilePath );
	bool loadOK = m_doc.LoadFile();	
	return loadOK;
}


bool FileTinyXml::saveToFile( string& desFilePath )
{
	return m_doc.SaveFile( desFilePath );
}

////////////////////////////////////////////////////////////////////////
//���ܣ�ɾ��ָ����ǩ�ڵ��Լ�ֻ����<a>�ӽڵ�Ľڵ�
//������
//	subTree�����ҷ�Χ��ֻ����subTree�µ�����
//	vDelTag��Ҫɾ����ǩ��
//////////////////////////////////////////////////////////////////////////
bool FileTinyXml::deleteNode( TiXmlNode* const subTree, set<string> &sDelTag )
{
	if( !subTree )
		return true;
	//TiXmlNode *parent = subTree;		//����subTree��ֹ�޸�
	//if find the tag in sDelTag then return true
	if ( ( ( sDelTag.find( subTree->ValueStr() ) != sDelTag.end() )  && subTree->FirstChild() && subTree->FirstChild()->ValueStr() != "table" ) ||
		( subTree->FirstChild() && !( subTree->FirstChild()->NextSibling() ) && subTree->FirstChild()->ValueStr() == "a" )	//the subtree contain only a tag
		) 
	{
		return true;		
	}
	else
	{
		for( TiXmlNode *child = subTree->FirstChild(); child; )
		{
			//if find the certain node(return value is true) then remove the node
			if ( deleteNode( child, sDelTag ) )
			{
				TiXmlNode *tempChild = child;
				child = child->NextSibling();
				subTree->RemoveChild( tempChild );
			}
			else
			{
				child = child->NextSibling();
			}
			
		}
		return false;
	}
}
////////////////////////////////////////////////////////////////////////
//���ܣ�����subTree������ֵΪstrTag�Ľڵ��������strTagΪ���򷵻����нڵ����
//������
//	subTree�����ҷ�Χ��ֻ����subTree�µ�����
//	strTag:Ҫ���ҵı�ǩֵ
//////////////////////////////////////////////////////////////////////////
int FileTinyXml::countNode( TiXmlNode * const subTree, string &strTag )
{
	//TiXmlNode *parent = subTree;
	if( !subTree )
		return 0;
	
	int re = 0;
	//����ҵ��ڵ����һ�����strTagΪ���򷵻����б�ǩ����
	if(  subTree->Type() == 1 && ( subTree->ValueStr() == strTag  || strTag.empty() ) )
		++re;
	
	TiXmlNode *child;
	for( child = subTree->FirstChild(); child; child = child->NextSibling() )
	{
		re += countNode( child, strTag );
	}
	return re;
	//return countNode( subTree->FirstChild(), strTag ) + countNode( subTree->NextSibling(), strTag ) + re;
}
////////////////////////////////////////////////////////////////////////
//���ܣ�������������������Ϊtext�Ľڵ��а�����vecCount���ַ����ĸ���
//		���sCountStringΪ���򷵻�text�ĳ���
//		���ǽ�ѭ����Ϊһ�α���
//������
//	subTree�����ҷ�Χ��ֻ����subTree�µ�����
//	strTag:Ҫ���ҵı�ǩֵ
//////////////////////////////////////////////////////////////////////////
int FileTinyXml::countStrInText( TiXmlNode * const subTree, vector<string> &vecCount )
{
	//TiXmlNode *parent = subTree;
	if( !subTree )
		return 0;
	int re = 0;	
	//ͳ�Ʊ��ʱ��ͳ��<a>����������
	if( subTree->Type() == 1 && subTree->ValueStr() == "a" && !vecCount.empty() )
		return 0;
	if( subTree->Type() == 4 )		//����Ϊtext
	{
		//���sCountStringΪ���򷵻�text���ȣ�������Һ���strCount���ַ��ĸ���
		if( !vecCount.empty() )
		{
			string::size_type pos = -1;
			vector<string>::iterator iter, iter_end = vecCount.end();
			for( iter = vecCount.begin(); iter != iter_end; ++iter )
			{
				//������find_first_of�����ı��ռ�����ַ������Ҳ�׼ȷ
				while ( ( pos = (subTree->ValueStr()).find( *iter, pos + 1 ) ) != string::npos )		
				{
					++re;
				}	
			}
		}
		else
		{
			re = ( subTree->ValueStr() ).length();	
			//cout << "re:" << subTree->ValueStr() << re << "\t";
		}
	}
	TiXmlNode *child;
	for( child = subTree->FirstChild(); child; child = child->NextSibling() )
	{
		re += countStrInText( child, vecCount );
	}
	return re;
	/*��������д������û��ѭ������
	if( !subTree->FirstChild() )
	return re;
	else
	return countStrInText( subTree->FirstChild(), vecCount ) + countStrInText( subTree->FirstChild()->NextSibling(), vecCount ) + re;
	*/
}
////////////////////////////////////////////////////////////////////////
//���ܣ�����subTree�������б�ǩΪstrTag��������Ȩ�أ����ñ�������-0.3*���������������������form��input��Ȩ�س���
//		1 + 0.1*( form��input���ִ��� )
//������
//	resultNode�����ص����Ȩ�صĽڵ㡣��Ϊ�ڴ������ʱ����Tixmlnodeû�з����ڴ�
//				new TiXmlNode()����˺���Ҫ���β�ָ��ĵ�ַ���ز�����
//	maxWeight��subTree���ӽڵ��е����Ȩ��
//	vecPunct�������ż�
//	strTag����ǩ�ַ�������caculateWeight�����ǩȨ��
//////////////////////////////////////////////////////////////////////////
void FileTinyXml::maxWeightBlock( TiXmlNode *const subTree, set<string> &setTag, TiXmlNode *&resultNode, float &maxWeight, vector<string> &vecPunct )
{	
	if( !subTree )
		return;	
	//����ָ����ǩ��Ȩ��
	if( subTree->Type() == 1 && setTag.find( subTree->ValueStr() ) != setTag.end() )
	{
		string linkTag = "a";
		string strTemp;
		vector<string> vecTemp;
		//����Ȩ��		
		float weight = countStrInText( subTree, vecPunct ) - 0.3 * countNode( subTree, linkTag );		
		//����from��input�Ľڵ����Ȩ��
		if( weight >= maxWeight )
		{
			string t1 = "form";
			string t2 = "input";
			weight /= 1 + 0.2 * ( countNode( subTree, t1 ) + countNode( subTree, t2 ) );
		}	
		if( weight >= maxWeight )
		{
			resultNode = subTree;
			maxWeight = weight;
		}	
	}
	TiXmlNode *child;
	for( child = subTree->FirstChild(); child; child = child->NextSibling() )
	{		
		maxWeightBlock( child, setTag, resultNode, maxWeight, vecPunct );	
	}	
}
//////////////////////����ô˷����Ļ�����<table>�а���<p>��<p>�°���<a>���Ѵ�<p>ȥ������7_sina��
///////////////////////���Կ��ǰ��ձ�ǩ��<a>������������ռ�������ж��Ƿ�����Ⱥ�����ֻ�ȥ��3_sina��table��ǩ
////////////////////////////////////////////////////////////////////////
//���ܣ�ɾ��subTree�µ�����Ⱥ
//������
//	subTree�����ҷ�Χ��ֵ����subTree������
//	threshod���ж�����Ⱥ����ֵ�����ĳһ�ڵ��������a��ǩ���������б�ǩ������
//				���ڴ���ֵ��ɾ���˽ڵ㡣���㹫ʽ��a�ڵ�����������нڵ����
//////////////////////////////////////////////////////////////////////////
void FileTinyXml::delLinkGroup( TiXmlNode *const subTree, string &strTag, float threshold )
{
	if( !subTree )
		return;
	
	//TiXmlNode *parent = subTree;
	//����ÿһ����ǩ�Ƿ�������Ⱥ
	if( subTree->Type() == 1 && subTree->ValueStr().compare( strTag ) != 0 )
	{
	/*//����<a>��ǩ��ռ����
	string strEmpty = "";
	float weight = float( countNode( subTree, strTag ) ) / float( countNode( subTree, strEmpty ) ) ;
	//cout << parent->ValueStr() << " link:" << link << " node:" << node << " weight:" << weight << endl;
	//cout << parent->ValueStr() << " weight:" << weight << endl;
	if(  weight > threshold )
	{
	subTree->Clear();
	
	  //cout << subTree->ValueStr() << "clear!" << endl;
	  //system("pause");
	  return;
	}*/
		
		vector<string> vecTemp;
		int textLen = countStrInText( subTree, vecTemp );		//p�а���������������
		vector<TiXmlNode *> vecLinkNode;
		vecLinkNode.reserve( 20 );
		string tag = "a";
		extractNode( subTree, tag, vecLinkNode );				//��ȡ��p�а���������<a>�ڵ�
		int linkNum = 0;
		for( vector<TiXmlNode *>::iterator iter = vecLinkNode.begin(); iter != vecLinkNode.end(); ++iter )
		{
			//vecTemp.clear();
			linkNum += countStrInText( *iter, vecTemp );
			//cout << countStrInText( *iter, vecTemp ) << "\t";
			//system("pause");
		}
		
		//cout << "puncNum:" << puncNum << "\t" << "textLen:" << textLen << "\t" << "linkNum" << vecLinkNode.size() << "\tlinkLen:" << linkNum << endl;
		vecLinkNode.clear();
		//if(textLen > 0)
		//cout <<  (float)linkNum / (float)textLen << endl;
		//if( pChild && countStrInText( subTree, vecPunct ) > 0 )
		string strEmpty = "";
		float weight = 0.35 * float( countNode( subTree, strTag ) ) / ( float( countNode( subTree, strEmpty ) ) + 1 ) + 0.65*(float)linkNum / ( 1 + (float)textLen );
		if( textLen > 0 && weight  > 0.5 )
		{
			//cout << "linknum:" << linkNum << "\ttextLen:" << textLen << endl;
			//cout << subTree->ValueStr() << endl;
			subTree->Clear();
			//system("pause");
			return;
		}
		
	}
	
	TiXmlNode *child;
	for( child = subTree->FirstChild(); child; child = child->NextSibling() )
		delLinkGroup( child, strTag, threshold );
}

////////////////////////////////////////////////////////////////////////
//���ܣ���ȡ�ļ�
//������
//	subTree�����ҷ�Χ��ֵ����subTree������
//	mianText����ŷ��ص�text����
//	direction: ���Ϊ1������push_back��queMainText������ͷ�������ݲ���quemaintextͷָ������ƶ�-direction��λ��
//////////////////////////////////////////////////////////////////////////
void FileTinyXml::extractText( TiXmlNode * const subTree, deque<string> &queMainText, int &direction )
{
	if( !subTree )
		return;
	
	//TiXmlNode *parent = subTree;
	//ֻ��ȡ��������һ�����ֵ�����
	if( subTree->Type() == 4 && (subTree->ValueStr()).length() > 2 )
	{
		//cout << "\n\ntype:4\n" << endl; 
		//cout << subTree->ValueStr() << endl;
		if( direction > 0 )
		{
			//���������Ƿ���Լ�һ����������־������Ҫ������begin����ٸ�Ԫ��֮ǰ����
			queMainText.push_back( subTree->ValueStr() );
			
			//queMainText.insert( queMainText.begin(), subTree->ValueStr() );
		}
		else						//��direction����ֵ�����Ʋ���λ��ʹ��ȡ���ݰ�����˳������
		{
			deque<string>::iterator iter = queMainText.begin();
			//cout << direction << endl;
			for( int i = 0; i > direction; --i )
				iter++;
			queMainText.insert( iter, subTree->ValueStr() );
			//cout << subTree->ValueStr();
			
			//copy( queMainText.begin(), queMainText.end(), ostream_iterator<string>( cout, "\n" ) );
			//system("pause");
			--direction;
		}
		return;
	}
	//cout << subTree->ValueStr() << endl;
	//system("pause");
	TiXmlNode *child;
	for( child = subTree->FirstChild(); child; child = child->NextSibling() )
	{
		extractText( child, queMainText, direction );
	}
}

////////////�����޸ģ������sepTreeǰ�ж��p����Щp�������ǵ����
///////////�޸ģ����<p>�к��б�㲢��Ȩֵ����ĳһֵ����ȡ��Ȩֵ�����ñ������ȥ<a>����������������<p>�������ı���
////////////////////////////////////////////////////////////////////////
//���ܣ���ȡ�ļ��е�<p>��ǩ�µ����ݣ����<p> ��ǩ��sepTreeǰ����
//		���ݷ���mainText���������ݵ�ǰ����������ں��档ֻ��ȡ����
//		���б�������
//������
//	subTree�����ҷ�Χ��ֻ����subTree������
//	mianText����ŷ��ص�text����
//////////////////////////////////////////////////////////////////////////
void FileTinyXml::extractPText( TiXmlNode * const subTree, TiXmlNode *sepTree, deque< string > &mainText, int &direction, vector<string> &vecPunct, int &maxLink )
{
	if( !subTree || maxLink < 0 )
		return;
	
	if( direction > 0 && subTree->Type() == 1 && subTree->FirstChild() )
	{
		if( subTree->ValueStr() == "a" )				//����a���һ
			--maxLink;
		else if( subTree->ValueStr() == "div" || subTree->ValueStr() == "hr" )			//����div�����
			maxLink /= 2;
		else if( subTree->ValueStr() == "input" || subTree->ValueStr() == "form" )
			maxLink /= 1.2;
	}
	
	
	//����sepTree��ת������¼��sepTree�ķ����Ա�֤��ҳ��������mainText�а�˳���ŷţ�����sepTree��û�����µݹ��������sepTree�ڵ�
	if( subTree == sepTree )
	{
		direction = 1;
	}
	else if( subTree->Type() == 1 && subTree->ValueStr() == "p" )
	{
		
		TiXmlNode *pChild = subTree->FirstChild();
		
		//ֻ��ȡֱ�Ӹ����ֲ��Һ��б����ŵ�p�ڵ�
		//int puncNum = countStrInText( subTree, vecPunct );		//p�а����ı����
		vector<string> vecTemp;
		int textLen = countStrInText( subTree, vecTemp );		//p�а���������������
		vector<TiXmlNode *> vecLinkNode;
		vecLinkNode.reserve( 20 );
		string tag = "a";
		extractNode( subTree, tag, vecLinkNode );				//��ȡ��p�а���������<a>�ڵ�
		int linkNum = 0;
		for( vector<TiXmlNode *>::iterator iter = vecLinkNode.begin(); iter != vecLinkNode.end(); ++iter )
		{
			//vecTemp.clear();
			linkNum += countStrInText( *iter, vecTemp );
			//cout << countStrInText( *iter, vecTemp ) << "\t";
			//system("pause");
		}
		
		//cout << "puncNum:" << puncNum << "\t" << "textLen:" << textLen << "\t" << "linkNum" << vecLinkNode.size() << "\tlinkLen:" << linkNum << endl;
		vecLinkNode.clear();
		//if(textLen > 0)
		//cout <<  (float)linkNum / (float)textLen << endl;
		//if( pChild && countStrInText( subTree, vecPunct ) > 0 )
		string strTag = "a";
		string strEmpty = "";
		float weight = 0.35 * float( countNode( subTree, strTag ) ) / ( 1 + float( countNode( subTree, strEmpty ) )  ) + 0.65*(float)linkNum / ( 1 + (float)textLen );
		//if( pChild && countStrInText( subTree, vecPunct ) > 0 && textLen > 0 && (float)linkNum / (float)textLen  < 0.5 )
		if( pChild && countStrInText( subTree, vecPunct ) > 0 && textLen > 0 && weight  < 0.5 )
			extractText( subTree, mainText, direction );
		
	}
	else
	{
		TiXmlNode *child;
		for( child = subTree->FirstChild(); child; child = child->NextSibling() )
		{
			extractPText( child, sepTree, mainText, direction, vecPunct, maxLink );
		}
	}
}

///////////////////////δ����
////////////////////////////////////////////////////////////////////////
//���ܣ���ȡsubTree�µ�һ����ǩΪstrTag���Һ����ֽڵ�Ľڵ㷵��
//������
//	subTree�����ҷ�Χ��ֻ����subTree������
//	mianText����ŷ��ص�text����
//////////////////////////////////////////////////////////////////////////
/*bool FileTinyXml::extractNode( TiXmlNode *const subTree, string &strTag, TiXmlNode *&reNode )
{
if( !subTree )
return false;

  //����ҵ��ڵ��򷵻�
  if(  subTree->Type() == 1 && subTree->ValueStr() == strTag && subTree->FirstChild() )
  {
		//vecNode.push_back( &subTree );
		reNode = subTree;
		//cout << "extract node:" << reNode->ValueStr() << endl;
		return true;
		}
		else
		{
		for( TiXmlNode *child = subTree->FirstChild(); child; child = child->NextSibling() )
		if( extractNode( child, strTag, reNode ) )
		return true;
		
		  
			}
}*/

////////////////////////////////////////////////////////////////////////
//���ܣ���ȡsubTree������tag��ǩ�ڵ㣬���������vecNode��
//������
//	subTree�����ҷ�Χ��ֻ����subTree������
//	strTag: Ҫ���ҵı�ǩ
//	vecNode:��ű�ǩ�ڵ�
//////////////////////////////////////////////////////////////////////////
void FileTinyXml::extractNode( TiXmlNode *const subTree, string &strTag, vector< TiXmlNode * > &vecNode )
{
	if( !subTree )
		return;
	
	//����ҵ��ڵ��򷵻�&& subTree->FirstChild()
	if(  subTree->Type() == 1 && subTree->ValueStr() == strTag  )
	{
		//vecNode.push_back( &subTree );
		//reNode = subTree;
		vecNode.push_back( subTree );
		//cout << "extract node:" << reNode->ValueStr() << endl;
	}
	
	for( TiXmlNode *child = subTree->FirstChild(); child; child = child->NextSibling() )
		extractNode( child, strTag, vecNode ) ;				
	
}

string FileTinyXml::extractContent( deque<string> &queText )
{
	TiXmlElement *const root = m_doc.RootElement();
	
	//ɾ����ǩ
	set<string> sDelTag;
	/*sDelTag.insert( "select" );
	sDelTag.insert( "img" );
	sDelTag.insert( "script" );
	sDelTag.insert( "input" );
	sDelTag.insert( "span" );
	sDelTag.insert( "form" );
	*/
	sDelTag.insert( "script" );
	sDelTag.insert( "select" );
	sDelTag.insert( "img" );
	
	deleteNode( root, sDelTag );
	sDelTag.clear();
	
	TiXmlNode *maxBlock = 0;
	float maxWeight = 0;
	string strTag = "table";
	set<string> setTag;
	setTag.insert( "table" );
	setTag.insert( "div" );
	vector<string> vecPunct;
	vecPunct.reserve( 3 );
	vecPunct.push_back( "��" );
	vecPunct.push_back( "��" );
	vecPunct.push_back( "��" );
	//system("pause");
	maxWeightBlock( root, setTag, maxBlock, maxWeight, vecPunct );
	
	//cout << maxBlock->ValueStr() << ":" << maxWeight << endl;
	
	/*if( !maxBlock )
	cout << "caculate max empty" << endl;
	else
	cout << "caculate max " << maxBlock->ValueStr() << endl;
	system("pause");*/
	
	//ɾ�����Ȩ�ر�ǩ�е�����Ⱥ
	float threshold = 0.45;
	string linkTag = "a";
	delLinkGroup( maxBlock, linkTag, threshold );
	//deque<string> queText;
	int direction = 1;
	//cout << "delet link group" << endl;
	//system( "pause" );
	
	//��ȡ����
	extractText( maxBlock, queText, direction );
	
	//cout << queText.size() << endl;
	//extractText( maxBlock, queText );
	/*cout << "extract main:"<< endl;
	for( deque<string>::iterator iter = queText.begin(); iter != queText.end(); ++iter )
	{
	cout << *iter << endl;
}*/
	//system("pause");
	//cout << "extract main text" << endl;
	//system("pause");
	
	////////////////////////////��������table�����ֶ������жϱ���pʱ��������<a>����,������table�����һ����ǩ��ʼ����������������ݵ�
	//////////////////a��ǩ�ĸ���������ֵ������ȡp����ֵ�����table�ַ���С��50����ֵΪ���޴���200����Ϊ15��200����Ϊ7
	//????????????????contentLen��maxLink�Ĺ�ϵ���д���һ������???????????????
	//�����������ĳ���
	int contentLen = 0;
	//cout << queText.begin()->length();
	deque<string>::iterator qiter, qiter_end = queText.end();
	for( qiter = queText.begin(); qiter != qiter_end; ++qiter )
	{
		contentLen += qiter->length();
		//cout << qiter->length << "\t" << contentLen << endl;
	}
	int maxLink;
	if( contentLen <= 50 )
		maxLink = 100;
	else if( contentLen <= 100 )
		maxLink = 20;
	else if( contentLen <=200 )
		maxLink = 13;
	else
		maxLink = 7;
	//cout << queText.size() << "\t" << contentLen << "\t" << maxLink << endl;
	//system("pause");
	///////////////////////////////////////////
	direction = 0;
	
	extractPText( root, maxBlock, queText, direction, vecPunct, maxLink ); 
	//cout << queText.size() << endl;
	//system( "pause" );
	//cout << "after extract P:" << endl;
	/*for( iter = queText.begin(); iter != queText.end(); ++iter )
	{
	cout << *iter << endl;
}*/
	//queText.clear();
	//cout << "extract P!" << endl;
	//system("pause");
	vecPunct.clear();
	return "";
}

/*char * FileTinyXml::extract()
{
FileTidy tidyFile( m_srcFilePath );
tidyFile.mainTidy();

  //ʹ��tinyxml�������νṹ
  FileTinyXml tinyXmlFile( m_srcFilePath.c_str() );
  bool loadOk = tinyXmlFile.loadSrcFile();
		
		  if( !loadOk )
		  return NULL;
		  
			//��ȡ�ļ�������
			deque<string> queText;
			tinyXmlFile.extractContent( queText );
			
			  string content;
			  deque<string>::const_iterator dIter, dIter_end = queText.end();
			  for( dIter = queText.begin(); dIter != dIter_end; ++dIter )
			  {
			  content += *dIter;
			  }
			  
				if( m_result != NULL )
				delete[] m_result;
				m_result = new char[content.size() + 1];
				strcpy( m_result, content.c_str() );
				
				  return m_result;
				  }
				  */