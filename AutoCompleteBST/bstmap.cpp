// Yusuf Pisan pisan@uw.edu
// 17 Jan 2021

// BST class
// Creates a BST to store values
// Uses Node which holds the data

#include "bstmap.h"
#include <cassert>
#include <cmath>
#include <stack>

using namespace std;

// copy constructor
BSTMap::BSTMap(const BSTMap &bst) { root = copyHelper(bst.root); }

// given an array of length n
// create a tree to have all items in that array
// with the minimum height (uses same helper as rebalance)
BSTMap::BSTMap(const vector<BSTMap::value_type> &v) {
  for (const auto &i : v) {
    (*this)[i.first] = i.second;
  }
}

// destructor
BSTMap::~BSTMap() { clear(); }

// delete all nodes in tree
void BSTMap::clear() { deleteTree(&root); }

// true if no nodes in BST
bool BSTMap::empty() const { return root == nullptr; }

// Number of nodes in BST
int BSTMap::size() const { return numOfNodes; }

// 0 if empty, 1 if only root, otherwise
// height of root is max height of subtrees + 1
int BSTMap::height() const { return getHeight(root); }

// true if item is in BST
bool BSTMap::contains(const BSTMap::key_type &key) const {
  auto *temp = root;
  while (temp != nullptr && temp->data.first != key) {
    temp = key < temp->data.first ? temp->left : temp->right;
  }
  return temp != nullptr;
}

// same as contains, but returns 1 or 0
// compatibility with std::map
size_t BSTMap::count(const string &k) const {
  auto *temp = root;
  while (temp != nullptr && temp->data.first != k) {
    temp = k < temp->data.first ? temp->left : temp->right;
  }
  return (temp == nullptr ? 0 : 1);
}

// If k matches the key returns a reference to its value
// If k does not match any key, inserts a new element
// with that key and returns a reference to its mapped value.
BSTMap::mapped_type &BSTMap::operator[](const BSTMap::key_type &k) {
  if (root == nullptr) {
    root = new Node();
    root->data.first = k;
    numOfNodes++;
    return root->data.second;
  }
  return insertKey(root, k);
}

vector<BSTMap::value_type> BSTMap::getAll(const BSTMap::key_type &k) const {
  auto *temp = root;
  vector<BSTMap::value_type> result;
  autoCompleteSearch(temp, k, result);
  return result;
}

// height of a Node, nullptr is 0, root is 1, static, no access to 'this'
// helper function to height(), used by printVertical
int BSTMap::getHeight(const BSTMap::Node *n) {
  if (n == nullptr) {
    return 0;
  }
  return 1 + max(getHeight(n->left), getHeight(n->right));
}

// inorder traversal: left-root-right
// takes a function that takes a single parameter of type T
void BSTMap::inorder(void visit(const BSTMap::value_type &item)) const {
  auto *temp = root;
  inorderHelper(visit, temp);
}

// preorder traversal: root-left-right
void BSTMap::preorder(void visit(const BSTMap::value_type &item)) const {
  auto *temp = root;
  preorderHelper(visit, temp);
}

// postorder traversal: left-right-root
void BSTMap::postorder(void visit(const BSTMap::value_type &item)) const {
  auto *temp = root;
  postorderHelper(visit, temp);
}

// balance the BST by saving all nodes to a vector inorder
// and then recreating the BST from the vector
void BSTMap::rebalance() {
  vector<Node *> vec;
  treeToVector(vec, root);
  root = sortedVectorToBST(vec, 0, vec.size() - 1);
}

// returns true if item successfully erased - BONUS
bool BSTMap::erase(const BSTMap::key_type &k) {
  bool isSuccessful = false;
  removeNode(k, root, isSuccessful);
  return isSuccessful;
}

// trees are equal if they have the same structure
// AND the same item values at all the nodes
bool BSTMap::operator==(const BSTMap &other) const {
  if (root == nullptr && other.root == nullptr) {
    return true;
  }
  if (root == nullptr || other.root == nullptr) {
    return false;
  }
  queue<BSTMap::Node *> q1;
  queue<BSTMap::Node *> q2;
  q1.push(root);
  q2.push(other.root);

  while (!q1.empty() || !q2.empty()) {
    int size1 = q1.size();
    int size2 = q2.size();
    if (size1 != size2) {
      return false;
    }
    for (int i = 0; i < size1; i++) {
      auto *tmp1 = q1.front();
      q1.pop();
      auto *tmp2 = q2.front();
      q2.pop();
      if (tmp1->data.first != tmp2->data.first ||
          tmp1->data.second != tmp2->data.second) {
        return false;
      }
      if (((tmp1->left == nullptr) && (tmp2->left != nullptr)) ||
          ((tmp1->left != nullptr) && (tmp2->left == nullptr)) ||
          ((tmp1->right == nullptr) && (tmp2->right != nullptr)) ||
          ((tmp1->right != nullptr) && (tmp2->right == nullptr))) {
        return false;
      }
      if (tmp1->left != nullptr) {
        q1.push(tmp1->left);
      }
      if (tmp1->right != nullptr) {
        q1.push(tmp1->right);
      }
      if (tmp2->left != nullptr) {
        q2.push(tmp2->left);
      }
      if (tmp2->right != nullptr) {
        q2.push(tmp2->right);
      }
    }
  }
  return (q1.empty() && q2.empty());
}

// not == to each other
bool BSTMap::operator!=(const BSTMap &other) const { return !(*this == other); }

/////////////////////////////////////////////////// Helper Classes
///////////////////////////////////////////////////

// Copy constructor helper.
BSTMap::Node *BSTMap::copyHelper(BSTMap::Node *bstNode) {
  if (bstNode == nullptr) {
    return nullptr;
  }
  auto *newNode = new BSTMap::Node;
  newNode->data = bstNode->data;
  newNode->left = copyHelper(bstNode->left);
  newNode->right = copyHelper(bstNode->right);
  return newNode;
}

BSTMap::Node *BSTMap::sortedVectorToBST(vector<BSTMap::Node *> &v, int start,
                                        int end) {
  if (start > end) {
    return nullptr;
  }
  int mid = start + (end - start) / 2;
  numOfNodes++;
  auto *newRoot = v[mid];
  newRoot->left = sortedVectorToBST(v, start, mid - 1);
  newRoot->right = sortedVectorToBST(v, mid + 1, end);
  return newRoot;
}

void BSTMap::inorderHelper(void visit(const BSTMap::value_type &item),
                           BSTMap::Node *curr) const {
  if (curr != nullptr) {
    inorderHelper(visit, curr->left);
    auto theItem = curr->data;
    visit(theItem);
    inorderHelper(visit, curr->right);
  }
}

void BSTMap::preorderHelper(void visit(const BSTMap::value_type &item),
                            BSTMap::Node *curr) const {
  if (curr != nullptr) {
    auto theItem = curr->data;
    visit(theItem);
    preorderHelper(visit, curr->left);
    preorderHelper(visit, curr->right);
  }
}

void BSTMap::postorderHelper(void visit(const BSTMap::value_type &item),
                             BSTMap::Node *curr) const {
  if (curr != nullptr) {
    postorderHelper(visit, curr->left);
    postorderHelper(visit, curr->right);
    auto theItem = curr->data;
    visit(theItem);
  }
}

void BSTMap::autoCompleteSearch(BSTMap::Node *source,
                                const BSTMap::key_type &prefix,
                                vector<BSTMap::value_type> &vec) const {
  if (source != nullptr) {
    autoCompleteSearch(source->left, prefix, vec);
    if (source->data.first.substr(0, prefix.length()) == prefix) {
      auto tempData = source->data;
      vec.push_back(tempData);
    }
    autoCompleteSearch(source->right, prefix, vec);
  }
}

BSTMap::Node *BSTMap::removeNode(const BSTMap::key_type &k,
                                 BSTMap::Node *subTree, bool &isSuccessful) {
  if (subTree == nullptr) {
    isSuccessful = false;
    return subTree;
  }
  if (k < subTree->data.first) {
    subTree->left = removeNode(k, subTree->left, isSuccessful);
  } else if (k > subTree->data.first) {
    subTree->right = removeNode(k, subTree->right, isSuccessful);
  } else {
    if (subTree->left == nullptr && root->right == nullptr) {
      delete subTree;
      subTree = nullptr;
      isSuccessful = true;
      numOfNodes--;
      return subTree;
    }
    if (subTree->left == nullptr) {
      auto *temp = subTree->right;
      delete subTree;
      subTree = nullptr;
      isSuccessful = true;
      numOfNodes--;
      return temp;
    }
    if (subTree->right == nullptr) {
      auto *temp = subTree->left;
      delete subTree;
      subTree = nullptr;
      isSuccessful = true;
      numOfNodes--;
      return temp;
    }
    auto *temp = minValueNode(subTree->right);
    subTree->data = temp->data;
    subTree->right = removeNode(k, subTree->right, isSuccessful);
  }
  numOfNodes--;
  return subTree;
}

BSTMap::Node *BSTMap::minValueNode(BSTMap::Node *node) {
  auto *curr = node;
  while (curr != nullptr && curr->left != nullptr) {
    curr = curr->left;
  }
  return curr;
}

BSTMap::mapped_type &BSTMap::insertKey(BSTMap::Node *curr,
                                       const BSTMap::key_type &k) {
  if (curr->data.first > k) {
    if (curr->left == nullptr) {
      curr->left = new Node();
      curr = curr->left;
      curr->data.first = k;
      numOfNodes++;
      return curr->data.second;
    }
    return insertKey(curr->left, k);
  }
  if (curr->data.first < k) {
    if (curr->right == nullptr) {
      curr->right = new Node();
      curr = curr->right;
      curr->data.first = k;
      numOfNodes++;
      return curr->data.second;
    }
    return insertKey(curr->right, k);
  }
  return curr->data.second;
}

// delete all nodes in tree
void BSTMap::destroyTree(BSTMap::Node *toDel) {
  if (toDel == nullptr) {
    return;
  }
  /* first delete both subtrees */
  destroyTree(toDel->left);
  destroyTree(toDel->right);
  delete toDel;
}

void BSTMap::deleteTree(BSTMap::Node **nodeRef) {
  destroyTree(*nodeRef);
  *nodeRef = nullptr;
}

void BSTMap::treeToVector(vector<Node *> &vec, Node *currRoot) {
  if (currRoot == nullptr) {
    return;
  }
  treeToVector(vec, currRoot->left);
  vec.push_back(currRoot);
  treeToVector(vec, currRoot->right);
}