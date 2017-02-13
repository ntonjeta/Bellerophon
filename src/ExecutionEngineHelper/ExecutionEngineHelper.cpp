/******************************************************************************
 * @file   ExecutionEngineHelper.cpp
 * @author Antonio Tammaro
 * @date   13 gen 2017
 * @brief  TODO
 ******************************************************************************/
#include "Log.h"

#include "ExecutionEngineHelper/ExecutionEngineHelper.h"
#include "clang/CodeGen/CodeGenAction.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Tool.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Frontend/FrontendDiagnostic.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/IR/ValueSymbolTable.h"
#include "llvm/Object/Archive.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/DynamicLibrary.h"
#include "llvm/Support/raw_ostream.h"
#include <algorithm>
#include <memory>
#include <vector>

#define DEBUG_TYPE "exec_engine"

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;

// This function isn't referenced outside its translation unit, but it
// can't use the "static" keyword because its address is used for
// GetMainExecutable (since some platforms don't support taking the
// address of main, and some platforms can't implement GetMainExecutable
// without being given the address of a function in the main executable).
std::string GetExecutablePath(const char *Argv0) {
  // This just needs to be some symbol in the binary; C++ doesn't
  // allow taking the address of ::main however.
  void *MainAddr = (void *)(intptr_t)GetExecutablePath;
  return llvm::sys::fs::getMainExecutable(Argv0, MainAddr);
}


///\{
// Private methods
void bellerophon::engine::ExecutionEngineHelper::createExecutionEngine(
    ::std::unique_ptr<::llvm::Module> M)
{
  ::bellerophon::log::BellerophonLogger::verbose(
    "Creating an execution engine and adding module \n "); // + M->getName() + "\n");

  ::std::string ErrorStr;
  this->ee.reset(::llvm::EngineBuilder(::std::move(M))
                     .setEngineKind(::llvm::EngineKind::Either)
                     .setErrorStr(&ErrorStr)
                     .create());
  if (!this->ee) {
    ::bellerophon::log::BellerophonLogger::error
      ("Unable to make execution engine: " + ErrorStr + "\n");
  } else {
    // The execution engine was successfully created
    // Actions to execute on its creation:
    // - Load 'default' objects
    this->loadObjs(this->objPaths);
    this->loadArchives(this->archivePaths);
  }
}


int bellerophon::engine::ExecutionEngineHelper::compileDatabaseAndLoad(
    const CompilationDatabase &cDB) {
  // Reset the engine
  this->ee.reset(nullptr);

  // Preliminary checks:
  // - Check if a compilation database has been loaded
  if (cDB.getAllFiles().empty()) {
    ::llvm::errs() << "The compilation database is empty\n";
    return 1;
  }

  // Path to the C/C++ file in which the function is defined
  ::std::string clangExecutable = "clang";

  // Retrieve the path to the "clang" executable
  void *MainAddr = (void *)(intptr_t)GetExecutablePath;
  std::string Path = GetExecutablePath(clangExecutable.c_str());

  // Create "triple" for the system
  // Use ELF on windows for now.
  std::string TripleStr = llvm::sys::getProcessTriple();
  llvm::Triple T(TripleStr);
  if (T.isOSBinFormatCOFF())
    T.setObjectFormat(llvm::Triple::ELF);

  // Create a diagnostic engine
  IntrusiveRefCntPtr<DiagnosticOptions> DiagOpts =
      new DiagnosticOptions(); // Options
  TextDiagnosticPrinter *DiagClient =
      new TextDiagnosticPrinter(llvm::errs(), &*DiagOpts);
  IntrusiveRefCntPtr<DiagnosticIDs> DiagID(new DiagnosticIDs());
  DiagnosticsEngine Diags(DiagID, &*DiagOpts, DiagClient);

  // TODO: Check for already existings object files
  // Common variables for the compilations
  ::std::vector<const char *> Args;
  CompileCommand cCommand;
  ::std::unique_ptr<Compilation> C;
  ::std::unique_ptr<CompilerInvocation> CI;
  ::std::unique_ptr<CodeGenAction> CodeGenAct;
  ::std::unique_ptr<llvm::Module> M;
  //  ::llvm::Function* funcEntry;

  // Common arguments
  // - Compilation arguments
  Args.push_back(Path.c_str()); // driver executable
  Args.push_back("-fsyntax-only");
  Args.push_back(
      "-Qunused-arguments"); // suppress warnings on command line arguments
  Args.push_back(
      "-fno-use-cxa-atexit"); // resolve problem with the __dso_handle
  // FIXME Some Bug, could not find stddef.h
  Args.push_back("-I/usr/lib/clang/3.9.1/include/");

  for (const auto &a : this->additionalArgs) {
    Args.push_back(a.c_str());
  }

  int commonArgsSize = Args.size();

  // Loop on all files
  ::std::vector<::std::string> objFiles;
  for (const auto &file : cDB.getAllFiles()) {
    // TODO: Automatic discovery of preexistent object files
    //    if (this->objectsPath != "") {
    //      // Before starts the compilation  see if the file is available as
    //      object file
    //      ::std::string filename =
    //      ::std::string(::llvm::sys::path::filename(file))
    //          + ".o";
    //      DEBUG(
    //          ::llvm::errs() << "Searching for object file: " << filename <<
    //          "\n");
    //      if (::llvm::sys::fs::exists(this->objectsPath + filename)) {
    //        // Save it to later loading
    //        objFiles.push_back(filename);
    //        continue;
    //      }
    //      DEBUG(::llvm::errs() << "Not found\n");
    //    }
    //    DEBUG(::llvm::errs() << "Compiling file: " << file << "\n");

    // Prepare for the compilation:

    // A Driver is a generic class for gcc-line "drivers"
    // Create a driver that drives the executable in Path, using the triple in T
    // and the Diags
    // DiagnosticsEngine
    Driver TheDriver(Path, T.str(), Diags);
    TheDriver.setTitle("clang-compiler");
    // Not check for inputs files, because there can be relative paths
    TheDriver.setCheckInputsExist(false);
    // Set C++

    // Erase not common arguments
    Args.erase(Args.begin() + commonArgsSize, Args.end());

    // Compile command of this file
    cCommand = cDB.getCompileCommands(file).at(0); // Take the first occurrence

    // Add file-specific compile commands
    cCommand.CommandLine.erase(
        cCommand.CommandLine.begin()); // Delete first element
    for (const auto &command : cCommand.CommandLine) {
      DEBUG(::llvm::errs() << "Adding compile command: " << command << "\n");
      Args.push_back(command.c_str());
    }

    // - Create a Compilation, that contains the exactly one Job to carry on
    C.reset(TheDriver.BuildCompilation(Args));
    if (!C) {
      // Error
      Diags.Report(0) << "Couldn't create Compilation";
    }
    // We expect to get back exactly one command job, if we didn't something
    // failed. Extract that job from the compilation.
    const driver::JobList &Jobs = C->getJobs();
    if (Jobs.size() != 1 || !isa<driver::Command>(*Jobs.begin())) {
      // Error
      SmallString<256> Msg;
      llvm::raw_svector_ostream OS(Msg);
      Jobs.Print(OS, "; ", true);
      Diags.Report(diag::err_fe_expected_compiler_job) << OS.str();
      return 1;
    }

    // - Create a Command and check that has been created by clang
    const driver::Command &Cmd = cast<driver::Command>(*Jobs.begin());
    if (llvm::StringRef(Cmd.getCreator().getName()) != "clang") {
      Diags.Report(diag::err_fe_expected_clang_command);
      return 1;
    }

    // - Initialize a compiler invocation object from the clang (-cc1)
    // arguments.
    const driver::ArgStringList &CCArgs = Cmd.getArguments();
    CI.reset(new CompilerInvocation);
    CompilerInvocation::CreateFromArgs(
        *CI, const_cast<const char **>(CCArgs.data()),
        const_cast<const char **>(CCArgs.data()) + CCArgs.size(), Diags);
    // Show the invocation, with -v.
    if (CI->getHeaderSearchOpts().Verbose) {
      llvm::errs() << "clang invocation:\n";
      Jobs.Print(llvm::errs(), "\n", true);
      llvm::errs() << "\n";
    }

    // Change the WorkingDir of the CompilerInvocation to the one contained in
    // the compile command
    DEBUG(::llvm::errs() << "Changing CompilerInvocation WorkingDir to: "
                         << cCommand.Directory << "\n");
    CI->getFileSystemOpts().WorkingDir = cCommand.Directory;

    // - Create a compiler instance to handle the actual work.
    CompilerInstance Clang;
    Clang.setInvocation(CI.release());
    // Create the compilers actual diagnostics engine.
    Clang.createDiagnostics();
    if (!Clang.hasDiagnostics()) {
      ::llvm::errs() << "Couldn't create the diagnostic engine.\n";
      return 1;
    }
    // Infer the builtin include path if unspecified.
    if (Clang.getHeaderSearchOpts().UseBuiltinIncludes &&
        Clang.getHeaderSearchOpts().ResourceDir.empty())
      Clang.getHeaderSearchOpts().ResourceDir =
          CompilerInvocation::GetResourcesPath(clangExecutable.c_str(),
                                               MainAddr);

    // - Create and execute the frontend to generate an LLVM bitcode module,
    // - passing the global context avoid errors on multiple modules.
    CodeGenAct.reset(new EmitLLVMOnlyAction(&this->myContext));
    if (!Clang.ExecuteAction(*CodeGenAct)) {
      ::llvm::errs() << "Couldn't execute the code gen action.\n";
      return 1;
    }

    // The CodeGenAct has been executed, it is now possible to use it and
    // retrieve the module
    // If it is the first iteration, create an internal execution engine,
    // otherwise
    // add to it the module
    M.reset(CodeGenAct->takeModule().release());
    if (M) {
      DEBUG({
        ::llvm::dbgs() << "Functions in the module: " << M->getName() << ":\n";
        for (const auto &f : M->getFunctionList()) {
          ::llvm::dbgs() << f.getName() << "\n";
        }
        ::llvm::dbgs() << "\n";
      });
      this->loadModule(::std::move(M));
    } else {
      // Error
      Diags.Report(0) << "Failure in taking a module";
    }
  }
  return 0;
}
/// \}

// Public methods
/// \{

static ::llvm::object::OwningBinary<::llvm::object::Archive> createArchive(
         const ::std::string &Path)
{
  ::llvm::ErrorOr<std::unique_ptr<::llvm::MemoryBuffer>> FileOrErr =
      ::llvm::MemoryBuffer::getFile(Path);
  if (std::error_code EC = FileOrErr.getError()){
    ::bellerophon::log::BellerophonLogger::error("Couldn't create memory buffer\n");
    return ::llvm::object::OwningBinary<::llvm::object::Archive>();
  }
  std::unique_ptr<::llvm::MemoryBuffer> Buffer = std::move(FileOrErr.get());
  
  ::llvm::Expected<std::unique_ptr<::llvm::object::Archive>> ObjOrErr =
      ::llvm::object::Archive::create(Buffer->getMemBufferRef());
  if (llvm::Error EC = ObjOrErr.takeError()) {
    ::bellerophon::log::BellerophonLogger::error("Couldn't create memory buffer\n");
    return ::llvm::object::OwningBinary<::llvm::object::Archive>();
  }
  std::unique_ptr<::llvm::object::Archive> Obj = std::move(ObjOrErr.get());

  return ::llvm::object::OwningBinary<::llvm::object::Archive>(
      std::move(Obj), std::move(Buffer));
}

void bellerophon::engine::ExecutionEngineHelper::loadArchive(const ::std::string &path)
{
  ::bellerophon::log::BellerophonLogger::verbose("Trying to load archive: " + path +  "...");
  if (::llvm::sys::fs::exists(path)) {
    this->ee->addArchive(createArchive(path));
    ::bellerophon::log::BellerophonLogger::verbose("Done\n");
  } else {
    ::bellerophon::log::BellerophonLogger::verbose("Fail, it doesn't exist\n");
  }
}

void ::bellerophon::engine::ExecutionEngineHelper::loadArchives (
        const ::std::vector<::std::string> &Paths)
{
   // - Each arcPath points to an object file (.a)
  for (const auto &arcPath : Paths) {
    this->loadArchive(arcPath);
  }
}

void bellerophon::engine::ExecutionEngineHelper::loadObj(const ::std::string &objPath)
{
  log::BellerophonLogger::verbose("Trying to load: " + objPath + "...");
  if (::llvm::sys::fs::exists(objPath)) {
    //Different bheavour according to the file extension
    if(objPath.substr(objPath.find_last_of(".") + 1) == "so") {
      //Shared object (.so)
      log::BellerophonLogger::verbose("Loading Shared Object...");
      //Try to load it as shared object   
      ::llvm::sys::DynamicLibrary so;
      char *s = new char[objPath.length()];
      ::std::strcpy(s,objPath.c_str());
      std::string *error;
      *error = "NO ERROR!";
      //Shared object loading
      if(so.LoadLibraryPermanently(s, error)) { 
        //Print any evantual error (not due to loading)
        log::BellerophonLogger::verbose("LoadLibraryPermanently ERROR: " + *error);
      } else {
        log::BellerophonLogger::verbose("Shared object loaded!");
      }
      delete s;
    } else {
      //Normal object (.o)
      log::BellerophonLogger::verbose("Loading Object..."); 
      if(auto objOr = ::llvm::object::ObjectFile::createObjectFile(objPath)) {
        auto &obj = *objOr;
        this->ee->addObjectFile(objOr.get().takeBinary().first);
        log::BellerophonLogger::verbose("object loaded!");
      }else {
        log::BellerophonLogger::verbose("ERROR: Failed to open object!");
      }
    }
  }else {
    log::BellerophonLogger::verbose("Failed, object doesn't exist!");
  }
}

void bellerophon::engine::ExecutionEngineHelper::loadObjs(
    const ::std::vector<::std::string> &objPaths) {
  // Preliminary checks:
  // - Each objPath points to an object file (.o)
  for (const auto &objPath : objPaths) {
    this->loadObj(objPath);
  }
}

bool bellerophon::engine::ExecutionEngineHelper::loadModule(
    ::std::unique_ptr<::llvm::Module> M)
{
  ::llvm::Module &m = *M;
  if (!this->ee) {
    // There isn't a local execution engine
    this->createExecutionEngine(::std::move(M));
  } else {
    DEBUG(::llvm::errs() << "Adding the module: " << M->getName()
                         << " to the execution engine\n");
    // There is a local execution engine, add the module
    this->ee->addModule(::std::move(M));
  }
  // Generate the code for this module
  this->ee->generateCodeForModule(&m);
  // Save the module
  // this->modules.push_back(&m);
  return (bool)this->ee;
}

bool bellerophon::engine::ExecutionEngineHelper::setCompilationDatabase(
    const ::std::string &path) {
  ::std::string error;
  this->cDatabase = CompilationDatabase::loadFromDirectory(path, error);
  if (!this->cDatabase) {
    return false;
  }
  DEBUG({
    ::llvm::errs() << "Compilation database dump:\n";
    for (const auto &f : this->cDatabase->getAllFiles()) {
      ::llvm::errs() << "File: " << f << "\n";
      for (const auto &c : this->cDatabase->getCompileCommands(f)) {
        ::llvm::errs() << "Directory: " << c.Directory << "\n";
        for (const auto &command : c.CommandLine) {
          ::llvm::errs() << command << " ";
        }
        ::llvm::errs() << "\n";
      }
    }
  });
  return true;
}

bool bellerophon::engine::ExecutionEngineHelper::hasCompileCommand(
    ::llvm::StringRef filename) {
  bool hasIt = false;
  this->cDatabase->getCompileCommands(filename);
  return hasIt;
}


// IMPORTANTE
int bellerophon::engine::ExecutionEngineHelper::runFunction(
    ::llvm::GenericValue &rv, ::llvm::StringRef funcName,
    ::llvm::ArrayRef<::llvm::GenericValue> ArgValues) {
  // Preliminary checks:
  // - Execution engine is set
  if (!this->ee) {
    ::llvm::errs()
        << "Execution engine is not set. Could not run a function.\n";
    return 1;
  }

  // - funcName exists in the loaded modules
  // Calling getFunctionAddress it also create al the necessary code
  if (this->ee->getFunctionAddress(funcName) == 0) {
//    llvm::errs() << funcName << " function not found in modules.\n";
    return 1;
  }
  // funcName exists and funcPtr holds its address

  ::llvm::Function *funcEntryPtr = this->ee->FindFunctionNamed(funcName.data());
  DEBUG(::llvm::errs() << "*Running function: " << funcName << "*\n");
  // The function exists at this point, this call is safe
  rv = this->ee->runFunction(funcEntryPtr, ArgValues);
  return 0;
}

int bellerophon::engine::ExecutionEngineHelper::compileAndRunFunction(
    ::llvm::GenericValue &retval, ::llvm::StringRef funcName,
    ::llvm::ArrayRef<::llvm::GenericValue> args) {
  // Compile the local database and prepare the execution engine
  if (this->compileDatabaseAndLoad(*this->cDatabase)) {
    // Error
    return 1;
  }
  // The execution engine is ready
  return this->runFunction(retval, funcName, args);
}
/// \}

uint64_t bellerophon::engine::ExecutionEngineHelper::getAddress(::llvm::StringRef Name) {
  // - Execution engine is set
  assert(this->ee && "Execution engine is not set. Could not get an address");
  uint64_t addr = this->ee->getGlobalValueAddress(Name);
  if (addr == 0) {
    ::llvm::errs() << "Global value named " << Name << " not found\n";
    exit(1);
  }
  return addr;
}
