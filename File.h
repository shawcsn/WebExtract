#ifndef __FILE_H__
#define __FILE_H__

#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <deque>

using namespace std;


#ifdef _WIN32
//windows系统的文件结束行标志"\r\n"和长度
const char g_lineEnd = '\r';
const int g_lineEndLen = 2;

#else
//linux文件结束符
const char g_lineEnd = '\n';
const int g_lineEndLen = 1;

#endif

class File
{
public:
	File(){};
	File( const string& path ){ m_strPath = path; }
	//read file to vector( don't clear the vector )
	void setPath( const string &path ){ m_strPath = path; }
	
	bool read( vector<string>& desVec );
	
	bool read( set<string>& desSet );
	
	//write vector to file( don't care if the vector is empty )
	bool write( vector<string>& srcVec );
	
	bool write( const string &content );
	
	bool write( const deque<string> &srcQueue );
protected:
private:
	string m_strPath;
};


#endif