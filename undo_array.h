/**
 * @file    undo_array.h
 * @brief   This file contains the implementation of a custom data structure, 
 *          called UndoArray. It works like an ordinary fixed-array that 
 *          stores values of a specified template type, but also has the 
 *          ability to undo calls to the set() function. In a sense, UndoArray
 *          stores the complete history of all the previous values at each 
 *          element in the array. The provided main.cpp includes several 
 *          functions to test each of the public member functions of UndoArray.
 * @usage   UndoArray<int> example(5); // create an empty array of size 5, to 
 *          hold type int
 */

#include <iostream>
#include "assert.h"

#define DEBUG_MODE

template <class T>
class UndoArray 
{

public: 
  /**
   * Constructor
   * @brief   The array's "skeleton" is allocated, which is just an array 
   *          of pointers to the (to be created) history arrays
   * @param   (unsigned int) desired size of array
   */
  UndoArray(const unsigned int& inputSize) :
    m_size(inputSize)
  {
    m_historySizes = new unsigned int [m_size];
    m_values = new T* [m_size];
    // initialize each element in m_values to a NULL pointer
    for (unsigned int i = 0; i < m_size; ++i)
    {
      m_historySizes[i] = 0;
      m_values[i] = NULL;
    }
  }
  
  /**
   * Destructor
   * @brief   Frees all heap memory
   */
  ~UndoArray()
  {
    // free all heap memory that was allocated for the object
    delete m_historySizes;
    for (unsigned int i = 0; i < m_size; ++i)
      delete m_values[i];
    
    delete m_values;
  }
  
  /**
   * Copy Constructor
   * @brief   Frees all heap memory
   */
  UndoArray(const UndoArray<T>& copy) :
    m_size(copy.m_size),
    m_historySizes(new unsigned int [m_size]),
    m_values(new T* [m_size])
  {
    
    // deep copy the array contents, including all history
    for (unsigned int i = 0; i < copy.m_size; ++i)
    {
      m_historySizes[i] = copy.m_historySizes[i];
      m_values[i] = new T [ m_historySizes[i] ];
      
      // deep copy each historical element
      for (unsigned int j = 0; j < m_historySizes[i]; ++j)
        m_values[i][j] = copy.m_values[i][j];
    }
  }
  
  /**
   * Assignment Operator
   * @brief   Deep copy the entire rhs object to this object
   * @param   (const UndoArray<T>&) rhs
   * @return  (UndoArray<T>&)
   * @note    make this a non-member function if support for LHS 
   *          implicit conversions are required
   */
  UndoArray<T>& operator=(const UndoArray<T>& rhs)
  {
    // first, free any heap memory to avoid leaks
    if (m_historySizes != NULL)
      delete m_historySizes;
    for (unsigned int i = 0; i < m_size; ++i)
    {
      if (m_values[i] != NULL)
        delete m_values[i];
    }

    // resize and allocate sufficient memory
    m_size = rhs.m_size;
    m_historySizes = new unsigned int [rhs.m_size];
    m_values = new T* [m_size];

    // perform deep copy
    for (unsigned int i = 0; i < rhs.m_size; ++i)
    {
      m_historySizes[i] = rhs.m_historySizes[i];
      m_values[i]= new T [ m_historySizes[i] ];
    
      for (unsigned int j = 0; j < m_historySizes[i]; ++j)
        m_values[i][j] = rhs.m_values[i][j];
    }     
    
    return *this;
  }
 
  /**
   * get
   * @brief   get the most recent history value for the given index
   * @param   (unsigned int) index
   * @return  (T) most recent value stored at the given index
   */
  T get(const unsigned int& index) const
  {
    // demonstration of pointer arithmetic
    return *(*(m_values + index) + (*(m_historySizes + index) - 1));
  }

  /**
   * set
   * @brief   set the value of the array at the given index
   * @param   (unsigned int) index
   * @param   (T&) newValue
   * @return  (void)
   */
  void set(const unsigned int& index, const T& newValue)
  {
    // create a temporary pointer to store the history of m_values[index]
    T* tempHistory = new T [ m_historySizes[index] ];
    
    // copy the history of m_values[index] to *tempHistory
    const unsigned int& historyLength = m_historySizes[index];
    for (unsigned int i = 0; i < historyLength; ++i)
      tempHistory[i] = m_values[index][i];
    
    // delete and reallocate the memory from the old m_values[index] pointer
    delete m_values[index];
    m_values[index] = new T [ m_historySizes[index] ];
    
    // copy the history in *tempHistory back to m_values[index]
    for (unsigned int i = 0; i < m_historySizes[index]; ++i)
      m_values[index][i] = tempHistory[i];
    
    delete tempHistory;
    
    ++m_historySizes[index];
    
    // add the new value onto the history for m_values[index]
    m_values[index][ m_historySizes[index] - 1 ] = newValue;
  }

  /**
   * undo
   * @brief   revert to the most recent value for the given index
   * @param   (unsigned int) index
   * @return  (void)
   */
  void undo(const unsigned int& index)
  {
    assert(this->initialized(index));
    const unsigned int oldSize = m_historySizes[index];
    const unsigned int newSize = oldSize - 1;
   
    // create a temporary pointer to store the history of m_values[index]
    T* tempHistory = new T [oldSize];
    
    // copy the history of m_values[index] to *tempHistory
    for (unsigned int i = 0; i < oldSize; ++i)
      tempHistory[i] = m_values[index][i];
    
    // delete and reallocate the memory from the old m_values[index] pointer
    delete m_values[index];
    m_values[index] = new T [newSize];
    
    // copy the history in *tempHistory back to m_values[index]
    for (unsigned int i = 0; i < newSize; ++i)
      m_values[index][i] = tempHistory[i];
    
    delete tempHistory;
    
    --m_historySizes[index];
    if (m_historySizes[index] == 0)
    {
      // set m_values[index] to the NULL pointer
      delete m_values[index];
      m_values[index] = NULL;
    }
  }

  /**
   * isInitialized
   * @brief   is the value at the given index initialized? (i.e. there is at 
   *          least one history element)
   * @param   (unsigned int) index
   * @return  (bool) : true if array index is initialized, else false
   */
  bool initialized(unsigned int index)
  {
    return m_values[index] != NULL;
  }
  
  /**
   * print
   * @brief   print the contents of the array, including the history for each 
   *          index
   * @param   (void)
   * @return  (void)
   */
  void print()
  {
    unsigned int max_history = 0;
#ifdef DEBUG_MODE
    std::cout << "m_size:          " << m_size << std::endl;
    std::cout << "m_historySizes:  ";
#endif
    
    // determine the maximum history of each position in m_values
    for (unsigned int i = 0; i < m_size; i++)
    {
      std::cout << m_historySizes[i] << "  ";
      if (m_historySizes[i] > max_history)
        max_history = m_historySizes[i];
    }
    std::cout << std::endl << "m_values:  ";
    
    // print a '/' if the position is empty, and a '.' if it contains history
    for (unsigned int i = 0; i < m_size; i++)
    {
      if (m_historySizes[i] == 0)
        std::cout << "/  ";
      else
        std::cout << ".  ";
    }
    std::cout << std::endl << "         ";
   
    // print the history for each position
    for (unsigned int i = 0; i < max_history; ++i)
    {
      if (i > 0) std::cout << "         ";
      
      for (unsigned int j = 0; j < m_size; ++j)
      {
        if (m_historySizes[j] > i)
          std::cout << m_values[j][i] << "  ";
        else
          std::cout << "   ";
      }
      std::cout << std::endl;
    }
  } 
  
  /**
   * isEqualTo
   * @brief   Checks for equality between *this and rhs by performing deep
   *          comparison, looking at each history value for each corresponding
   *          index in the arrays
   * @param   (const UndoArray&) rhs
   * @return  (bool) : true if rhs contains identical elements and history as
   *          *this, else false
   */
  bool isEqualTo(const UndoArray& rhs)
  {
    if (m_size != rhs.m_size)
      return false;
    
    // perform deep comparison
    for (unsigned int i = 0; i < m_size; ++i)
    {
      if (m_historySizes[i] != rhs.m_historySizes[i])
        return false;

      const unsigned int count = m_historySizes[i];
      for (unsigned int j = 0; j < count; ++j)
      {
        if (m_values[i][j] != rhs.m_values[i][j])
          return false;
      }
    }
    return true;
  }

private:
  unsigned int m_size;
  unsigned int* m_historySizes;
  T** m_values;

  /**
   * Default Constructor
   * @brief: disallow default constructor
   */
  UndoArray();

};

