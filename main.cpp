/** TRACCC library, part of the ACTS project (R&D line)
 *
 * (c) 2021-2022 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#include <iostream>
#include <execution>
#include <algorithm>

#include <vector>
#include <memory_resource>
#include "CountingIterator.h"

namespace vecmem{
  // equivalent to std::pmr::vector
  template <typename T>
  using vector = std::vector<T, std::pmr::polymorphic_allocator<T>>;
}

int main(int argc, char* argv[]){    

  const int DSIZE = 64*1048576;

  // populate vectors
  vecmem::vector<float> a_initialized(DSIZE);
  vecmem::vector<float> b_initialized(DSIZE);
  for (int i = 0; i < DSIZE; i++){
      a_initialized.at(i) = rand()/(float)RAND_MAX;
      b_initialized.at(i) = rand()/(float)RAND_MAX;
  }

  // initialize const and std vectors
  const vecmem::vector<float> a = a_initialized;
  const vecmem::vector<float> b = b_initialized;
  std::vector<float> c(DSIZE);

  // get data pointers
  const float* a_p = a.data();
  const float* b_p = b.data();
  float* c_p = c.data();

  // execute 
  std::for_each_n(std::execution::par, counting_iterator(0), DSIZE, [=](unsigned int i){
    c_p[i] = a_p[i] + b_p[i];
  });

  // alternative execution call
  // std::transform(std::execution::par, a.begin(), a.end(), b.begin(), c.begin(), [](float x, float y) -> float {return x+y;});

  // print out some results to verify calculation
  std::cout << "-----------\n";
  std::cout << a.at(0) << "\n";
  std::cout << b.at(0) << "\n";
  std::cout << c.at(0) << "\n";
  return 0;
}
