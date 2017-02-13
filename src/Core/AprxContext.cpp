/******************************************************************************
 * @file    AprxContext.cpp
 * @author  Antonio Tammaro
 * @date    20 gen 2017
 * @brief   AprxContext header file.
 * @details TODO
 *          
 ******************************************************************************/

// Tools Headers
#include "Core/AprxContext.h"
#include "Core/AprxTechnique.h"
#include <Log.h>
// C/C++ Headers

using namespace bellerophon;

/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     // 
//                            LOCATION MANGMENT METHODS                                //
//                                                                                     //  
/////////////////////////////////////////////////////////////////////////////////////////

void core::AprxContext::printLocation()
{
  this->printLocation(this->locations);
}

void core::AprxContext::printLocation(
     const AprxLocationVector &locations)
{
  for (const auto &loc : locations) {
    log::BellerophonLogger::info(
        "AprxLocation-> Id:" + ::std::to_string(loc.technique->getId()) 
        + ", Grade:"         + ::std::to_string(loc.g) 
        + "\n");
  }
}


/// \brief Add one location in Evolution Context
/// \param AprxTechnique instance
void core::AprxContext::addLocation(::std::shared_ptr<core::AprxTechnique> c)
{
  this->locations.push_back(c);
}

void core::AprxContext::addLocations(
      const ::std::vector<::std::shared_ptr<core::AprxTechnique>> tcnq)
{
  for (auto t : tcnq) {
    this->addLocation(t);
  }
}
