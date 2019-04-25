#define _STRING_UTILS_H_
#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_
void exitWithError(std::string _strErrorMsg, int _iExitNumber);

bool bIsStringIP(const char * _szString);

char* szTruncateByDelimiter(const char* _szBuf, char _szDilimiter);

std::vector <char* > vszParseString(const char* _szString, char _szDilimiter);

//Declare some more types
/*
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;
typedef unsigned long long qword;

typedef char* cstring;

typedef short BYTE;
typedef long WORD;
typedef long long DWORD;

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;

typedef short int int16;
typedef int int32;


//Done with types
*/
#endif /* _STRING_UTILS_H_*/

