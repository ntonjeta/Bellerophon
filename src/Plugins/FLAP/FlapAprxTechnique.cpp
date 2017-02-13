//
// Bellerophon - Design Space Exploration for Approximate Computing technique using
// Gentic Algorithm
// Copyright (C) 2017 Antonio Tammaro <ntonjeta@autistici.org>
//
// This file is part of Bellerophon.
//
// Bellerophon is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Bellerophon is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Bellerophon.  If not, see <http://www.gnu.org/licenses/>.
//

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
