/******************************************************************************
 * @file   ExecutionEngineHelper.h
 * @author Antonio Tammaro
 * @date   12 gen 2017
 * @brief  TODO
 ******************************************************************************/
#ifndef INCLUDE_EXECUTIONENGINEHELPER_H_
#define INCLUDE_EXECUTIONENGINEHELPER_H_

// LLVM Headers
#include "clang/Tooling/CompilationDatabase.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/TargetSelect.h"
// C/C++ Headers
#include <string>
#include <vector>

namespace bellerophon 
{
namespace engine
{
/// \{
/// \brief Helper class to interact with the LLVM execution engine
/// \details It manages a compilation database which contains the compile
/// commands for a bunch of source files.
///          These file can be compiled and linked in order to execute a
///          function defined in them.
///          At the moment is not supported the linking phase in which an
///          external library can be specified.
///          TODO: Support libraries
class ExecutionEngineHelper
{
    using CompilationDatabasePtr = ::std::unique_ptr<::clang::tooling::CompilationDatabase>;
    using CompileCommandVector = ::std::vector<::clang::tooling::CompileCommand>;
    using ModuleVector = ::std::vector<::std::unique_ptr<::llvm::Module>::pointer>;

public:
    /// \brief Default ctor
    ExecutionEngineHelper() : ee ( nullptr ), cDatabase ( nullptr ) {
        ::llvm::InitializeAllTargets();
        ::llvm::InitializeNativeTarget();
        ::llvm::InitializeNativeTargetAsmPrinter();
        ::std::unique_ptr<::llvm::Module> fakeM =
            ::llvm::make_unique<::llvm::Module> ( "fake_module",
                   this->myContext);
        fakeM->setTargetTriple ( ::llvm::sys::getProcessTriple() );

        ::std::string ErrorStr;
        this->ee.reset ( ::llvm::EngineBuilder ( ::std::move ( fakeM ) )
                         .setEngineKind ( ::llvm::EngineKind::JIT )
                         .setErrorStr ( &ErrorStr )
                         .create() );
        if ( !this->ee ) {
            ::llvm::errs() << "Unable to make execution engine: " << ErrorStr << "\n";
        }
    }

    ~ExecutionEngineHelper() {
        // Free some resources used by LLVM
        this->ee.release();  // Release the ExecutionEngine
        ::llvm::llvm_shutdown();
    }

    /// \{
    /// \brief Add the module \p M to the current execution engine. If it doesn't
    /// exist it is created.
    /// \param M A LLVM Module
    /// \return If the set succeeds
    bool loadModule ( ::std::unique_ptr<::llvm::Module> M );

    /// \brief Take the path to a compilation database and load its compile
    /// commands
    /// \param path Path to the dir in which there is the compilation database
    /// \return If the loading succeeds
    bool setCompilationDatabase ( const ::std::string& path );

    /// \brief Check if \p filename has a compile command among the internal ones
    /// \param filename The name of the file that should have a compile command
    /// \return If \p filename has a compile command
    bool hasCompileCommand ( ::llvm::StringRef filename );

    /// \brief Given a function name defined somewhere in the source files of
    /// which the compilation database have
    ///        the compile commands, execute it and eventually returns its value.
    /// \details Using the JIT compiler of LLVM, the necessary files are
    /// just-in-time compiled, and
    ///          then it is performed a linking phase and the \p funcName is
    ///          executed passing \p args as
    ///          arguments.
    /// \param[out] retval Whatever the function returns
    /// \param funcName Name of the function to execute, it should be defined in
    /// the source files
    /// \param args Arguments to pass to the functions
    /// \return 0 if succeeds, and then retval is properly set
    ///         1 otherwise
    int runFunction ( ::llvm::GenericValue& retval, ::llvm::StringRef funcName,
                      ::llvm::ArrayRef<::llvm::GenericValue> args = ::llvm::None );

    /// \brief Same as runFunction but it previous performs the compilation
    int compileAndRunFunction (
        ::llvm::GenericValue& retval, ::llvm::StringRef funcName,
        ::llvm::ArrayRef<::llvm::GenericValue> args = ::llvm::None );
    /// \}

    /// \{
    /// \brief Get the address of a global value
    /// \return Equals to 0 if not founf
    uint64_t getAddress ( ::llvm::StringRef );
    /// \}

    /// \{
    void setAdditionalArgs ( const ::std::vector<::std::string>& additionalArgs ) {
        this->additionalArgs = additionalArgs;
    }

    void clearAdditionalArgs() {
        this->additionalArgs.clear();
    }

    void setObjPaths ( const ::std::vector<::std::string>& objectPaths ) {
        this->objPaths = objectPaths;
    }

    void addObjPath ( const ::std::string& objPath ) {
        this->objPaths.push_back ( objPath );
    }

    void clearObjPaths() {
        this->objPaths.clear();
    }

    void setArchivePaths ( const ::std::vector<::std::string>& paths ) {
        this->archivePaths = paths;
    }

    void addArchivePath ( const ::std::string& path ) {
        this->archivePaths.push_back ( path );
    }

    void clearArchivePaths() {
        this->archivePaths.clear();
    }
		
		/// \brief Load the archive contained in  arcPaths
    /// \param arcPaths paths to archive files
    void loadArchive ( const ::std::string& path );
    void loadArchives( const ::std::vector<::std::string>& arcPaths);

    /// \brief Load the objects contained in \p objPaths
    /// \param objPaths Paths to object files
    void loadObj ( const ::std::string& objPath );
    void loadObjs ( const ::std::vector<::std::string>& objPaths );

    int compile() {
        return this->compileDatabaseAndLoad ( *this->cDatabase );
    }

    /// \}
private:
    /// \brief Create a local execution engine from a (first) module, and apply
    /// once-only operations
    /// \param The first module to load
    void createExecutionEngine ( ::std::unique_ptr<::llvm::Module> );

    /// \brief Compile each file in the compilation database \p cDatabase and
    ///        load them in the local execution engine
    /// \param[in] cDatabase A compilation database
    /// \param[in] funcNames Names of the functions to look after, in order to
    /// retrieve their llvm::Function*
    /// @return 0 if succeeds,
    ///         1 otherwise
    int compileDatabaseAndLoad (const ::clang::tooling::CompilationDatabase& cDatabase );

    ::std::unique_ptr<::llvm::ExecutionEngine> ee;
    CompilationDatabasePtr cDatabase;  ///< Compilation database with the compile
    /// commands for the source files
    ::std::vector<::std::string> additionalArgs;
    ModuleVector modules;
    ::std::vector<::std::string> objPaths;      ///< Paths to objects to load
    ::std::vector<::std::string> archivePaths;  ///< Paths to archive to load
    ::llvm::LLVMContext myContext;
}; // end ExecutionEngine class definition

}  // end engine namespace
}  // end bellerophon namespace

#endif /* INCLUDE_EXECUTIONENGINEHELPER_H_ */
