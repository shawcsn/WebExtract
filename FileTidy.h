#ifndef FILETIDY_H
#define FILETIDY_H

#include <iostream>
#include <vector>

using namespace std;

//the class to use the xmltidy to modify html file
class FileTidy
{
public:
	FileTidy(){};
	
	//input html file path
	FileTidy( const string& filePath ){ srcFilePath = filePath; }				
	
	~FileTidy(){ vFileContent.clear(); }
	
	//get the name of the file 
	void getFileName( string& fileName );
	//print the vector content
	void printVector();
	
	//the main function of the class FileTidy
	bool mainTidy();
	
	
protected:
	//read the file content to vector
	bool readFileToVector();
	//convert the string of the vector to lower
	void convertToLower();
	//remove some strings from the source file ( &nbsp e.g )
	void removeString( string& stringDel );
	
	//add xml header to the file and write the vector to the file
	bool addHeadToFile( string& strAdded );
	//write the content of the vector to the file
	bool writeVectorToSrcFile();
	
	int tidyFile( string originalfile );
	
private:
	string srcFilePath;		//the name of source file
	vector<string> vFileContent;
};

#endif