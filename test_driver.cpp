/**
 * @file    test_driver.cpp
 * @author  Jason Ventresca
 * @brief   Brief description goes here
 */

#include "test_driver.h"

namespace UndoArray
{

void TestDriver::simpleTest() const
{
  // create an undo array of chars of length 7
  UndoArray<char> ua(7);
  for (int i = 0; i < 7; i++) 
  {
    assert (ua.isInitialized(i) == false);
  }

  // do some operations to verify the performance
  ua.set(2,'a');
  assert (ua.isInitialized(2) == true);
  assert (ua.get(2) == 'a');
  ua.set(2,'b');
  assert (ua.isInitialized(2) == true);
  assert (ua.get(2) == 'b');
  ua.set(4,'c');
  assert (ua.isInitialized(4) == true);
  assert (ua.get(4) == 'c');

  ua.print();

  ua.undo(2);
  assert (ua.get(2) == 'a');
  assert (ua.get(4) == 'c');
  ua.undo(4);
  assert (ua.isInitialized(4) == false);  
  assert (ua.isInitialized(2) == true);  
  assert (ua.get(2) == 'a');

  ua.print();

  // note...  the UndoArray destructor is implicitly called for the
  // variable 'ua' when we leave the function and it goes out of scope
}


// Batch test will repeatedly load & process a file with UndoArray
// operations.  If the program's working memory does not grow when the
// program is run many, many times on a large test case, the data
// structure is probably free of memory leaks.
void TestDriver::batchTest(const char* filename, int num) const
{
  using namespace std;
	assert (num > 0);
	
	bool once;
	if (num == 1)
		once = true;
	else
		once = false;

	while (num > 0) 
  {
    --num;
		// open the file stream for reading
		ifstream istr(filename);
		assert (istr);
		// read the size of the array to allocate
		char c;
		int i;
		istr >> c >> i;
		assert (c == 'a');
		// here we explicitly allocate memory for the UndoArray object
		UndoArray<char> *ua = new UndoArray<char>(i);
		// read in and perform various operations on the array
		while (istr >> c) 
    {
			if (c == 's') 
      {
				istr >> i >> c;
				ua->set(i,c);	
			}
      else if (c == 'g') 
      {
				istr >> i;
				if (ua->isInitialized(i))
        {
//					char val = ua->get(i);
				}
			}
      else 
      {
				assert (c == 'u');
				istr >> i;
				if (ua->isInitialized(i))
					ua->undo(i);	
			}
		}
		
		if (once)
			ua->print();
		// Because the UndoArray memory was allocated explicitly (using new)
		// we need to explicitly deallocate the memory (using delete)
		delete ua;
	}
}

void TestDriver::testClassType()const
{
  using namespace std;
	testInt();
	testDouble();
	testFloat();
	testBool();
	cout << "Template Class Type Test passed.\n";
}

void TestDriver::testCopyConstructor() const
{
  using namespace std;
	UndoArray<char> t1(3);
	t1.set(0,'a');
	t1.set(1,'b');
	t1.set(2,'c');
	UndoArray<char> t2(t1);
	assert(t1 == t2);
	cout << "Copy Constructor Test passed.\n";
}

void TestDriver::testAssignmentOperator() const
{
  using namespace std;
	UndoArray<char> t1(3);
	t1.set(0,'a');
	t1.set(1,'b');
	t1.set(2,'c');
	UndoArray<char> t2 = t1;
	assert(t1 == 2);
	cout << "Assignment Operator Test passed.\n";
}

void TestDriver::testDestructor() const
{
	UndoArray<char> *t1 = new UndoArray<char> (3);
	t1->set(0,'a');
	t1->set(1,'b');
	t1->set(2,'c');
	delete t1;
}

void TestDriver::testInt() const
{
	UndoArray<int> t1(5);
	t1.set(0,1);
	t1.set(0,2);
	t1.set(0,2);
	t1.set(0,4);
	t1.set(1,5);
	t1.set(4,6);
	t1.set(4,7);
	t1.undo(0);
	t1.undo(1);
	assert(t1.get(4) == 7);
	assert(!t1.isInitialized(2));
	assert(t1.get(0) == 2);
}

void TestDriver::testDouble() const
{
	UndoArray<double> t2(5);
	t2.set(0,1.3);
	t2.set(0,2.43);
	t2.set(0,3.1415);
	t2.set(0,4.0);
	t2.set(1,5.667);
	t2.set(4,3.1415);
	t2.set(4,7.2);
	t2.undo(0);
	t2.undo(1);
	assert(t2.get(4) == 7.2);
	assert(!t2.isInitialized(2));
	assert(t2.get(0) == 3.1415);
}

void TestDriver::testFloat() const
{
	UndoArray<float> t3(5);
	t3.set(0,1.3);
	t3.set(0,2.43);
	t3.set(0,2);
	t3.set(0,4.0);
	t3.set(1,5.667);
	t3.set(4,3.1415);
	t3.set(4,3);
	t3.undo(0);
	t3.undo(1);
	assert(t3.get(0) == 2);
	assert(!t3.isInitialized(1));
	assert(t3.get(4) == 3);
}

void TestDriver::testBool() const
{
	UndoArray<bool> t4(3);
	t4.set(0,true);
	t4.set(0,true);
	t4.set(0,false);
	t4.set(0,true);
	t4.set(1,false);
	t4.set(2,false);
	t4.set(2,true);
	t4.undo(0);
	t4.undo(1);
	assert(t4.get(2) == true);
	assert(!t4.isInitialized(1));
	assert(t4.get(0) == false);
}

} // namespace UndoArray

