/*
#include "string_utils.h"

using namespace std;

vector <char* > vszParseString( const char* _szString, char _szDilimiter) {

	int iCurrVec = 0;

	unsigned int uiIndex = 0;

	char szTmpBuf;
	char* szCurrStr = '\0';
	vector <char* > vszBuf_;


	do {
		szTmpBuf = _szString[uiIndex];
		if (szTmpBuf == _szDilimiter|| szTmpBuf == '\n') {
			vszBuf_.push_back(szCurrStr);
			iCurrVec++;
			memset(szCurrStr, 0, sizeof(&szCurrStr));
			uiIndex++;
		} else {
			szCurrStr[uiIndex] = szTmpBuf;
			uiIndex++;
		}
		//cout << "\n" << iCurrVec << "."<< szTmpBuf << "\n";
	} while (uiIndex <= strlen(_szString));
	return vszBuf_;
}

void exitWithError(std::string _strErrorMsg, int _iExitNumber) {
	cout << _strErrorMsg << "\n";
	exit(_iExitNumber);
}

char* szTruncateByDelimiter(char* _szBuf, char _szDelimiter) {
	int index = 0;

	char* szBuf = '\0';
	char* szTmpBuf = '\0';

	do {
		if (index > (int) sizeof _szBuf)
			return _szBuf;
		szTmpBuf[0] = _szBuf[index];
		if (szTmpBuf == &_szDelimiter)
			break;
		szBuf[index] = _szBuf[index];
		cout << szTmpBuf;
		index++;
	} while (szTmpBuf != &_szDelimiter);
	return szBuf;
}

bool bIsStringIP( const char* _szString) {
	int value = 0;
	bool bIsLessThan256 = false;
	vector <char* > vszBuf_;
	vszBuf_ = vszParseString(_szString, '.');

	if (vszBuf_.size() == 3) {
		for (unsigned int i = 0; i < vszBuf_.size(); i++) {
			if(sscanf("%d", vszBuf_[i], &value) < 1)
				break;
			if (value <= 256 && value >= 0)
				bIsLessThan256 = true;
			else {
				bIsLessThan256 = false;
				break;
			}
		}
		if (bIsLessThan256) {
			return true;
		} else {
			return false;
		}

	}

	return false;
}
*/
#ifdef STRING_UTILS_TEST
int main() {
	
}
#endif


