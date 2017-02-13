/******************************************************************************
 * @file    main.cpp
 * @author  Antonio Tammaro
 * @date    3 gen 2017
 * @brief   Bellerophont main file.
 * @details Basic steps:
 *          - Execute an external function
 *          -
 ******************************************************************************/
// Tools Header
#include "Tool/BellerophonTool.h"
#include "Core/AprxContext.h"
#include "Core/EvolutionContext.h"
#include "Log.h"
// LLVM Header
#include "llvm/Support/CommandLine.h"
// C/C++ Header
#include <cstdlib>

using namespace bellerophon;


static ::llvm::cl::OptionCategory catBellerophon("Bellerophon",
                                          "Options for the Bellerophon tool");

::llvm::cl::opt<double> optTau(::llvm::cl::Positional,
  ::llvm::cl::ValueRequired,
  ::llvm::cl::desc("<tau>"));

::llvm::cl::opt<::std::string> cDatabasePath("p",
  ::llvm::cl::desc("Specify the path of the parent directory of a compile_database.json file"),
  ::llvm::cl::ValueRequired,
  ::llvm::cl::value_desc("path"),
  ::llvm::cl::cat(catBellerophon));

::llvm::cl::opt<bool> optVerbose("v", 
  ::llvm::cl::desc("Enable verbose outputs"),
  ::llvm::cl::ValueDisallowed,
  ::llvm::cl::cat(catBellerophon));

::llvm::cl::opt<bool> test("test",
  ::llvm::cl::desc("Simple test"),
  ::llvm::cl::ValueDisallowed,
  ::llvm::cl::cat(catBellerophon));
                           
::llvm::cl::opt<bool> listTechnique("l",
  ::llvm::cl::desc("Show approximate technique list"),
  ::llvm::cl::ValueDisallowed,
  ::llvm::cl::cat(catBellerophon));

::llvm::cl::opt<::std::string> technique("t",
  ::llvm::cl::desc("Select the approximate technique to apply"),
  ::llvm::cl::ValueRequired,
  ::llvm::cl::cat(catBellerophon));

::llvm::cl::opt<::std::string> Report("r",
  ::llvm::cl::desc("Report produced by Approximation Operator"),
  ::llvm::cl::ValueRequired,
  ::llvm::cl::cat(catBellerophon));

::llvm::cl::list<::std::string> objPaths("objs",
  ::llvm::cl::desc("Specify an object, or multiple comma separated, to load "
                   "when linking the executable. It can be specified multiple times"),
  ::llvm::cl::CommaSeparated,
  ::llvm::cl::ValueRequired,
  ::llvm::cl::value_desc("path to object file ('.o' , '.so'"),
  ::llvm::cl::cat(catBellerophon));

::llvm::cl::list<::std::string> arcPaths("arcs",
  ::llvm::cl::CommaSeparated,
  ::llvm::cl::desc("Specify an archive object, or multiple comma separated, to load "
                   "when linking the executable. It can be specified multiple times"),
  ::llvm::cl::ValueRequired, ::llvm::cl::value_desc("archive object-path"),
  ::llvm::cl::cat(catBellerophon));

 //TODO Add CLI option for Evolution Context Parameter File
/*static ::llvm::cl::OptionCategory catEvolution("Evolution",*/
                                          /*"Options for Evolution Algorithm");*/
::llvm::cl::opt<::std::string> ParamFile("P",
  ::llvm::cl::desc("Specify the path of the parent directory of a .param file"),
  ::llvm::cl::ValueRequired,
  ::llvm::cl::value_desc("path"),
  ::llvm::cl::cat(catBellerophon));

::llvm::cl::opt<::std::string> libDbPath(
  "libdb",
  ::llvm::cl::desc(
    "Specify the patch in which of the parent directory of a library_database.csv file"),
  ::llvm::cl::ValueRequired,
  ::llvm::cl::cat(catBellerophon));

// TODO Add CLI option for Evolution Context Parameter File
static ::llvm::cl::OptionCategory catEvolution("Evolution",
                                          "Options for Evolution Algorithm");



// Utility functions
bool tool::BellerophonTool::optIsOccured(const ::std::string &optString, int argc, const char **argv) 
{
  for (int i = 0; i < argc; ++i){
    if (argv[i] == ("-" + optString)){
      return true;
    }
  }
  return false;
}

bool tool::BellerophonTool::registerAprxContext(
        ::std::shared_ptr<::bellerophon::core::AprxContext> op) {
  std::pair<typename AprxContextPtrMap::iterator, bool> retval =
      this->registeredContextMap.insert(
          std::pair<bellerophon::core::AprxContextIdTy,
                    ::std::shared_ptr<::bellerophon::core::AprxContext>>(
              op->getId(), op));
  return retval.second;
}

bool tool::BellerophonTool::unregisterAprxContext(
  const bellerophon::core::AprxContextIdTy &id) {
  return this->registeredContextMap.erase(id);
}

// run function
void tool::BellerophonTool::run(int argc, const char *argv[]) {
  // Hide options from category different from catBellerophon
  ::llvm::cl::HideUnrelatedOptions(catBellerophon);
  
	// Initilaze the log system
	log::BellerophonLogger::init();
 
	if (argc == 1) {
    ::llvm::cl::PrintHelpMessage();
  }

  // Parsing options
  ::llvm::cl::ParseCommandLineOptions(argc, argv, "Bellerophon");
  
  // Init the verbose output
  if (optVerbose) {
    log::BellerophonLogger::initVerbose();
    log::BellerophonLogger::setVerboseLevel(9);
  }
  
  double tau = optTau;
  optTau.removeArgument(); // To avoid clang complains

  // List possible approximate technique   
  if(listTechnique){
    //show the approximate technique list
    const tool::AprxContextPtrMap &map = this->registeredContextMap;
    for (auto it = map.begin(); it !=map.end(); it++) {
      log::BellerophonLogger::info("Context: " + it->first().str() + "\n");  
    }
    exit(0);
  }
  
  if (test){
    ::bellerophon::engine::ExecutionEngineHelper helper;

    // Add object files
    // IMPORTANT: Objects MUST be compiled with : -fno-use-cxa-atexit
    for (const auto &objPath : objPaths) {
      log::BellerophonLogger::info("Loading object:" + objPath + "\n");
      helper.addObjPath(objPath);
    }

    // Load compilation database
    if (!helper.setCompilationDatabase(cDatabasePath)) {
      log::BellerophonLogger::error("Execution stop: Unable to find a valid compilation database\n");
			exit(1);
    }

    helper.compile();

    ::llvm::GenericValue retval;
    if (!helper.runFunction(retval, "BELLERO_getError")) {
      log::BellerophonLogger::info("Return value: " + ::std::to_string((double)retval.DoubleVal) + "\n");
    }
    exit(0);
  }
  
  ::std::string path = cDatabasePath;
  // Initialize Execution Engine 
  if (!this->Context.initExecutionEngine(path)) {
    log::BellerophonLogger::fatal("can't initializze Execution Engine");
    exit(1);
  }
    
  // Add object files
  // IMPORTANT: Objects MUST be compiled with : -fno-use-cxa-atexit
  for (const auto &objPath : objPaths) {
    log::BellerophonLogger::verbose( "Loading object:" + objPath + "\n");
    this->Context.addObj(objPath);
  }

  for (const auto &arcPath : arcPaths) {
    ::llvm::outs() << "Loading object archive:" << arcPath << "\n";
    this->Context.addArc(arcPath);
  }
  
  if(!libDbPath.empty()){
    ::llvm::outs() << "Trying to read library_database.csv from " << libDbPath << "\n";
    this->Context.addLibraryDatabase(libDbPath);
  }

	
  // Plugin management 
  // Search in registered Context
  ::std::shared_ptr<core::AprxContext> ctx;

  const tool::AprxContextPtrMap &map = this->registeredContextMap;
  auto it = map.find(technique);

  if(it != map.end()){
    log::BellerophonLogger::verbose("Read Context " + it->first().str());
    ctx = it->second;
  }else{
    log::BellerophonLogger::error("Can't read approximate technique");
    exit(1);
  }
  
  // Intialize with Report
  ctx->readReport(Report); 
  // Set AprxContext in Evolution
  this->Context.setAprxContext(ctx); 
  
  // TODO Read the initialization for evolution algorithm from CLI
  // Initialize Evolution Parameter and apply evolution algorithm
  ::std::string param = "";
  if (!ParamFile.empty())
    param = "@" + ParamFile;
  
  this->Context.initializeEvolution(tau,param);
  
  // TODO print result on .csv
  // print results
  this->Context.printResult();   
}
