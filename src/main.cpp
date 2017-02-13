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
 * @file    main.cpp
 * @author  Antonio Tammaro
 * @date    3 gen 2017
 * @brief   Bellerophont main file.
 * @details Basic steps:
 *          - Execute an external function
 *          -
 ******************************************************************************/
// Tool Headers
#include "Tool/BellerophonTool.h"
#include "Core/AprxContext.h"
// Plugin Headers
#include "Plugins/FLAP/FlapAprxContext.h"
#include "Plugins/FLAP/FlapAprxTechnique.h"
#include "Plugins/LoopPerforation/LoopAprxContext.h"
#include "Plugins/LoopPerforation/LoopAprxTechnique.h"
// C/C++ Headers
#include <cstdlib>

using namespace std;
using namespace bellerophon;
 
main(int argc, const char *argv[]) {
  tool::BellerophonTool bellerophontool;

  bellerophontool.registerAprxContext(getFlapAprxContext());
  bellerophontool.registerAprxContext(getLoopAprxContext());
  // run tool 
  bellerophontool.run(argc,argv);
  return 0;
}
