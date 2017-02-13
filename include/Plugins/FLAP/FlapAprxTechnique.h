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

//===----------------------------------------------------------------------===//
/// \file FlapAprxTechnique.h
/// \author Antonio Tammaro
/// \date  20 gen 2017
/// \brief This file makes visible Approximate Context
//===----------------------------------------------------------------------===//
#ifndef INCLUDE_BELLEROPHON_FLAPAPRXTECHNIQUE_H_
#define INCLUDE_BELLEROPHON_FLAPAPRXTECHNIQUE_H_

// Tools Headers
#include "Core/AprxTechnique.h"
// C/C++ Headers
#include <cassert>

namespace flap
{
enum OperationType { ADD, SUB, MUL, DIV };

enum OperationRetType { FLOAT, DOUBLE };
}

using namespace flap;

class FlapAprxTechnique : public ::bellerophon::core::AprxTechnique 
{
public:
  // Default ctor
  FlapAprxTechnique (unsigned id,
                     const ::std::string opId,
                     OperationRetType rtTy = FLOAT,
                     OperationType ty = ADD )
    : AprxTechnique (id),OpId(opId),OpRetTy(rtTy),OpTy(ty) {
  }

  //*-------------------------------------------------------------------*
  // Get Methods
  //*-------------------------------------------------------------------*

  /// \brief Retrive Id
  /// \return the string ID
  const ::std::string& getOpId() const {
      return OpId;
  }

  //*-------------------------------------------------------------------*
  // Set Methods
  //*-------------------------------------------------------------------*

  /// \brief Set the Id
  /// \param The Id to set
  void setOpId ( const ::std::string& opId ) {
      this->OpId = opId;
  }
  /// \brief Set the OperandLHS
  /// \param The operand to set
  void setLHS ( const ::std::string lhs) {
    this->OperandLHS = lhs;
  }
  /// \brief Set the OperandRHS
  /// \param The operand to set
  void setRHS ( const ::std::string rhs) {
    this->OperandRHS = rhs;
  }

  //*-------------------------------------------------------------------*
  // Functional Methods 
  //*-------------------------------------------------------------------*

  /// \brief Provides a global value names 
  /// \details It provides the names of the global values that are required to be modified
  virtual ::std::vector<::std::string> getGlobalValueNames() const override;
  /// \brief Apply an approximation.
  /// \param The approximation grade
  /// \param Address of variable to modify
  /// \return String vector containing compiler arguments that represent the approximation
  virtual void applyApproximation ( ::bellerophon::core::AprxGrade,
                                    ::std::vector<uint64_t> ) const override;
  /// \brief Apply an approximation.
  /// \param The approximation grade
  /// \return String vector containing compiler arguments that represent the approximation
  virtual ::std::vector<::std::string> applyApproximation (
      ::bellerophon::core::AprxGrade g ) const override;
  /// \brief Dump information
  virtual void dump ( ::llvm::raw_ostream& out = ::llvm::outs() ) const override;

  ::std::string OperandLHS;
  ::std::string OperandRHS;

private:
  ::std::string OpId;        ///< Identifier of the operation
  OperationRetType OpRetTy;  ///< Which return type
  OperationType OpTy;        ///< Which operation
};

#endif /* INCLUDE_IIDEAA_FLAPAPRXTECHNIQUE_H_ */
