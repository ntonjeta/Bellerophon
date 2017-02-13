/******************************************************************************
 * @file   FlapAprxTechnique.h
 * @author Antonio Tammaro
 * @date   20 mar 2017
 * @brief  Implementation for the apprx Contexts of FLAP library
 ******************************************************************************/

#ifndef INCLUDE_BELLERO_FLAPAPRXCONTEXT_H_
#define INCLUDE_BELLERO_FLAPAPRXCONTEXT_H_

#include "Plugins/FLAP/fap.h"
#include "Core/AprxContext.h"


namespace bellerophon{
namespace flapContext{



class FlapAprxContext : public bellerophon::core::AprxContext {
public:
  FlapAprxContext() { }
  FlapAprxContext (bellerophon::core::AprxContextIdTy id, const ::std::string& desc) 
      : AprxContext(id,desc) {
  }
  /// \brief destructor  
  virtual ~FlapAprxContext(){}
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


};  // end class definition
}   // end flapContext namespace
}   // end bellerophon namespace

/// \brief build specific AprxContext 
/// \detailed build an instance of FlapAprxContext
/// \return  Return the istance of flapContext to main program.
::std::shared_ptr<::bellerophon::core::AprxContext> getFlapAprxContext();
#endif
