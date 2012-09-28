#ifndef TEST_DRIVER_H
#define TEST_DRIVER_H
/**
 * @file    test_driver.h
 * @author  Jason Ventresca
 * @brief   Unit testing for the components of the UndoArray class
 * */

#include <iostream>
#include "undo_array.h"
#include <fstream>
#include <stdlib.h>
#include <assert.h>

namespace UndoArray
{

class TestDriver
{
public:
  /**
   * Orthodox Canonical Form (OCF) member functions
   */

  /**
   * Default Constructor
   */
  TestDriver()
  {

  }

  /**
   * Copy Constructor
   */
  TestDriver(const TestDriver& copy)
  {

  }

  /**
   * Destructor
   */
  ~TestDriver()
  {

  }

  /**
   * Copy Assignment Operator
   */
  TestDriver& operator=(const TestDriver& rhs)
  {
    /**
     * perform deep copy -- consider moving to implementation file to prevent
     * the compiler from inlining this function if it gets heavy
     */

    return *this;
  }

  void simpleTest() const;
  void batchTest(const char* filename, int num) const;
  void testClassType() const;
  void testCopyConstructor() const;
  void testAssignmentOperator() const;
  void testDestructor() const;
  void testInt() const;
  void testDouble() const;
  void testFloat() const;
  void testBool() const;

private:

}; // class TestDriver

} // namespace UndoArray

#endif // include guard TEST_DRIVER_H

