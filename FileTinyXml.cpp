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
//功能：删除指定标签节点以及只含有<a>子节点的节点
//参数：
//	subTree：查找范围，只查找subTree下的子树
//	vDelTag：要删除标签集
//////////////////////////////////////////////////////////////////////////
bool FileTinyXml::deleteNode( TiXmlNode* const subTree, set<string> &sDelTag )
{
	if( !subTree )
		return true;
	//TiXmlNode *parent = subTree;		//拷贝subTree防止修改
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
//功能：计算subTree子树中值为strTag的节点数，如果strTag为空则返回所有节点个数
//参数：
//	subTree：查找范围，只查找subTree下的子树
//	strTag:要查找的标签值
//////////////////////////////////////////////////////////////////////////
int FileTinyXml::countNode( TiXmlNode * const subTree, string &strTag )
{
	//TiXmlNode *parent = subTree;
	if( !subTree )
		return 0;
	
	int re = 0;
	//如果找到节点则加一，如果strTag为空则返回所有标签个数
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
//功能：查找子树中所有类型为text的节点中包含在vecCount中字符串的个数
//		如果sCountString为空则返回text的长度
//		考虑将循环改为一次遍历
//参数：
//	subTree：查找范围，只查找subTree下的子树
//	strTag:要查找的标签值
//////////////////////////////////////////////////////////////////////////
int FileTinyXml::countStrInText( TiXmlNode * const subTree, vector<string> &vecCount )
{
	//TiXmlNode *parent = subTree;
	if( !subTree )
		return 0;
	int re = 0;	
	//统计标点时不统计<a>包含的内容
	if( subTree->Type() == 1 && subTree->ValueStr() == "a" && !vecCount.empty() )
		return 0;
	if( subTree->Type() == 4 )		//类型为text
	{
		//如果sCountString为空则返回text长度，否则查找含有strCount中字符的个数
		if( !vecCount.empty() )
		{
			string::size_type pos = -1;
			vector<string>::iterator iter, iter_end = vecCount.end();
			for( iter = vecCount.begin(); iter != iter_end; ++iter )
			{
				//不可用find_first_of，中文标点占两个字符，查找不准确
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
	/*不能这样写，这样没法循环起来
	if( !subTree->FirstChild() )
	return re;
	else
	return countStrInText( subTree->FirstChild(), vecCount ) + countStrInText( subTree->FirstChild()->NextSibling(), vecCount ) + re;
	*/
}
////////////////////////////////////////////////////////////////////////
//功能：计算subTree的子树中标签为strTag的子树的权重，采用标点符号数-0.3*链接数，并且如果出现了form或input则将权重除以
//		1 + 0.1*( form和input出现次数 )
//参数：
//	resultNode：返回的最大权重的节点。因为在传入参数时定义Tixmlnode没有分配内存
//				new TiXmlNode()，因此函数要将形参指针的地址返回才能用
//	maxWeight：subTree的子节点中的最大权重
//	vecPunct：标点符号集
//	strTag：标签字符串，用caculateWeight计算标签权重
//////////////////////////////////////////////////////////////////////////
void FileTinyXml::maxWeightBlock( TiXmlNode *const subTree, set<string> &setTag, TiXmlNode *&resultNode, float &maxWeight, vector<string> &vecPunct )
{	
	if( !subTree )
		return;	
	//计算指定标签的权重
	if( subTree->Type() == 1 && setTag.find( subTree->ValueStr() ) != setTag.end() )
	{
		string linkTag = "a";
		string strTemp;
		vector<string> vecTemp;
		//计算权重		
		float weight = countStrInText( subTree, vecPunct ) - 0.3 * countNode( subTree, linkTag );		
		//出现from和input的节点减少权重
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
//////////////////////如果用此方法的话，若<table>中包含<p>且<p>下包含<a>则会把此<p>去除，如7_sina，
///////////////////////可以考虑按照标签中<a>包含文字数所占比例来判断是否链接群，但又会去除3_sina的table标签
////////////////////////////////////////////////////////////////////////
//功能：删除subTree下的链接群
//参数：
//	subTree：查找范围，值查找subTree的子树
//	threshod：判定链接群的阈值。如果某一节点的子树中a标签个数与所有标签个数比
//				大于此阈值则删除此节点。计算公式：a节点个数除以所有节点个数
//////////////////////////////////////////////////////////////////////////
void FileTinyXml::delLinkGroup( TiXmlNode *const subTree, string &strTag, float threshold )
{
	if( !subTree )
		return;
	
	//TiXmlNode *parent = subTree;
	//计算每一个标签是否含有链接群
	if( subTree->Type() == 1 && subTree->ValueStr().compare( strTag ) != 0 )
	{
	/*//按照<a>标签所占比例
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
		int textLen = countStrInText( subTree, vecTemp );		//p中包含的所有文字数
		vector<TiXmlNode *> vecLinkNode;
		vecLinkNode.reserve( 20 );
		string tag = "a";
		extractNode( subTree, tag, vecLinkNode );				//提取出p中包含的所有<a>节点
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
//功能：提取文件
//参数：
//	subTree：查找范围，值查找subTree的子树
//	mianText：存放返回的text内容
//	direction: 如果为1则将文字push_back入queMainText，否则头将新内容插入quemaintext头指针向后移动-direction的位置
//////////////////////////////////////////////////////////////////////////
void FileTinyXml::extractText( TiXmlNode * const subTree, deque<string> &queMainText, int &direction )
{
	if( !subTree )
		return;
	
	//TiXmlNode *parent = subTree;
	//只提取包含多于一个汉字的内容
	if( subTree->Type() == 4 && (subTree->ValueStr()).length() > 2 )
	{
		//cout << "\n\ntype:4\n" << endl; 
		//cout << subTree->ValueStr() << endl;
		if( direction > 0 )
		{
			//产生倒序，是否可以加一个整数来标志现在需要插入离begin后多少个元素之前？？
			queMainText.push_back( subTree->ValueStr() );
			
			//queMainText.insert( queMainText.begin(), subTree->ValueStr() );
		}
		else						//用direction的数值来控制插入位置使提取内容按文章顺序排列
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

////////////正在修改：如果在sepTree前有多个p则这些p的内容是倒序的
///////////修改：如果<p>中含有标点并且权值大于某一值则提取，权值可以用标点数减去<a>包含的文字数除以<p>包含总文本数
////////////////////////////////////////////////////////////////////////
//功能：提取文件中的<p>标签下的内容，如果<p> 标签在sepTree前则将其
//		内容放在mainText中已有内容的前方，否则放在后面。只提取所有
//		含有标点的内容
//参数：
//	subTree：查找范围，只查找subTree的子树
//	mianText：存放返回的text内容
//////////////////////////////////////////////////////////////////////////
void FileTinyXml::extractPText( TiXmlNode * const subTree, TiXmlNode *sepTree, deque< string > &mainText, int &direction, vector<string> &vecPunct, int &maxLink )
{
	if( !subTree || maxLink < 0 )
		return;
	
	if( direction > 0 && subTree->Type() == 1 && subTree->FirstChild() )
	{
		if( subTree->ValueStr() == "a" )				//遇到a则减一
			--maxLink;
		else if( subTree->ValueStr() == "div" || subTree->ValueStr() == "hr" )			//遇到div则减半
			maxLink /= 2;
		else if( subTree->ValueStr() == "input" || subTree->ValueStr() == "form" )
			maxLink /= 1.2;
	}
	
	
	//遇到sepTree则转变正文录入sepTree的方向以保证网页中正文再mainText中按顺序排放，遇到sepTree后没有往下递归而跳过了sepTree节点
	if( subTree == sepTree )
	{
		direction = 1;
	}
	else if( subTree->Type() == 1 && subTree->ValueStr() == "p" )
	{
		
		TiXmlNode *pChild = subTree->FirstChild();
		
		//只提取直接跟文字并且含有标点符号的p节点
		//int puncNum = countStrInText( subTree, vecPunct );		//p中包含的标点数
		vector<string> vecTemp;
		int textLen = countStrInText( subTree, vecTemp );		//p中包含的所有文字数
		vector<TiXmlNode *> vecLinkNode;
		vecLinkNode.reserve( 20 );
		string tag = "a";
		extractNode( subTree, tag, vecLinkNode );				//提取出p中包含的所有<a>节点
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

///////////////////////未测试
////////////////////////////////////////////////////////////////////////
//功能：提取subTree下第一个标签为strTag并且含有字节点的节点返回
//参数：
//	subTree：查找范围，只查找subTree的子树
//	mianText：存放返回的text内容
//////////////////////////////////////////////////////////////////////////
/*bool FileTinyXml::extractNode( TiXmlNode *const subTree, string &strTag, TiXmlNode *&reNode )
{
if( !subTree )
return false;

  //如果找到节点则返回
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
//功能：提取subTree下所有tag标签节点，并将其放入vecNode中
//参数：
//	subTree：查找范围，只查找subTree的子树
//	strTag: 要查找的标签
//	vecNode:存放标签节点
//////////////////////////////////////////////////////////////////////////
void FileTinyXml::extractNode( TiXmlNode *const subTree, string &strTag, vector< TiXmlNode * > &vecNode )
{
	if( !subTree )
		return;
	
	//如果找到节点则返回&& subTree->FirstChild()
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
	
	//删除标签
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
	vecPunct.push_back( "，" );
	vecPunct.push_back( "。" );
	vecPunct.push_back( "；" );
	//system("pause");
	maxWeightBlock( root, setTag, maxBlock, maxWeight, vecPunct );
	
	//cout << maxBlock->ValueStr() << ":" << maxWeight << endl;
	
	/*if( !maxBlock )
	cout << "caculate max empty" << endl;
	else
	cout << "caculate max " << maxBlock->ValueStr() << endl;
	system("pause");*/
	
	//删除最大权重标签中的链接群
	float threshold = 0.45;
	string linkTag = "a";
	delLinkGroup( maxBlock, linkTag, threshold );
	//deque<string> queText;
	int direction = 1;
	//cout << "delet link group" << endl;
	//system( "pause" );
	
	//提取正文
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
	
	////////////////////////////按照正文table中文字多少来判断遍历p时经过多少<a>结束,从正文table的最后一个标签开始，如果经过含有内容的
	//////////////////a标签的个数大于阈值则不再提取p。阈值：如果table字符数小于50则阈值为无限大，在200内则为15，200以上为7
	//????????????????contentLen与maxLink的关系还有待进一步商量???????????????
	//计算现在正文长度
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

  //使用tinyxml建立树形结构
  FileTinyXml tinyXmlFile( m_srcFilePath.c_str() );
  bool loadOk = tinyXmlFile.loadSrcFile();
		
		  if( !loadOk )
		  return NULL;
		  
			//提取文件的正文
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