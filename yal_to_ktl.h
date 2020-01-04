// #include <ktl.h>
#ifndef _YAL_TO_KTL
#define _YAL_TO_KTL

#include <ktl/la.h>
#include "yal.h"

template<int size>
// Ktl::Vector<size> yal_to_ktl(const Yal::Vector<size>& Vy, const Ktl::Vector<size>& Vk){
Ktl::Vector<size> yal_to_ktl(const Yal::Vector<size>& Vy){
  Ktl::Vector<size> Vk;
  for(int i=0; i<size; i++) Vk[i] = Vy[i];
  return Vk;
}

#endif
