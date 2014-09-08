#ifndef _HSA_BASE_H_
#define _HSA_BASE_H_

#include <hsa.h>
#include <hsa_ext_finalize.h>

#define DEALLOC(pointer) if ((pointer) != nullptr) { delete (pointer); (pointer) = nullptr; }
#define DEALLOC_ARR(pointer) if ((pointer) != nullptr) { delete[] (pointer); (pointer) = nullptr; }

#endif
