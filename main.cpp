/**
 * @file    main.cpp
 * @brief   This file contains the main() function which performs unit tests
 *          on the UndoArray type. 
 * @usage   Supply 2 arguments on the command line: the file path for the text
 *          file (e.g. large.txt) and the number of times to repeat the 
 *          parsing process for this file.
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <assert.h>
#include "undo_array.h"
#include "test_driver.h"

// these functions test that all of the public member functions of UndoArray are working properly
int main(int argc, char* argv[])
{
  using namespace UndoArray;
  using namespace std;

	TestDriver testDriver;
  testDriver.simpleTest();
	cout << "Simple test passed." << endl;
	
	testDriver.testClassType();
  cout << "1 passed." << endl;
	testDriver.testCopyConstructor();
  cout << "2 passed." << endl;
	testDriver.testAssignmentOperator();
  cout << "3 passed." << endl;
	testDriver.testDestructor();
  cout << "OCF Method tests passed." << endl;

	if (argc > 1) 
  {
		assert (argc == 3);
		testDriver.batchTest(argv[1],atoi(argv[2]));
		cout << "Batch test completed." << endl;
	}
	
	return 0;
}

