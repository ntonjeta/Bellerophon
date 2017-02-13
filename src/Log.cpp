//===- Log.cpp ----------------------------------------------------*- C++ -*-===//
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
/// \file Log.cpp
/// \author Antonio Tammaro
/// \brief This file contains a logging facility
//===----------------------------------------------------------------------===//

#include "Log.h"

using namespace bellerophon;

INITIALIZE_EASYLOGGINGPP

const char* log::BellerophonLogger::loggerName = "belleroLogger";  ///< Member initialization
el::Configurations log::BellerophonLogger::configurator =
    el::Configurations();
log::VerboseLevel log::BellerophonLogger::actualVLevel = 0;

void log::BellerophonLogger::init() {
  /// Configure el++ : chimeraLogger
  configurator.setGlobally(el::ConfigurationType::Enabled, "false");
  configurator.setGlobally(el::ConfigurationType::ToFile, "false");
  configurator.set(el::Level::Error, el::ConfigurationType::ToStandardOutput,
                   "true");
  // Debug level
  configurator.set(el::Level::Debug, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Debug, el::ConfigurationType::Format,
                   "[BELLEROPHON DEBUG] %msg");

  // Warning Level
  configurator.set(el::Level::Info, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Info, el::ConfigurationType::Format,
                   "[BELLEROPHON INFO] %msg");

  // Warning Level
  configurator.set(el::Level::Warning, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Warning, el::ConfigurationType::Format,
                   "[BELLEROPHON WARNING] %msg");

  // Error Level
  configurator.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Error, el::ConfigurationType::Format,
                   "[BELLEROPHON ERROR] %msg");
  // Fatal Level
  configurator.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");
  ;
  configurator.set(el::Level::Fatal, el::ConfigurationType::Format,
                   "[BELLEROPHON FATAL] %msg");
  el::Loggers::reconfigureLogger(loggerName, configurator);
}

void log::BellerophonLogger::initVerbose() {
  // Verbose Level
  configurator.set(el::Level::Verbose, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Verbose, el::ConfigurationType::ToFile, "false");
  configurator.set(el::Level::Verbose, el::ConfigurationType::ToStandardOutput,
                   "true");
  configurator.set(el::Level::Verbose, el::ConfigurationType::Format,
                   "[BELLEROPHON VERBOSE-%vlevel] %msg");
  el::Loggers::reconfigureLogger(loggerName, configurator);
}

void log::BellerophonLogger::initTrace() {
  // Init the verbose level
  initVerbose();
  // Debug Level
  configurator.set(el::Level::Trace, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Trace, el::ConfigurationType::ToFile, "false");
  configurator.set(el::Level::Trace, el::ConfigurationType::ToStandardOutput,
                   "true");
  configurator.set(el::Level::Trace, el::ConfigurationType::Format,
                   "[BELLEROPHON TRACE] %msg");
  el::Loggers::reconfigureLogger(loggerName, configurator);
}

void log::BellerophonLogger::init(int argc, const char** argv) {
  START_EASYLOGGINGPP(argc, argv);
  init();
}

void log::BellerophonLogger::verbose(VerboseLevel vlevel,
                                          const std::string& msg) {
  // Check if the logger has been registered
  if (!isInitialize())
    return;
  // Check the verbose level
  if (!VLOG_IS_ON(vlevel))
    return;
  std::string message = "";
  for (unsigned char i = 0; i < vlevel; i++) {
    message += "=";
  }
  message += "> " + msg;
  CVLOG(vlevel,loggerName) << message;
}
