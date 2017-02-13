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
 * @file    main.cpp
 * @author  Antonio Tammaro
 * @date    3 gen 2017
 * @brief   Bellerophont main file.
 * @details Basic steps:
 *          - Execute an external function
 *          -
 ******************************************************************************/
#ifndef BELLEROPHON_TOOL_H
#define BELLEROPHON_TOOL_H

// Tools Headers
#include "Core/EvolutionContext.h"
#include "Core/AprxTechnique.h"
// Plugin Headers
//#include "Plugins/FLAP/FlapAprxTechnique.h"
//#include "Plugins/LoopPerforationAprx.h"
// LLVM Headers
#include "llvm/Support/Debug.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/ADT/StringMap.h"
  // C/C++ Headers
#include <cstdlib>

// Define a general namespace 
namespace bellerophon {
// Define tool namespace
namespace tool{ 


// Typedefs
using AprxContextPtr = std::unique_ptr<bellerophon::core::AprxContext>;
using AprxContextPtrMap = ::llvm::StringMap<::std::shared_ptr<bellerophon::core::AprxContext>>;

// \brief Bellerophon Tool Class
// \details The main class expose all funcionality of tool
class BellerophonTool
{
public: 
    BellerophonTool(){} // Constructor 
    // AprxContext management methods
    /// \brief Register a AprxContext 
    /// \param The AprxContext to register
    /// \return If succeeded, if the operator's identifier already exists the
    /// operation fails.
    bool registerAprxContext (::std::shared_ptr<bellerophon::core::AprxContext>);

    /// \brief Unregister a aprxContext
    /// \param The identifier of the aprxContext 
    /// \return If succeeded, id est the operator was registered
    bool unregisterAprxContext ( const bellerophon::core::AprxContextIdTy & );

   
    // \brief Run function
    // \param CLI arguments
    void run(int argc, const char *argv[]); 
private:
    /// \ brief Utility function
    /// \ param CLI arguments
    /// \ return Bool value to check if option is occured
    bool optIsOccured(const ::std::string&, int argc, const char **argv);
    /// \ brief Vector of registerd Context
    AprxContextPtrMap registeredContextMap;
    /// \brief Evolution Context tha main class
    bellerophon::core::EvolutionContext Context; 
}; // end class definition
}  // end namespace tool
}  // end namespace bellerophon
 
#endif
