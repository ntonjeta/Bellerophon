/******************************************************************************
* @file   LoopAprxTechnique.cpp
* @author Antonio Tammaro
* @date   23 gen 2017
* @brief  Implementation for the flap aprx technique
******************************************************************************/

// Tools Headers
#include "Plugins/LoopPerforation/LoopAprxTechnique.h"
#include "Log.h"
#include "lib/csv.h"
// LLVM Headers
#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Debug.h"
// C/C++ Headers
#include <map>
#include <memory>
#include <utility>


using namespace bellerophon;

::std::vector<::std::string> LoopAprxTechnique::getGlobalValueNames() const
{
  ::std::vector<::std::string> names;
  names.push_back(this->OpId);
  return names;
}

void LoopAprxTechnique::applyApproximation(
    ::bellerophon::core::AprxGrade g, ::std::vector<uint64_t> addrs) const
{
  int *p = (int *)addrs.at(0);

  log::BellerophonLogger::verbose(
    "Address: " + ::std::to_string(addrs.at(0)) +
    " - grade(" + ::std::to_string(*p) + 
    ")");
  
  if (!(g == 0))  
    (*p) = g; 

  log::BellerophonLogger::verbose(
    "After approximation: " + ::std::to_string(*p) +
    "\n");
}

::std::vector<::std::string> LoopAprxTechnique::applyApproximation(
      ::bellerophon::core::AprxGrade g) const
{
  ::std::vector<::std::string> args;
  return args; 
}


void LoopAprxTechnique::dump(::llvm::raw_ostream &out) const {}
