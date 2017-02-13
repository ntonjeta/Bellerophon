/* * Bellerophon - Design Space Exploration for Approximate Computing technique using
 * Gentic Algorithm
 * Copyright (C) 2017 Antonio Tammaro <ntonjeta@autistici.org>
 *
 * This file is part of Bellerophon.
 *
 * Bellerophon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bellerophon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Bellerophon.  If not, see <http://www.gnu.org/licenses/>.
 */

/******************************************************************************
 * @file   LoopPerforationTechnique.h
 * @author Antonio Tammaro
 * @date   07 nov 2016
 * @brief  Implementation for the apprx techniques of FLAP library
 ******************************************************************************/
#ifndef INCLUDE_BELLERO_LOOPAPRXTECHNIQUE_H_
#define INCLUDE_BELLERO_LOOPAPRXTECHNIQUE_H_

#include "Core/AprxContext.h"

namespace bellerophon{
namespace loopContext{

class LoopAprxContext : public ::bellerophon::core::AprxContext {
public:
    // Default ctor
    LoopAprxContext(){}
    LoopAprxContext (bellerophon::core::AprxContextIdTy id, const ::std::string& desc)
      : AprxContext(id,desc) {
    }

  /// \brief destructor
  virtual ~LoopAprxContext(){}
  //*-------------------------------------------------------------------*
  // Set Methods
  //*-------------------------------------------------------------------*
  /// \brief set Location Vector 
  /// \pram tcnq Vector of location
  virtual void setLocation(::std::vector<core::AprxLocation> tcqn) override;  
   //*-------------------------------------------------------------------*
  // Functional Methods 
  //*-------------------------------------------------------------------*
  /// \brief Overload of = operator 
  virtual bellerophon::core::AprxContext* operator= (
    bellerophon::core::AprxContext& rhs) override; 
  
  /// \brief Get the aprx locations vector reading report
  /// \param reportPath The path to the report from which gather the info
  /// \return true if operation have success
  virtual bool readReport(::std::string report) override;
  /// \brief Get the maximum approximation grade
  /// \return Maximum approximation grade applicable
  virtual ::bellerophon::core::AprxGrade getMaxApplicableGrade() const override;

};
} // end loopContext namespace
} // end bellerophon namespace

/// \brief build specific AprxContext
/// \detailed build an istance of LoopPerforationContext   
/// \return Return the istance of flapContext to main program.
::std::shared_ptr<::bellerophon::core::AprxContext> getLoopAprxContext ();
#endif 
