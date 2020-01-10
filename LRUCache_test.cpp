#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "LRUCache.h"

bool my_assert(bool result, std::string msg) {
  if (result) {
    std::cout << "OK: " << msg << std::endl;
  } else {
    std::cout << "FAIL: " << msg << std::endl;
  }
  return result;
}

bool intkey_test() {
   com_zettix::LRUCache<int, double> cache(10L);
   int keepkey = 10;
   for (int i = 0; i < 10000; i++) {
     cache.insert(std::make_pair(i, (double) i * 1.00001));
     if (i > keepkey) {
       auto foo = cache.find(keepkey);
     }
   }
   bool result = my_assert(cache.size() == 10, "Int cache size");
   result &= my_assert(cache.count(keepkey) == 1, "Find keep key");
   result &= my_assert(cache.count(0) == 0, "Find 0 key");
   int k = 10000 - 3;
   result &= my_assert(cache.count(k) == 1, "Find 99.. key");
   return result;
}

bool stress_test() {
   com_zettix::LRUCache<std::string, int> cache(10L);
   std::vector<std::string> names;
   for (char c = 'a'; c < 'z'; c++) {
     for (char c1 = 'a'; c1 < 'z'; c1++) {
       std::stringstream ss;
       ss << c;
       ss << c1;
       names.push_back(ss.str());
    }
  }
  int ks = names.size();
  std::cout << "Names created: " << ks << std::endl;
  for (int i = 0; i < 1e6; i++) {
    std::string k = names[i % ks];
    cache.insert(std::make_pair(k, i));
  }
  std::cout << "cache:" << std::endl << cache.str() << std::endl;
  bool result = my_assert(cache.count("yy") == 1, "Find yy");
  result &= my_assert(cache.count("ya") == 0, "Find ya");
  result &= my_assert(cache.size() == 10L, "Correct size");
  return result;
}
    
bool small_test() {
   com_zettix::LRUCache<std::string, std::string> nextwords(2L);
	 nextwords.insert(std::make_pair("New York", "Mets"));
   nextwords.insert(std::make_pair("Philadelphia", "Phillies"));
   nextwords.insert(std::make_pair("Boston", "Red Sox"));
   nextwords.insert(std::make_pair("Oakland", "A's"));
   nextwords.insert(std::make_pair("Pittsburgh", "Pirates"));
   bool result = my_assert(nextwords.size() == 2, "Correct size");
   result &= my_assert(nextwords.count("New York") == 0, "Find New York");
   result &= my_assert(nextwords.count("Pittsburgh") == 1, "Find Pittsburgh");
   result &= my_assert(nextwords["Pittsburgh"].compare("Pirates") == 0, "Compare values");
   return result;
}

bool iterator_test() {
   com_zettix::LRUCache<std::string, int> nextwords(5L);
   nextwords.insert(std::make_pair("one", 1));
   nextwords.insert(std::make_pair("two", 2));
   nextwords.insert(std::make_pair("three", 3));
   nextwords.insert(std::make_pair("four", 4));
   nextwords.insert(std::make_pair("five", 5));
   nextwords.insert(std::make_pair("six", 6));
   nextwords.insert(std::make_pair("seven", 7));
   int count = 1;
   auto it = nextwords.begin();
   while (it.has_next()) {
     std::cout << ">> " << (*it).first << " : " << (*it).second << std::endl;
     it++;
     count++;
   }
   bool result = my_assert(count == 5, "correct size");
   return result; 
}

bool ref_test() {
  com_zettix::LRUCache<std::string, int> cache(4L);
  cache.insert(std::make_pair("keepme", 101));
 
  char k = 'a';
  int check = -1;
  for (int i = 0; i < 100; i++) {
    std::stringstream ss;
    ss << k;
    k++;
    if (k > 'y') k = 'a';
    cache.insert(std::make_pair(ss.str(), i));
    check = cache["keepme"];
  }
  bool result = my_assert(check == 101, "ref check");
  return result;
}
    
  
   
int main(int argc, char **argv) {
   bool result = small_test();
   result &= stress_test();
   result &= iterator_test();
   result &= ref_test();
   result &= intkey_test();
   if (result) {
     std::cout << "Success!" << std::endl;
   } else {
     std::cout << "Failures!" << std::endl;
     return 1; 
   }
   return 0; 
}
