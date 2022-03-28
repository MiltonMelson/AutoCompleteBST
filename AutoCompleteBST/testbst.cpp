// /**
//  * Testing BST - Binary Search Tree functions
//  *
//  * This file has series of tests for BST
//  * Each test is independent and uses assert statements
//  * Test functions are of the form
//  *
//  *      test_netidXX()
//  *
//  * where netid is UW netid and XX is the test number starting from 01
//  *
//  * Test functions can only use the public functions from BST
//  * testBSTAll() is called from main in main.cpp
//  * testBSTAll calls all other functions
//  * @author Multiple
//  * @date ongoing
//  */

#include "bstmap.h"
#include <cassert>
#include <sstream>

using namespace std;

// global value for testing
// NOLINTNEXTLINE
stringstream globalSS;

// need to reset SS before calling this
void printer(const BSTMap::value_type &p) {
  globalSS << "[" << p.first << "=" << p.second << "]";
}

// // Testing == and []
void test01() {
  cout << "Starting test01" << endl;
  cout << "* Testing ==, !=, [] and copy constructor" << endl;
  BSTMap b1;
  auto val = b1["hello"];
  assert(val == 0);
  b1["hello"] = 5;
  val = b1["hello"];
  assert(val == 5);
  b1["world"] = 42;

  BSTMap b2;
  assert(b1 != b2);
  b2["hello"] = 5;
  b2["world"] = 42;
  assert(b1 == b2);

  BSTMap b3(b2);
  assert(b1 == b3);
  cout << "Ending tes01" << endl;
}

// Testing traversal
void test02() {
  cout << "Starting test02" << endl;
  cout << "* Testing traversal" << endl;
  BSTMap b;
  b["x"] = 10;
  b["f"] = 5;
  b["b"] = 3;
  b["e"] = 4;
  b["z"] = 50;
  cout << b;

  globalSS.str("");
  b.inorder(printer);
  string order = globalSS.str();
  assert(order == "[b=3][e=4][f=5][x=10][z=50]");

  globalSS.str("");
  b.preorder(printer);
  order = globalSS.str();
  assert(order == "[x=10][f=5][b=3][e=4][z=50]");

  globalSS.str("");
  b.postorder(printer);
  order = globalSS.str();
  assert(order == "[e=4][b=3][f=5][z=50][x=10]");
  cout << "Ending test02" << endl;
}

// Testing rebalance
void test03() {
  cout << "Starting test03" << endl;
  cout << "* Testing rebalance" << endl;
  BSTMap b;
  b["1"] = 1;
  b["2"] = 2;
  b["3"] = 3;
  b["4"] = 4;
  b["5"] = 5;
  b["6"] = 6;
  assert(b.height() == 6);
  b.rebalance();
  assert(b.height() == 3);
  cout << b << endl;
  b.clear();
  cout << b << endl;
  assert(b.height() == 0);
  cout << "Ending test03" << endl;
}

void test04() {
  cout << "Starting test04" << endl;
  cout << "* Testing empty, size" << endl;
  BSTMap b;
  bool empty = true;
  assert(empty == b.empty());
  b["A"] = 1;
  b["B"] = 2;
  assert(b.size() == 2);
  string str = "A";
  assert(b.contains(str) == true);
  assert(b.count(str) == 1);
  cout << "Ending Test04" << endl;
}


// // Calling all test functions
void testBSTAll() {
  test01();
  test02();
  test03();
  // TODO(student) Add more tests
  test04();
}