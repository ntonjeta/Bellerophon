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
 * @file   EvolutionContext.cpp
 * @author Antonio Tammaro
 * @date   13 gen 2017
 * @brief  TODO
 ******************************************************************************/
// Tool Headers
#include "Core/EvolutionContext.h"
#include "Core/AprxContext.h"
#include "Log.h"
// Paradiseo Evolutionary Framework Headers
#include <moeo>
#include <eo>
#include <es.h>
// TODO
#include <eoEvalFuncCounter.h>
#include <core/moeoEvalFunc.h>
#include <eoInit.h>
#include <es.h> //questa non so che e`
#include <utils/eoParser.h>
#include <utils/eoState.h>
#include <utils/eoRndGenerators.h>
#include <do/make_pop.h>
#include <do/make_continue_moeo.h>

// LLVM Headers
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Debug.h"
// C/C++ Headers
#include <cassert>
#include <iostream>
#include <functional>
#include <map>
#include <limits.h>
#include <float.h>

/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     //
//                         EVOLUTION ALGORITHM METHODS                                 //
//                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////

/// \brief Initialize the exectution Engine
/// \param CompilationDatabase.json path
/// \detailed Initialize the JIT Execution Engine pass the Compilation Database path
bool bellerophon::core::EvolutionContext::initExecutionEngine(const ::std::string &cdPath) {
  // Load compilation database
  return eval.setCDatabase(cdPath);
}

/// \brief Initialize Evolution algorithm 
//TODO e sto tau
void bellerophon::core::EvolutionContext::initializeEvolution
  (double tau, std::string param)
{
  // Compiling the Database 
  ::bellerophon::log::BellerophonLogger::info(
    "Compiling the database ...\n");
  // Compile the source file
  if (this->eval.compile() != 0) {
    ::bellerophon::log::BellerophonLogger::error(
      "Error: Compilation failed. Execution stopped.\n");
    exit(1);
  }
  ::bellerophon::log::BellerophonLogger::info(
    "Compiling the database ... Done");
  
  // EVOLUTION ALGORITHM INITIALIZATION

  // TODO write more clear this code export in some function
  // ** Initialize Evaluator 
  if (!this->aprxContext->isEmptyLoc()) { // check if locations are not empy
    // Initialize location
    this->eval.setAprxLoc(this->aprxContext->getLocations());
  }else{
    ::bellerophon::log::BellerophonLogger::error(
      "Locations are empty. Execution stopped.\n");
    exit(1);
  }
  // Set Tau for Error threshold 
  this->eval.setTau(tau);

  // Fake Comand line for parser initializzation 
  int argc = 0;
  char *argv[2];
  argv[0] = new (char[4]);
  argv[1] = new (char[param.size()]);
  argv[0] = "main";
  if(!param.empty()){
    argc = 2; 
    strcpy(argv[1], param.c_str());
  }
  // ** Parser and State Initialization  
  eoParser parser(argc, argv);  // for user-parameter reading
  eoState state;                // to keep all things allocated

  // ** Random  number genrator  Initialization
  int maxGrade = this->aprxContext->getMaxApplicableGrade();
  eoUniformGenerator<unsigned int> puGen(1,maxGrade);
  
  // Initialize All Parameter  

  unsigned MAX_GEN = parser.getORcreateParam(
    unsigned(1),
    "maxGen",
    "Maximum number of generations",'G',
    "Stopping criterion" ).value();

  unsigned MIN_GEN = parser.createParam(
    unsigned(100),
    "minGen",
    "Minimum number of generations",'g',
    "Stopping criterion" ).value();
 
  unsigned STEADY_GEN = parser.createParam(
    unsigned(100),
    "steadyGen",
    "Number of generations with no improvement",'s',
    "Stopping criterion" ).value();
   
  unsigned MAX_TIME = parser.createParam(
    unsigned(100),
    "maxTime",
    "Max Time of algorithm computation",'t',
    "Stopping criterion" ).value();
  
  double P_CROSS = parser.createParam(
    double(0.25),
    "pCross", 
    "Probability of Crossover", 'C', 
    "Genetic Operators" ).value();

  double P_MUT = parser.createParam(double(0.1),
    "pMut", 
    "Probability of Mutation", 'M', 
    "Genetic Operators" ).value();

  double EPSILON = parser.createParam(
    double(0.01),
    "EPSILON",
    "Width for uniform mutation", '\0',
    "Genetic Operators" ).value();
  
  double SIGMA = parser.createParam(double(0.3),
    "SIGMA", 
    "Sigma for normal mutation", '\0',
     "Genetic Operators" ).value();

  double hypercubeRate = parser.createParam(double(1),
    "hypercubeRate", 
    "Relative rate for hypercube crossover", '\0', 
    "Genetic Operators" ).value();

  double segmentRate = parser.createParam(double(1),
    "segmentRate", 
    "Relative rate for segment crossover", '\0', 
    "Genetic Operators" ).value();

  double uniformMutRate = parser.createParam(double(1),
    "uniformMutRate", 
    "Relative rate for segment crossover", '\0', 
    "Genetic Operators" ).value();
  
  double detMutRate = parser.createParam(double(1),
    "detMutRate", 
    "Relative rate for segment crossover", '\0', 
    "Genetic Operators" ).value();
  
  double normalMutRate = parser.createParam(double(1),
    "normalMutRate", 
    "Relative rate for segment crossover", '\0', 
    "Genetic Operators" ).value();
  
  // ** Initialize genotype
  // Retrive the number of location from Approximate Context
  int dim = this->aprxContext->getLocNumb();
  // Create Intializer for population
  eoInit<aprx>* pinit = new aprxInit(dim,puGen);
  eoInit<aprx>& init = *pinit;
  // Initialize population from genotype 
  eoPop<aprx> pop = do_make_pop(parser,state,init);
   // Salve population localy
  this->pop = pop;
  // ** Initialize continuator
  
  // stop after MAX_GEN generations
  eoGenContinue<aprx> genCont(MAX_GEN);
  // do MIN_GEN gen., then stop after STEADY_GEN gen. without improvement
  eoSteadyFitContinue<aprx> steadyCont(MIN_GEN, STEADY_GEN);

  eoTimeContinue<aprx> timeCont(MAX_TIME);
  // do stop when one of the above says so
  eoCombinedContinue<aprx> continuator(genCont);
  continuator.add(steadyCont);
  continuator.add(timeCont);
  
  // ** Initialize Genetic Operator
  // TODO Select crossover and mutation configuration from CLI
  // CROSSOVER
  // uniform chooce on segment made by the parents
  eoSegmentCrossover<aprx> xoverS;
  // uniform choice in hypercube built by the parents
  eoHypercubeCrossover<aprx> xoverA;
  // Combine them with relative weights
  eoPropCombinedQuadOp<aprx> xover(xoverS, segmentRate);
  xover.add(xoverA, hypercubeRate);

  // MUTATION
  // offspring(i) uniformly chosen in [parent(i)-epsilon, parent(i)+epsilon]
  eoUniformMutation<aprx>  mutationU(EPSILON);
  // k (=1) coordinates of parents are uniformly modified
  eoDetUniformMutation<aprx>  mutationD(EPSILON);
  // Combine them with relative weights
  eoPropCombinedMonOp<aprx> mutation(mutationU, uniformMutRate);
  mutation.add(mutationD, detMutRate);
  // The operators are  encapsulated into an eoTRansform object
  eoSGATransform<aprx> op(xover, P_CROSS, mutation, P_MUT);
  // Initialize Genetic Algorithm NSGA-II
  moeoNSGAII<aprx> nsgaII(continuator,this->eval,op);
  // Run genetic algorithm
  ::bellerophon::log::BellerophonLogger::verbose
	("Applying evolution algortithm with tau= " + ::std::to_string(tau)  +"\n");
  nsgaII(this->pop);
  // printing of the initial population
  std::cout << "Final Population\n";
  this->pop.sortedPrintOn(std::cout);
  std::cout << "\n\n\n\n";
}

/// \brief print the algorithm result
/// \detail print the result of NSGAII Evolution Algorithm, print on stout
void ::bellerophon::core::EvolutionContext::printResult()
{
    // extract first front of the final population using an moeoArchive (this is the output of nsgaII)
    this->arch(pop);
    // printing of the final archive
    bellerophon::log::BellerophonLogger::info("Final Archive");
    this->arch.sortedPrintOn (std::cout);
    ::std::cout << "\n";
}

//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//                          FITNESS FUNCTION DEFINITION                                 //
//                                                                                      // 
//////////////////////////////////////////////////////////////////////////////////////////

double bellerophon::core::aprxEval::getError(::bellerophon::core::aprx &_aprx)
{
  ::std::vector<uint64_t> addrs;

  // Iterate on locations
  int i = 0;
  for (const auto &loc : this->locations) {
    addrs.clear();
    for (const auto &name : loc.technique->getGlobalValueNames()) {
      addrs.push_back(this->eeHelper.getAddress(name));
    }
    // TODO check the applicable grade of approximation
    loc.technique->applyApproximation(_aprx[i], addrs);
    i++;
  }

  ::llvm::GenericValue rv;
  if (this->eeHelper.runFunction(rv, "BELLERO_getError") != 0){
    exit(1);
  }
  // Constraint Evalutation, modify the fitness function. 
  // if error is major of tau assign high value.

  double error = rv.DoubleVal; 
  if(error >= this->tau) error = DBL_MAX; 
  return error;
}

double bellerophon::core::aprxEval::Reward(::bellerophon::core::aprx &_aprx)
{
  ::std::vector<uint64_t> addrs;

  // Iterate on locations
  int i = 0;
  for (const auto &loc : this->locations) {
    addrs.clear();
    for (const auto &name : loc.technique->getGlobalValueNames()) {
      addrs.push_back(this->eeHelper.getAddress(name));
    }
    loc.technique->applyApproximation(_aprx[i], addrs);
    i++;
  }

  ::llvm::GenericValue rv;
  if (this->eeHelper.runFunction(rv, "BELLERO_Reward") != 0){
    return this->maximizingAprx(_aprx);
  }
  return rv.DoubleVal;
}

double bellerophon::core::aprxEval::Penality(::bellerophon::core::aprx &_aprx)
{
  ::std::vector<uint64_t> addrs;

  // Iterate on locations
  int i = 0;
  for (const auto &loc : this->locations) {
    addrs.clear();
    for (const auto &name : loc.technique->getGlobalValueNames()) {
      addrs.push_back(this->eeHelper.getAddress(name));
    }
    loc.technique->applyApproximation(_aprx[i], addrs);
    i++;
  }

  ::llvm::GenericValue rv;
  if (this->eeHelper.runFunction(rv, "BELLERO_Penality") != 0){
    return 0;
  }
  return  rv.DoubleVal;
}

double bellerophon::core::aprxEval::maximizingAprx(
        bellerophon::core::aprx &_aprx)
{
  unsigned int sum = 0;
  for(int i = 0; i < _aprx.size(); i++){
    sum += _aprx[i];
  }
  return sum;
}


