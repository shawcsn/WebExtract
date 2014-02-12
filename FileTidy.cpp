#include "tidy.h"
#include "buffio.h"
#include <stdio.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "FileTidy.h"
#include "File.h"

using namespace std;

//read the content of the file into vector
bool FileTidy::readFileToVector( )
{
	if ( srcFilePath.empty() ) 
		return false;
	
	vFileContent.clear();
	
	File inFile( srcFilePath );
	
	
	return inFile.read( vFileContent );
	/*
	ifstream inFile( srcFilePath.c_str() );
	
	  if ( !inFile ) 
	  {
	  return false;
	  }
	  
		//clear vector
		vFileContent.clear();
		string textLine;
		//read text into vector
		while ( getline( inFile, textLine ) ) 
		{
		if ( !textLine.empty() && textLine.find_first_not_of( " \t" ) != string::npos ) 
		{
		vFileContent.push_back( textLine );
		}
		}
		inFile.close();
	return true;*/
}

//print the content of vector
/*void FileTidy::printVector()
{
if ( !vFileContent.empty() ) 
{
copy( vFileContent.begin(), vFileContent.end(), ostream_iterator<string>( cout, "\n" ) );
}
}*/

//convert the string to lower
void FileTidy::convertToLower()
{
	vector<string>::iterator vIterBegin, vIterEnd;
	vIterBegin = vFileContent.begin();
	vIterEnd = vFileContent.end();
	
	string::size_type pos, pos_end;
	for( ; vIterBegin != vIterEnd; vIterBegin++ )
	{
		//transform( (*vIterBegin).begin(), (*vIterBegin).end(), (*vIterBegin).begin(), tolower );
		for( pos = 0, pos_end = vIterBegin->size(); pos < pos_end; ++pos )
			if( (*vIterBegin)[pos] >= 0x41 && (*vIterBegin)[pos] <= 0x5a  )
				(*vIterBegin)[pos] += 0x20;
			
	}
	
}
//remove some string in the vector
void FileTidy::removeString( string& stringDel )
{
	vector<string>::iterator vIterBegin, vIterEnd;
	vIterBegin = vFileContent.begin();
	vIterEnd = vFileContent.end();
	
	for( ; vIterBegin != vIterEnd; vIterBegin++ )			//处理每一行
	{
		string::size_type pos = (*vIterBegin).find( stringDel );
		
		//erase the stringDel in vector
		while ( pos != string::npos ) 
		{
			(*vIterBegin).erase( pos, stringDel.length() );
			pos = (*vIterBegin).find( stringDel );
		}
	}
}

//add xml decleration to the head of the file
bool FileTidy::addHeadToFile( string& strAdded )
{
	ofstream outFile( srcFilePath.c_str() );
	if ( !outFile ) 
		return false;
	
	outFile << strAdded << endl;			//add head decleration
	
	//store the vector content into file
	vector<string>::iterator iterBegin, iterEnd = vFileContent.end();
	for( iterBegin = vFileContent.begin(); iterBegin != iterEnd; iterBegin++ )
	{
		outFile << (*iterBegin).c_str() << endl;
	}
	outFile.close();
	return true;
}

//write the vector to the file
bool FileTidy::writeVectorToSrcFile()
{
	if( vFileContent.empty() )
		return true;
	
	File outFile( srcFilePath );
	return outFile.write( vFileContent );
	/*ofstream outFile( srcFilePath.c_str() );
	if ( !outFile ) 
	return false;
	
	  //store the vector content into file
	  vector<string>::iterator iterBegin, iterEnd = vFileContent.end();
	  for( iterBegin = vFileContent.begin(); iterBegin != iterEnd; iterBegin++ )
	  {
	  outFile << (*iterBegin).c_str() << endl;
	  }
	  outFile.close();
	return true;*/
}

////////////////////can't use, link erro////////////////////


//use tidy utility to tidy the file
int FileTidy::tidyFile( string originalfile )
{
	char const * input = srcFilePath.c_str();
	TidyBuffer output = {0};
	TidyBuffer errbuf = {0};
	int rc = -1;
	// int doctypemode;
	//  int indentspace;
	Bool ok;
	
	TidyDoc tdoc = tidyCreate();                     // Initialize "document"
	//  printf( "Tidying:\t%s\n", input );
	
	ok = tidyOptSetValue  ( tdoc, TidyCharEncoding, "raw" );// Change Character Encoding
	
	// Set the TidyDoctypeMode to be strict.
	//  doctypemode = tidyOptGetInt( tdoc, TidyDoctypeMode);
	// cout << "The original value of doctypemode is " << doctypemode << endl;
	ok = tidyOptSetInt( tdoc, TidyDoctypeMode, 2);
	//  doctypemode = tidyOptGetInt( tdoc, TidyDoctypeMode);
	//  cout << "The current value of doctypemode is " << doctypemode << endl;
	
	// Set the TidyIndentSpaces to be 0.
	//  indentspace = tidyOptGetInt (tdoc, TidyIndentSpaces);
	// cout << "The original value of indentspace is " << indentspace << endl;
	ok = tidyOptSetInt( tdoc, TidyIndentSpaces, 0);
	//  indentspace = tidyOptGetInt( tdoc, TidyIndentSpaces);
	// cout << "The current value of indentspace is " << indentspace << endl;
	
	// Make sure the output is XML.
	ok = tidyOptSetBool( tdoc, TidyXmlOut, yes );  // Convert to XML
	
	// Add <?xml?> for XML docs.
	ok = tidyOptSetBool(tdoc, TidyXmlDecl, yes);
	//cout << ok << endl;
	
	// Hide all comments.
	ok = tidyOptSetBool( tdoc, TidyHideComments, yes ); 
	
	ok = tidyOptSetBool( tdoc, TidyDropFontTags, yes );
	
	if ( ok )
		rc = tidySetErrorBuffer( tdoc, &errbuf );      // Capture diagnostics
	if ( rc >= 0 )
		rc = tidyParseFile( tdoc, input );           // Parse the input
	if ( rc >= 0 )
	{
		rc = tidyCleanAndRepair( tdoc );               // Tidy it up!
		//cout << endl <<"Tidy it up!" << endl;
	}
	if ( rc >= 0 )
		rc = tidyRunDiagnostics( tdoc );               // Kvetch
	if ( rc > 1 )                                    // If error, force output.
		rc = ( tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1 );
	if ( rc >= 0 ){
		rc = tidySaveFile( tdoc, input ); 
		//cout << "Tidy saved file." << endl;         // Pretty Print
    }
	
	/* if ( rc >= 0 )
	{
    if ( rc > 0 )
	printf( "\nDiagnostics:\n\n%s", errbuf.bp );
    printf( "\nAnd here is the result:\n\n%s", output.bp );
	}
	else
    printf( "A severe error (%d) occurred.\\n", rc );
	*/
	tidyBufFree( &output );
	tidyBufFree( &errbuf );
	tidyRelease( tdoc );
	return rc;
}

bool FileTidy::mainTidy()
{
	if ( srcFilePath.empty() ) 
		return false;
	
	//read the source file to the vector
	bool flag = readFileToVector();
	if( !flag || vFileContent.empty() )		//return if fail to read the file or the file has no content
	{	
		//cout << "readFile wrong!" << endl;
		return flag;
	}
	
	//convert the content of vector to lower leters
	convertToLower();
	
	//delete the space flag &nbsp and &nbsp; from the vector
	string strDel = "&nbsp;";
	removeString( strDel );
	strDel = "&nbsp";
	removeString( strDel );
	
	string strXmlHead = "<?xml version=\"1.0\" encoding=\"gb2312\"?>";
	if ( (*vFileContent.begin()).find( "<?xml" ) != string::npos )		//if the vector has xml declearation then 
	{													//	write the vector to the file
		writeVectorToSrcFile();
	}
	else												//else add the declearation to the file
	{													//	and write the vector tho the file
		addHeadToFile( strXmlHead );
	}
	
	vFileContent.clear();								//clear the vector
	
	
	flag = tidyFile( srcFilePath );
	if( flag >= 0 )
		return true;
	else
		return false;
}


