/******************************************************************************
* @file   FlapAprxTechnique.cpp
* @author Antonio Tammaro
* @date   23 gen 2017
* @brief  Implementation for the flap aprx technique
******************************************************************************/

// Tools Headers
#include "Plugins/FLAP/FlapAprxTechnique.h"
#include "Plugins/FLAP/fap.h"
#include "lib/csv.h"
#include "Log.h"
// LLVM Headers
#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Debug.h"
// C/C++ Headers
#include <map>
#include <memory>
#include <utility>

using namespace bellerophon;

::std::vector<::std::string> FlapAprxTechnique::getGlobalValueNames() const
{
  ::std::vector<::std::string> names;
  names.push_back(this->OpId);
  return names;
}

void FlapAprxTechnique::applyApproximation(
    ::core::AprxGrade g, ::std::vector<uint64_t> addrs) const
{
  // Take the maximum
  unsigned short expPrec = 11;
  unsigned short maxMantPrec = 52;
  if (this->OpRetTy == FLOAT) {
    expPrec = 8;
    maxMantPrec = 23;
  }

  assert(g <= maxMantPrec); // Precondition

  ::fap::FloatPrecTy *p = (::fap::FloatPrecTy *)addrs.at(0);

  log::BellerophonLogger::verbose(
    "Address: " + ::std::to_string(addrs.at(0)) +
    " - Prec("  +::std::to_string(p->exp_size) + ":" + ::std::to_string(p->mant_size) +
    ")\n");

  *p = ::fap::FloatPrecTy(expPrec, maxMantPrec - g);

  log::BellerophonLogger::verbose(
    "After approximation: " + ::std::to_string(p->exp_size) +
    ":" + ::std::to_string(p->mant_size) +
    "\n");
}

::std::vector<::std::string> FlapAprxTechnique::applyApproximation(
      ::bellerophon::core::AprxGrade g) const
{
  ::std::vector<::std::string> args;

  // Take the maximum
  unsigned short expPrec = 11;
  unsigned short maxMantPrec = 52;
  if (this->OpRetTy == FLOAT) {
    expPrec = 8;
    maxMantPrec = 23;
  }

  assert(g <= maxMantPrec); // Precondition

  ::std::string prec = "{" + ::std::to_string(expPrec) + "," +
                       ::std::to_string(maxMantPrec - g) + "}";

  args.push_back("-D" + this->OpId + "=" + prec + "");
  log::BellerophonLogger::verbose(
    "AprxLoxContext[" + ::std::to_string(this->Id) + "] - \n" +
    "Adding argument: -D" + this->OpId + "=" + prec + "\n");

  return args;
}


void FlapAprxTechnique::dump(::llvm::raw_ostream &out) const {}
