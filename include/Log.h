//===- Log.h ----------------------------------------------------*- C++ -*-===//
//
//  Copyright (C) 2016, 2017  Antonio Tammaro (ntonjeta@autistici.org)
// 
//  This file is part of Bellerophon.
//
//  Clang-Chimera is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Affero General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Clang-Chimera is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Affero General Public License for more details.
//
//  You should have received a copy of the GNU Affero General Public License
//  along with Clang-Chimera. If not, see <http://www.gnu.org/licenses/>.
//
//===----------------------------------------------------------------------===//
/// \file Log.h
/// \author Antonio Tammaro
/// \brief This file contains a logging facility
//===----------------------------------------------------------------------===//

#ifndef SRC_INCLUDE_LOG_H_
#define SRC_INCLUDE_LOG_H_

#define ELPP_NO_DEFAULT_LOG_FILE            ///< Disable default logs folder.
#define ELPP_DISABLE_DEFAULT_CRASH_HANDLING ///< Disable crash handling
#include "lib/easylogging++.h"

namespace bellerophon{
namespace log {
// FIXME Delete this intermediate class?
// Typedefs
using VerboseLevel = el::base::type::VerboseLevel;
/// @brief BelleroLogger Class, provides all functions for the logging
/// @details Through this helper class, it's simpler to use easylogging++.
class BellerophonLogger {
public:
  /// @brief Initialize the logging class
  static void init();
  /// @brief Enable the verbose level
  static void initVerbose();
  /// @brief Enable the debug level
  static void initTrace();
  static void init(int argc, const char **argv);

  static bool isInitialize() {
    return el::Loggers::getLogger(loggerName, false);
  }

  static void setVerboseLevel(VerboseLevel vlevel) {
    el::Loggers::setVerboseLevel(vlevel);
  }

  static void setActualVLevel(VerboseLevel vlevel) {
    actualVLevel = vlevel >= 0 && vlevel <= 9 ? vlevel : 0;
  }
  static void resetActualVLevel() { actualVLevel = 0; }
  static void incrActualVLevel() {
    actualVLevel = actualVLevel < 9 ? actualVLevel + 1 : 9;
  }

  static void decrActualVLevel() {
    actualVLevel = actualVLevel > 0 ? actualVLevel - 1 : 0;
  }

  /// @brief Log a message for the Verbose level
  ///
  /// @details Verbose levels are from 0 to 9
  static void verbose(VerboseLevel, const std::string &msg);

  /// @brief Log a message in Verbose actual level

  static void verbose(const std::string &msg) { verbose(actualVLevel, msg); }

  static void verbose(const std::string &&msg) { verbose(actualVLevel, msg); }

  static void verboseAndIncr(const std::string &msg) {
    verbose(msg);
    incrActualVLevel();
  }

  static void verboseAndIncr(const std::string &&msg) {
    verbose(msg);
    incrActualVLevel();
  }

  static void verboseAndDecr(const std::string &msg) {
    verbose(msg);
    decrActualVLevel();
  }

  static void verboseAndDecr(const std::string &&msg) {
    verbose(msg);
    decrActualVLevel();
  }

  static void verbosePreDecr(const std::string &msg) {
    decrActualVLevel();
    verbose(msg);
  }

  static void verbosePreDecr(const std::string &&msg) {
    decrActualVLevel();
    verbose(msg);
  }

  static void debug(const std::string &msg) { CLOG(DEBUG, loggerName) << msg; }

  static void debug(const std::string &&msg) { CLOG(DEBUG, loggerName) << msg; }

  static void info(const std::string &msg) { CLOG(INFO, loggerName) << msg; }

  static void info(const std::string &&msg) { CLOG(INFO, loggerName) << msg; }

  static void trace(const std::string &msg) { CLOG(TRACE, loggerName) << msg; }

  static void trace(const std::string &&msg) { CLOG(TRACE, loggerName) << msg; }

  static void warning(const std::string &msg) {
    CLOG(WARNING, loggerName) << msg;
  }

  static void warning(const std::string &&msg) {
    CLOG(WARNING, loggerName) << msg;
  }

  static void error(const std::string &msg) { CLOG(ERROR, loggerName) << msg; }

  static void error(const std::string &&msg) { CLOG(ERROR, loggerName) << msg; }

  static void fatal(const std::string &msg) { CLOG(FATAL, loggerName) << msg; }

  static void fatal(const std::string &&msg) { CLOG(FATAL, loggerName) << msg; }

private:
  static const char *loggerName;
  static el::Configurations configurator;
  static VerboseLevel actualVLevel;
};
}
}

#endif /* SRC_INCLUDE_LOG_H_ */
