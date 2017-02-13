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
