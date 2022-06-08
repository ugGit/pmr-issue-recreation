# nvc++: Polymorphic Memory Allocator and Stdpar

Clone the repo:

```
git clone git@github.com:ugGit/pmr-issue-recreation.git
```

## Requirements
The issue has been encountered in an environment using the following modules, which must be available during the compilation:

* gcc/11.2.0
* nvhpc/22.5 
* cuda/11.7

## Information about the setup
The code has been tested on an Nvidia A6000 and a GeForce RTX 2080.

This project contains the minimal required modules and code to recreate the issue encountered.

## Polymorphic Memory Allocators
This issue involves polymorphic memory allocators, thus this section quotes the relevant parts from the docs of
`std::pmr::polymorphic_allocator` (https://en.cppreference.com/w/cpp/memory/polymorphic_allocator):

> The class template std::pmr::polymorphic_allocator is an Allocator which exhibits different allocation behavior depending upon the std::pmr::memory_resource from which it is constructed. Since memory_resource uses runtime polymorphism to manage allocations, different container instances with polymorphic_allocator as their static allocator type are interoperable, but can behave as if they had different allocator types. 

## Issue description
There is an incompatibility between data containers (e.g. `vector`) that use polymorphic memory allocators, since accessing data from an std par loop results in a `cudaErrorIllegalAddress`. 
Such as behavior intuitively is expected when data should be appended to a data container using a polymorphic memory allocator, but comes surprisingly as well in case of read-only access and when accessing the underlying data of the container directly.
The code in this repository illustrates this through a vector addition example using a random data access pattern.


## Execute the code
```
nvc++ -stdpar=gpu -o main main.cpp && ./main
```

Resulting output:
terminate called after throwing an instance of 'thrust::system::system_error'
  what():  for_each: failed to synchronize: cudaErrorIllegalAddress: an illegal memory access was encountered
Aborted (core dumped)

## Observations
The following list resumes the changes made that allow to run the program successful:
* Use `std::vector` for the input vectors `a` and `b`. 
* (not on GPU) Compile for multicore CPU `-stdpar=multicore`.
* (not on GPU) Compile without stdpar `-nostdpar`.
* (not on GPU) Compile with g++ and TBB backend.
