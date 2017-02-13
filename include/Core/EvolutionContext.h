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
 * @file   EvolutionContext.h
 * @author Antonio Tammaro
 * @date   12 gen 2017
 * @brief  Evolution Contex Header file. It contains all the functions necessary to run Evolution Algorithm.
 ******************************************************************************/
#ifndef INCLUDE_EVOLUTION_H
#define INCLUDE_EVOLUTION_H

 
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// Tool Headers
#include "ExecutionEngineHelper/ExecutionEngineHelper.h"
#include "Core/AprxContext.h"
// Paradiseo Evolutionary Framework Headers
#include <moeo>
// C/C++ Headers
#include <memory>
#include <string>
#include <vector>

#include "lib/csv.h"
//TODO doxygen
namespace bellerophon{
namespace core{


//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//                       MULTI-OBJECT-EVOLUTION ALGORITHM CLASS                         //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

// Specify the objective function Type 
class AprxObjectiveVectorTraits : public moeoObjectiveVectorTraits
{
public:
  static bool minimizing (int i)
  {
      if(i == 1 || i== 2 ) return true;
      else return false;
  }
  static bool maximizing (int i)
  {
      if( i == 0 ) return true;
      else return false;
  }
  static unsigned int nObjectives ()
  {
      return 3;
  }
};

// Define objective vector rappresentation
typedef moeoRealObjectiveVector <AprxObjectiveVectorTraits> aprxObjectiveVector;


// Define the solution space for multi-objective problem
class aprx : public moeoVector<aprxObjectiveVector, unsigned int>
{
public:
  // Constructor
  aprx() : nAprx(), moeoVector<aprxObjectiveVector, unsigned int>(1) { } 

  aprx( int nAprx ) : nAprx(nAprx), moeoVector<aprxObjectiveVector, unsigned int> ( nAprx ) { }
  /*-------------------------------------------------------------------*/
  // Get Methods
  //*-------------------------------------------------------------------*/
  
  int getNAprx(){ return nAprx; }
  /*-------------------------------------------------------------------*/
  // Set Methods
  //*-------------------------------------------------------------------*/
  
  void setNAprx(int nAprx){ this->nAprx = nAprx; }

  std::string className() const {
    return "aprx";
  }

private:
  // Number of aproximate location
  int nAprx;  
};

typedef eoInitFixedLength<aprx> aprxInit;

// Define the objective function
class aprxEval : public moeoEvalFunc <aprx>
{

public:
  // Constructor
  aprxEval(){}
  /// \brief define fitness function 
  void operator () (aprx & _aprx)
  {
    if (_aprx.invalidObjectiveVector())
    {
      aprxObjectiveVector objVec;
      objVec[0] = Reward(_aprx);
      objVec[1] = getError(_aprx);
      objVec[2] = Penality(_aprx);
      _aprx.objectiveVector(objVec);
    }
  }
  
  int compile(){return this->eeHelper.compile();}
  
  //[>-------------------------------------------------------------------*
  // Set Methods
  //[>-------------------------------------------------------------------*
  
  void setTau(double tau){
    this->tau = tau;
  }

  bool setCDatabase(const ::std::string &cdpath){
    this->eeHelper.setCompilationDatabase(cdpath);
  }
 
  void setAprxLoc(const ::bellerophon::core::AprxLocationVector &v){this->locations=v;}

  void addObj ( const ::std::string& objPath )
  {
    if ( objPath != "" ) {
      //this->eeHelper->addObjPath ( objPath );
      this->eeHelper.addObjPath(objPath);
    }
  }
  
  void addArchivePath ( const ::std::string& arcPath ) {
    if ( arcPath != "" ) {
        this->eeHelper.addArchivePath ( arcPath );
    }
  }
  /// \brief Load library modules listed into the library_database.csv
  /// \param
  bool addLibraryDatabase(::std::string libDbPath){
    io::CSVReader<1, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>>
    libraryDatabase(libDbPath+"library_database.csv");
    ::std::string lib;
    while (libraryDatabase.read_row(lib)){
      if(lib.substr(0, 2) != "\\\\"){
        if(has_suffix(lib, ".o") || has_suffix(lib, ".so") ){
          ::llvm::outs() <<  "Adding " << lib << "\n";
          this->eeHelper.addObjPath ( libDbPath+"libs.o/"+lib );
        }
        if(has_suffix(lib, ".a")){
          ::llvm::outs() <<  "Adding " << lib << "\n";
          this->eeHelper.addArchivePath ( libDbPath+"libs.a/  "+lib );
        }
      }
    }
  }


private:
  /// \brief Given aprx locations, apply approximations based on the approximation mode of
  ///        the aprx contexts, and compute the error
  /// \param[in] v Approximations
  /// \return user-defined error, provided by an BELLEROPHON _getError function
  double getError(aprx &_aprx);

  double maximizingAprx(aprx & _aprx);

  double Penality(aprx & _aprx);
  
  double Reward(aprx & _aprx);

  double tau;

  ::bellerophon::core::AprxLocationVector locations;
  ::bellerophon::engine::ExecutionEngineHelper eeHelper;

  static bool has_suffix(const std::string &str, const std::string &suffix)
  {
    return str.size() >= suffix.size() &&
    str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
  }

};


//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//                          EVOLUTION CONTEXT CLASS                                     //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////


class EvolutionContext
{

public:

  EvolutionContext(){}

  //[>-------------------------------------------------------------------*
  // Initialization Methods
  //[>-------------------------------------------------------------------*

  /// \brief Init function for the internal execution engine
  bool initExecutionEngine ( const ::std::string& );

  /// \brief Initialize the Aproximate Context
  /// \param report path
  /// \return Satus Code 
  //int initializeAprxContext(::std::string path){} 
 
  /// \brief Initialize the Evolution Parameter 
  /// \param path to parameter file
  /// \detail Initialize the Evolution Parameter from file specified by path
  void initializeEvolution(double, std::string);

  /// \brief Add an object that will be loaded
  /// \param objPath Path to the object
  void addObj ( const ::std::string& objPath )
  {
    if ( objPath != "" ) {
      this->eval.addObj(objPath);
    }
  }
  
  /// \brief Add an object archive that will be loaded
  /// \param arcPath Path to the object archive (.a)
  void addArc ( const ::std::string& arcPath ) {
    if ( arcPath != "" ) {
      this->eval.addArchivePath (arcPath);
    }
  }

  void addLibraryDatabase(const ::std::string& path) {
    this->eval.addLibraryDatabase(path);
  } 

  //[>-------------------------------------------------------------------*
  // Functional Methods
  //[>-------------------------------------------------------------------*
  
  /// \brief Print the result of evolution algorithm
  void printResult ();
  
	//[>-------------------------------------------------------------------*
  // Set Methods
  //[>-------------------------------------------------------------------*
  
  /// \brief Initialize the Aprx Context
  void setAprxContext(const ::std::shared_ptr<::bellerophon::core::AprxContext> C){ 
    this->aprxContext = C;
   }
  //uint64_t getExploredNodesNum();
  //uint64_t getLeavesNum();
private:  
  // Approximate Context
  ::std::shared_ptr<::bellerophon::core::AprxContext> aprxContext;  

  moeoUnboundedArchive<aprx> arch;
  aprxEval         eval;
  eoPop<aprx>      pop;
}; // end evolutionContext class definition 
}  // end evolution namespace
}  // end bellerophon namespace

#endif /* INCLUDE_EVOLUTION_H */

