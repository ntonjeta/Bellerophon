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
