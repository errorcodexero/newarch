#include "data_logger.h"



#ifdef DATALOGGER_TEST     // THIS NEEDS TO BE UPDATED
void test_datalogger()
{
	DataloggerDestination target;
	Datalogger test(target);
	int columnindex = createColumn("name");
	assert(0 == columnindex);
	int columnindex1 = createColumn("name");
	assert(1 == columnindex1);
	startLoop();
	logData(0, 20);

	endLoop();



}
int main() 
{
	test_datalogger();
	return 0;
}
#endif 

