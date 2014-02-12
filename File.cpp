
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "File.h"

using namespace std;

//read file to set
bool File::read( set<string>& desSet )
{
	if( m_strPath.empty() )
		return false;

	//打开文件
	FILE *fp;
	fp = fopen( m_strPath.c_str(), "rb");
	if (!fp)
	{
		cerr << "can't open " << m_strPath << endl;
		return false;
	}

	//计算文件长度
	long fileLen;
	fseek( fp, 0, SEEK_END );			//将指针移动到文件末尾
	fileLen = ftell( fp );				//计算文件长度
	if( fileLen == 0 ) {
        fclose(fp);
		return true;
    }
	fseek( fp, 0, SEEK_SET );			//重新将指针移动到文件开头
	char *buf = new char[fileLen + 1];
	
	if (!buf)
	{
		cerr << "new memory error" << endl;
        fclose(fp);
		return false;
	}

	//读取文件
	fread( buf, fileLen, 1, fp );	//读取文件
	buf[fileLen] = '\0';			//文件结尾放置字符串结束符
	
	//将换行符变为字符串结束字符。注意：是针对Linux系统，因为Linux换行符为\n而Windows为\r\n
	for( int i = fileLen - 1; i >= 0; --i )		
		if( buf[i] == g_lineEnd )
			buf[i] = '\0';

	char *line = buf;					//指向行的指针
	int lineLen, pos;					//行长度
	while ( line <= buf + fileLen ) 
	{
		lineLen = strlen( line );		
		
		//如果此行包含正常字符则插入
		for( pos = 0; pos < lineLen && ( line[ pos ] == ' ' || line[ pos ] == '\t'|| line[ pos ] == g_lineEnd  ) ; ++pos );
			if( pos < lineLen )
			desSet.insert( line );
		

		line += lineLen + g_lineEndLen;			//跨到下一行开始字符
	}

	fclose(fp);
	delete[] buf;
	buf = NULL;

	return true;
}

//将文档内容每行读入vector
bool File::read( vector<string>& desVec )
{
	
	if( m_strPath.empty() )
		return false;

	//打开文件
	FILE *fp;
	fp = fopen( m_strPath.c_str(), "rb");
	if (!fp)
	{
		cerr << "can't open " << m_strPath << endl;
		return false;
	}

	//计算文件长度
	long fileLen;
	fseek( fp, 0, SEEK_END );			//将指针移动到文件末尾
	fileLen = ftell( fp );				//计算文件长度
	if( fileLen == 0 ) {
        fclose(fp);
		return true;
    }
	fseek( fp, 0, SEEK_SET );			//重新将指针移动到文件开头
	char *buf = new char[fileLen + 1];
	
	if (!buf)
	{
        fclose(fp);
		cerr << "new memory error" << endl;
		return false;
	}

	//读取文件
	fread( buf, fileLen, 1, fp );	//读取文件
	buf[fileLen] = '\0';			//文件结尾放置字符串结束符
	
	//将换行符变为字符串结束字符。注意：是针对Linux系统，因为Linux换行符为\n而Windows为\r\n
	int lineNum = 1;
	for( int i = fileLen - 1; i >= 0; --i )		
	if( buf[i] == g_lineEnd )
	{
			buf[i] = '\0';
		++lineNum;
	}
	desVec.reserve( desVec.size() + lineNum );

	char *line = buf;					//指向行的指针
	int lineLen, pos;					//行长度
	while ( line <= buf + fileLen ) 
	{
		lineLen = strlen( line );		
		
		//如果此行包含正常字符则插入
		for( pos = 0; pos < lineLen && ( line[ pos ] == ' ' || line[ pos ] == '\t'|| line[ pos ] == g_lineEnd  ) ; ++pos );
			if( pos < lineLen )
				desVec.push_back( line );

		line += lineLen + g_lineEndLen;			//跨到下一行开始字符
	}

	fclose(fp);
	delete[] buf;
	buf = NULL;
	return true;
}

//用fwrite写入文件
bool File::write(const string &content )
{
	if( m_strPath.empty() )
		return false;

	FILE *fp;
	fp = fopen( m_strPath.c_str(), "w" );
	if( !fp )
	{
		cerr << "can't open file:" << m_strPath << endl;
		return false;
	}

	fwrite( content.c_str(), content.size(), 1, fp );

	fclose(fp);
	return true;
}

//write vector to file
bool File::write( vector<string>& srcVec )
{
	if( m_strPath.empty() )
		return false;

	ofstream outFile( m_strPath.c_str() );
	if( !outFile )
		return false;

	vector<string>::iterator iter, iterEnd = srcVec.end();
	for( iter = srcVec.begin(); iter != iterEnd; ++iter )
	{
		//if( !iter->empty() && iter->find_first_not_of( " \t" ) != string::npos )		//去除空行和没有文字的行
		outFile << (*iter).c_str() << endl;
	}
	outFile.close();
	return true;

}


bool File::write( const deque<string> &srcQueue )
{
	if( m_strPath.empty() )
		return false;

	string content;
	deque<string>::const_iterator dIter, dIter_end = srcQueue.end();
	for( dIter = srcQueue.begin(); dIter != dIter_end; ++dIter )
	{
		content += *dIter;
	}

	FILE *fp;
	fp = fopen( m_strPath.c_str(), "w" );
	if( !fp )
	{
		cerr << "can't open file:" << m_strPath << endl;
		return false;
	}

	fwrite( content.c_str(), content.size(), 1, fp );

	fclose(fp);
	return true;
}
