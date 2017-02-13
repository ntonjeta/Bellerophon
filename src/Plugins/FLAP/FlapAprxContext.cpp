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
 * @brief  Implementation for the flap aprx context
 ******************************************************************************/
// Tools Headers
#include "Core/AprxContext.h"
#include "Core/AprxTechnique.h"
#include "Plugins/FLAP/FlapAprxTechnique.h"
#include "Plugins/FLAP/FlapAprxContext.h"
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
using namespace flap;


::bellerophon::core::AprxGrade flapContext::FlapAprxContext::getMaxApplicableGrade() const
{
  //if (this->OpRetTy == FLOAT) {
  return 23;
  //} else if (this->OpRetTy == DOUBLE) {
    //return 52;
  //}
  //return 0;
}
::std::shared_ptr<core::AprxContext> getFlapAprxContext()
{
  ::std::shared_ptr<flapContext::FlapAprxContext> r
    (
      new flapContext::FlapAprxContext(
        "FLAPAprx",                     // Id
        "FLAP approximation plugin")    // Description
    );
  return r;
}

void flapContext::FlapAprxContext::setLocation (::std::vector<core::AprxLocation> tcnq)
{
  this->locations = tcnq;
}

core::AprxContext* flapContext::FlapAprxContext::operator= (core::AprxContext &rhs)
{
  flapContext::FlapAprxContext *r = 
      new flapContext::FlapAprxContext(rhs.getId(),rhs.getDesc());

  r->setLocation(rhs.getLocations());
  
  return r;  
}

bool flapContext::FlapAprxContext::readReport(::std::string reportPath)
{

  ::std::vector<core::AprxLocation> v;
  if (reportPath == "") {
    log::BellerophonLogger::error("Empty report path");
    return false;

  }
  constexpr char cols = 7;
  io::CSVReader<cols, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>>
      flapReport(reportPath);
  ::std::string OpId;
  int line;
  ::std::string OpTy, OpRetTy;
  ::std::string Op1, Op2, OpRet; // Operands

  log::BellerophonLogger::verbose("Reading a FLAP report:\n");
  while (flapReport.read_row(OpId, line, OpRetTy, OpTy, Op1, Op2, OpRet)) {
    log::BellerophonLogger::verbose(
      "OperationId: " + OpId +
      ", OpReType: "  + OpRetTy +
      ", OperationType: "+ OpTy + ".\n" +
      "Operand 1: " + Op1 + ", Operand 2: " + Op2 +
      ", Return Operand: " + OpRet + ".\n\n");

    OperationRetType retTy = ::flap::OperationRetType::FLOAT;
    if (OpRetTy == "DOUBLE") {
      retTy = ::flap::OperationRetType::DOUBLE;
    }
    
    OperationType Ty = ADD;
    if(OpTy == "SUB")       Ty = SUB;
    else if(OpTy == "MUL")  Ty = MUL;
    else if(OpTy == "DIV")  Ty = DIV;
    // Build an instance of FlapAprxTechnique 
    FlapAprxTechnique  c(this->LocStartId,OpId,retTy,Ty);
    this->LocStartId++;
    c.setLHS(Op1);
    c.setRHS(Op2);
    // Build an instance of AprxLocation
    core::AprxLocation l(::std::make_shared<FlapAprxTechnique>(c));
    // Push back in AprxLocation Vector
    v.push_back(l);
  }
  if(!v.empty())
    // Save the location Vector 
    setLocation(v);
  else return false;
  return true;

}
