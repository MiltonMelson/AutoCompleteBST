#include "autocomplete.h"
#include <algorithm>
#include <fstream>

using namespace std;

void testBSTAll();

void Autocomplete::readFile(const string &fileName) {
  ifstream ifs(fileName);
  if (!ifs.is_open()) {
    cout << "Unable to open file";
    return;
  }
  int total;
  ifs >> total;
  vector<BSTMap::value_type> result;
  while (!ifs.eof()) {
    BSTMap::mapped_type weight;
    BSTMap::key_type name;
    ifs >> weight;
    getline(ifs >> ws, name);
    phrases[name] = weight;
  }
  ifs.close();
  phrases.rebalance();
  cout << "rebalance done";
}

bool Autocomplete::sortByWeight(BSTMap::value_type &a, BSTMap::value_type &b) {
  return a.second > b.second;
}

vector<BSTMap::value_type>
Autocomplete::complete(const BSTMap::key_type &prefix) {
  vector<BSTMap::value_type> v;
  v = phrases.getAll(prefix);
  sort(v.begin(), v.end(), sortByWeight);
  return v;
}
