#include "memMan.hpp"

int main(int argc, char* argv[]) {
  vector<uint16_t> randNContnr;  // random number container

  int pid = getpid();
  cout << "Running benckmark for memory management algorithms..." << endl;
  cout << totalInstruction << " instructions in total" << endl;
  cout << totalPage << " pages in total" << endl;
  cout << memPageLimit << " pages limited" << endl;
  cout << "PID:" << pid << endl;

  /* Prepare a random number generator */  
  mt19937 rng;
  rng.seed(pid);  // use pid as seed;
  uniform_int_distribution<uint32_t> uintDist(0, totalPage - 1);

  cout << "random sequence:" << endl;
  for (int it = 0; it < totalInstruction; it++) {
    uint16_t rdNum = uintDist(rng);
    cout << rdNum << " ";
    randNContnr.push_back(rdNum);
  }
  cout << endl;

  assert(randNContnr.size() == totalInstruction);

  uint32_t diseffect;
/*
  diseffect =  FIFOMan(randNContnr);
  cout << "\nFIFO hit rate = " << 1 - float(diseffect)/totalInstruction << endl;
  diseffect = LRUMan(randNContnr);
  cout << "\nLRU hit rate = " << 1 - float(diseffect)/totalInstruction << endl;
  diseffect =  NURMan(randNContnr);
  cout << "\nFIFO hit rate = " << 1 - float(diseffect)/totalInstruction << endl;
*/
  return 0;
}

uint32_t FIFOMan(vector<uint16_t> randNContnr) {
  /* Prepare data structures */
  uint32_t diseffect = 0;  // mismatch
  set<uint16_t> memLookUp;  // use set for convient find and erase
  queue<uint16_t> FIFOContnr;   // page continer

  for (auto it = randNContnr.begin(); it != randNContnr.end(); it++) {
    cout << "\nrequesting " << *it << endl;
    auto reqPageInMem = memLookUp.find(*it);  // find requested page in memory

    /* if not found */
    if (reqPageInMem == memLookUp.end()) {  // returns .end() if not found
      diseffect++;

      /* if exceeds the memory limit */
      if (FIFOContnr.size() == memPageLimit) {
        uint16_t tbRemoved = FIFOContnr.front();
        FIFOContnr.pop();
        memLookUp.erase(tbRemoved);
        cout << tbRemoved << " removed" << endl;
      }
    
      /* insert new mem page */
      FIFOContnr.push(*it);
      memLookUp.insert(*it);
      cout << "put into mem " << *it << endl;
    }
  }

  return diseffect;
}

uint32_t LRUMan(vector<uint16_t> randNContnr) {
  /* Prepare data structures */
  uint32_t diseffect = 0;  // mismatch
  set<uint16_t> memLookUp;  // use set for convient find and erase
  priority_queue< pair<uint16_t, int>, vector< pair<uint16_t, int> >,
                  LRUTimeComp > LRUContnr, tmpContnr;   // page continer

  for (auto it = randNContnr.begin(); it != randNContnr.end(); it++) {
    cout << "\nrequesting " << *it << endl;
    auto reqPageInMem = memLookUp.find(*it);  // find requested page in memory

    /* if not found */
    if (reqPageInMem == memLookUp.end()) {  // returns .end() if not found
      diseffect++;

      /* if exceeds the memory limit */
      if (LRUContnr.size() == memPageLimit) {
        uint16_t tbRemoved = get<0>(LRUContnr.top());
        LRUContnr.pop();
        memLookUp.erase(tbRemoved);
        cout << tbRemoved << " removed" << endl;
      }
    
      /* insert new mem page */
      LRUContnr.push(make_pair(*it, 0));
      memLookUp.insert(*it);
      cout << "put into mem " << *it << endl;

    }
    /* replace with a 0 lapse */
    else {
      while (!LRUContnr.empty()) {
        if (get<0>(LRUContnr.top()) == *reqPageInMem) {
          cout << *reqPageInMem << "found" << endl;
          tmpContnr.push(make_pair(*reqPageInMem, 0));
          LRUContnr.pop();
        }
        else {
          tmpContnr.push(LRUContnr.top());
          LRUContnr.pop();
        }
      }
      LRUContnr = tmpContnr;
      tmpContnr = priority_queue< pair<uint16_t, int>, vector< pair<uint16_t, int> >,
                  LRUTimeComp >(); 
    }

    /* add lapse after each instrcution */    
    while (!LRUContnr.empty()) {  // decide dynamically, cannot use .size()
      tmpContnr.push(make_pair(get<0>(LRUContnr.top()), \
                               get<1>(LRUContnr.top())+1));
      cout << get<0>(LRUContnr.top()) << endl;
      LRUContnr.pop();
    }
    LRUContnr = tmpContnr;
    tmpContnr = priority_queue< pair<uint16_t, int>, vector< pair<uint16_t, int> >,
                LRUTimeComp >();
    cout << "top is <" << get<0>(LRUContnr.top()) << ", "
                       << get<1>(LRUContnr.top()) << ">" << endl;
  }
  return diseffect;
}


uint32_t NURMan(vector<uint16_t> randNContnr) {
  /* Prepare data structures */
  uint32_t diseffect = 0;  // mismatch
  vector<bool> memPageFlag;
  vector<uint16_t> memPage;

  for (auto it = randNContnr.begin(); it != randNContnr.end(); it++) {
    /* reset flags */
    if ((it - randNContnr.begin())%cycPeriod == 0) {
      cout << "cleaning flag" << endl;
      for (auto itt = memPageFlag.begin(); itt != memPageFlag.end(); itt++) {
        *itt = false;
      }
    }

    cout << "\nrequesting " << *it << endl;
    auto reqPageInMem = find(memPage.begin(), memPage.end(), *it);  // find requested page in memory
    /* if found */
    if (reqPageInMem != memPage.end()) {
      int reqPos = reqPageInMem - memPage.begin();
      cout << "found at positon " << reqPos << endl;
      memPageFlag[reqPos] = true;
    }
    else {
`     diseffect++;
      /* if within memory limit */
      if (memPage.size() < memPageLimit) {
        cout << "added" << endl;
        memPage.push_back(*it);
        memPageFlag.push_back(true); 
      }
      else {
        uint16_t itt = 0;  // serve as flag as well
        /* looking for unused page */
        for (itt = 0; itt < memPageFlag.size(); itt++) {
          if (memPageFlag[itt] == false) {
            cout << memPage[itt] << " removed" << endl;
            memPage[itt] = *it;
            memPageFlag[itt] = true;
            itt = 0;  // set flag
            break;
          }
        }
        /* all in use */
        if (itt > 0) {
          cout << "all in use, swap unit 0" << endl;
          cout << memPage[0] << " removed" << endl;
          memPage[0] = *it;
        }
      }
    }
  }
  return diseffect;
}
