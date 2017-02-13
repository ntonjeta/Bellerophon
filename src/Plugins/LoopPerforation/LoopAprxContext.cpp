/******************************************************************************
 * @file   LoopAprxTechnique.cpp
 * @author Antonio Tammaro
 * @date   23 gen 2017
 * @brief  Implementation for the Loop Aprx context
 ******************************************************************************/
// Tools Headers
#include "Core/AprxContext.h"
#include "Core/AprxTechnique.h"
#include "Plugins/LoopPerforation/LoopAprxTechnique.h"
#include "Plugins/LoopPerforation/LoopAprxContext.h"
#include "Log.h"
#include "lib/csv.h"
// LLVM Headers
#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/raw_ostream.h"
// C/C++ Headers
#include <map>
#include <memory>
#include <utility>

using namespace bellerophon;

::bellerophon::core::AprxGrade loopContext::LoopAprxContext::getMaxApplicableGrade() const
{
  // TODO return corret applicable grade
  return 10;
}

void loopContext::LoopAprxContext::setLocation (::std::vector<core::AprxLocation> tcnq)
{
  this->locations = tcnq;
}

::std::shared_ptr<core::AprxContext> getLoopAprxContext()
{
  ::std::shared_ptr<loopContext::LoopAprxContext> r
    (
      new loopContext::LoopAprxContext(
        "LoopAprx",                     // Id
        "Loop Perforation approximation plugin")    // Description
    );
  return r;
}

core::AprxContext* loopContext::LoopAprxContext::operator= (core::AprxContext &rhs)
{
  loopContext::LoopAprxContext *r = 
      new loopContext::LoopAprxContext(rhs.getId(),rhs.getDesc());

  r->setLocation(rhs.getLocations());
  
  return r;  
}

bool loopContext::LoopAprxContext::readReport(::std::string reportPath)
{

  ::std::vector<core::AprxLocation> v;
  if (reportPath == "") {
    log::BellerophonLogger::error("Empty report path");
    return false;
  }
  constexpr char cols = 4;
  io::CSVReader<cols, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>>
      loopReport(reportPath);

  ::std::string OpId;
  int line;
  ::std::string inc;
  int lenght;

  log::BellerophonLogger::verbose("Reading a LOOP report:\n");
  while (loopReport.read_row(OpId, line, inc, lenght)) {
    log::BellerophonLogger::verbose(
      "OperationId: " + OpId); 
    // Build an instance of LoopAprxTechnique 
    LoopAprxTechnique  c(this->LocStartId,OpId,inc,lenght); 
    this->LocStartId++;
    // Build an instance of AprxLocation
    core::AprxLocation l(::std::make_shared<LoopAprxTechnique>(c));
    v.push_back(l);
  }
  if(!v.empty())
    // Save the location Vector 
    setLocation(v);
  else return false;
  return true;

}
