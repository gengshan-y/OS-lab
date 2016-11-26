// include from local to global
#include <iostream>
#include <queue>
#include <random>
#include <set>
#include <tuple>
#include <unistd.h>  // for getpid()
#include <assert.h>

#define totalInstruction 1000
#define totalPage 10
#define memPageLimit 3

using namespace std;

uint32_t FIFOMan(vector<uint16_t> randNContnr);
uint32_t LRUMan(vector<uint16_t> randNContnr);

class LRUTimeComp {
  public:
    bool operator() (pair<uint16_t, int> left, pair<uint16_t, int> right) const {
      return get<1>(left) < get<1>(right);
    }
};
