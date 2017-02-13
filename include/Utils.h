//===----------------------------------------------------------------------===//
/// \file Utils.h
/// \author Antonio Tammaro
/// \brief This file contains a utility functions
//===----------------------------------------------------------------------===//

#ifndef SRC_INCLUDE_UTILS_H_
#define SRC_INCLUDE_UTILS_H_

#include "llvm/ADT/Twine.h"

#include <map>
#include <vector>
#include <string>

namespace bellerophon {
namespace conf {

/// @brief For each function name contains the vector of operator to apply.
/// The FunOp configuration file is a csv file, in which the first element of
/// every row is the
/// name of a function that should be present in the source code, the others
/// comma-separated
/// value in the row are the operator's identifier for the correspondent
/// operator to use.
///
/// If the first element of the first row is empty, it's possible to specify
/// only the operators
/// that will be applied to all functions found.
///
/// To comment a configuration entry use -> //.
using FunOpConfMap = std::map<std::string, std::vector<std::string>>;

/// @brief Reaf a functions/operators configuration file and create a
/// FunOpConfMap.
/// @retval bool If some error is encountered.
bool readFunctionsOperatorsConfFile(const std::string &filename,
                                    FunOpConfMap &fileMap);

} // End chimera::conf namespace

///////////////////////////////////////////////////////////////////////////////
/// @brief Filesystem functions
namespace fs {

extern const char pathSep;
/// @return The os-specific path separator
char getPathSeparator();

/// @brief Get the parent directory of a path
/// @param
/// @return
std::string getParentPath(const std::string &);

/// @brief Create directories e sub-directories
/// @param path The directory to create
/// @param ignoreExisting If it has to be ignored path previous existence.
/// @return If the directory is successfully created.
bool createDirectories(const llvm::Twine &path, bool ignoreExisting = true);

void deleteDirectory(const llvm::Twine &path);

} // End chimera::fs namespace

///////////////////////////////////////////////////////////////////////////////
/// @brief Syntax checker
namespace syntax {

// bool checkSyntax(std::string sourcePath, const
// clang::tooling::CompilationDatabase&);

} // end chimera::syntax namespace
} // End chimera namespace

#endif /* SRC_INCLUDE_UTILS_H_ */
