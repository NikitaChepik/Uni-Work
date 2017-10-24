#include "BucketSort.h"

#include <algorithm>
#include <cmath>
#include <thread>
#include <vector>
#include <mutex>
// added for testing.
#include <iostream>

// Determines the first digit of a number using binary search.
// Using the max int as the largest length size.
unsigned int firstDigit(unsigned int i){
  if (i >= 1000000) {
    if (i >= 10000000) {
        if (i >= 100000000) {
            if (i >= 1000000000) {
                i /= 1000000000;
            } else {
                i /= 100000000;
            }
        } else {
            i /= 10000000;
        }
    } else {
        i /= 1000000;
    }
  }

  if (i >= 1000) {
    if (i >= 10000) {
        if (i >= 100000) {
            i /= 100000;
        } else {
            i /= 10000;
        }
    } else {
        i /= 1000;
    }
  }
  if (i >= 100) i/= 100;
  if (i >= 10)  i/= 10;
  return i;
}

bool aLessB(const unsigned int& x, const unsigned int& y, unsigned int pow) {

  if (x == y) return false; // if the two numbers are the same then one is not less than the other

  unsigned int a = x;
  unsigned int b = y;

  // work out the digit we are currently comparing on. 
  if (pow == 0) {
    while (a / 10 > 0) {
            a = a / 10; 
    }   
    while (b / 10 > 0) {
            b = b / 10;
    }
  } else {
    while (a / 10 >= (unsigned int) std::round(std::pow(10,pow))) {
          a = a / 10;
    }
  while (b / 10 >= (unsigned int) std::round(std::pow(10,pow))) {
          b = b / 10;
    }
  }

  if (a == b)
    return aLessB(x,y,pow + 1);  // recurse if this digit is the same 
  else
    return a < b;
}

//Splits the initial vector up into smaller buckets.
void smallBuckets(const std::vector<unsigned int> &numbersToSort,unsigned int start,
    unsigned int end,unsigned int num,std::vector<std::vector<std::vector<unsigned int>>> &smallbuckets,std::mutex &mutex){
  
  //Initialise buckets length of 10 for each number [0-9].
  std::vector<std::vector<unsigned int>> buckets;
  for (int i = 0; i < 10; i++) {
    buckets.emplace_back(std::vector<unsigned int>()); 
  }
  //Put the number in the corresponding small vector.
  for(unsigned int i = start ; i < end ; ++i){
    buckets[firstDigit(numbersToSort.at(i))].emplace_back(numbersToSort.at(i));
  }
  // lock small buckets and push the buckets to it.
  std::lock_guard<std::mutex> lock(mutex);
  smallbuckets.emplace_back(buckets);
}


// Puts all the smaller buckets into the large corresponding bucket and then sorts. Takes in a start and end as each thread will deal
// with multiple index's.
void largeBuckets(std::vector<std::vector<unsigned int>> &largebuckets,std::vector<std::vector<std::vector<unsigned int>>> &smallbuckets,
  unsigned int start , unsigned int end){
  for(unsigned int i = start ; i < end ; ++i){
    for(const auto &vec : smallbuckets){
      for(const unsigned int &num : vec[i] ){
        largebuckets[i].emplace_back(num);
      }
    }
    std::sort(largebuckets[i].begin(),largebuckets[i].end(), [](const unsigned int& x, const unsigned int& y){
      return aLessB(x,y,0);
    } );
  
  }
}

void BucketSort::sort(unsigned int numCores) {
  
  std::vector<std::thread> threads;
  
  std::vector<std::vector<std::vector<unsigned int>>> smallbuckets;
  std::mutex mutex;

  //initialise largeBuckets with a bucket for each digit between 0-9
  std::vector<std::vector<unsigned int>> largebuckets;
  for (int i = 0; i < 10; i++) {
    largebuckets.emplace_back(std::vector<unsigned int>()); 
  }

  unsigned int loopLength = numbersToSort.size() / numCores;
  unsigned int start = 0; 
  unsigned int end = 0;
  //Partition the numbers equally between threads.
  for(unsigned int i = 0; i < numCores; ++i){
    if(i + 1 == numCores){
      start = end;
      end = numbersToSort.size();
    } else {
      start = end;
      end += loopLength;
    }
    threads.emplace_back(std::thread(smallBuckets,std::ref(numbersToSort),start,end,std::ref(i),std::ref(smallbuckets),std::ref(mutex)));
  }
  
  //Close threads used in smallBuckets function.
  for(auto &thread : threads){
    thread.join();
  }
  threads.clear();
  
  /*Determines how many large buckets each core
  / will have to deal with assuming that the total
  / avail cores will not be greater than 10. 
  */
  int eachCore = 0;
  int extraCore =0;
  int totBuckets = 10;
  if(totBuckets%numCores == 0){
    eachCore = totBuckets/numCores;
  } else{
    eachCore = totBuckets/numCores;
    extraCore = totBuckets%numCores;
  }
  start = 0;
  end = 0;
  extraCore--;
  for(unsigned int i = 0;i < numCores ; ++i){
    start = end;
    end = start + eachCore;
    if(i+1 == numCores){
      end = totBuckets;
    }
    if(extraCore > 0 && start >=4){
      end++;
      extraCore--;
    }
    threads.emplace_back(std::thread(largeBuckets,std::ref(largebuckets),std::ref(smallbuckets),start,end));
  }

  //Close threads used in smallBuckets function.
  for(auto &thread : threads){
    thread.join();
  }
  
  //clear the original vector and then merge the largbuckets back into the original vector.
  numbersToSort.clear();
  for(const auto &v : largebuckets){
    for(const unsigned int &num : v){
      numbersToSort.emplace_back(num);
    }
  }

}

void BucketSort::sortNormal() {
  std::sort(numbersToSort.begin(),numbersToSort.end(), [](const unsigned int& x, const unsigned int& y){
        return aLessB(x,y,0);
} );

}
