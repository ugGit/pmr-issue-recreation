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

  // Initialize vectors
  vecmem::vector<float> a(DSIZE);
  vecmem::vector<float> b(DSIZE);
  std::vector<float> c(DSIZE);
  for (int i = 0; i < DSIZE; i++){
      a.at(i) = rand()/(float)RAND_MAX;
      b.at(i) = rand()/(float)RAND_MAX;
  }

  // execute 
  std::transform(std::execution::par, a.begin(), a.end(), b.begin(), c.begin(), [](float x, float y) -> float {return x+y;});

  // print out some results to verify calculation
  std::cout << "-----------\n";
  std::cout << a.at(0) << "\n";
  std::cout << b.at(0) << "\n";
  std::cout << c.at(0) << "\n";
  return 0;
}
