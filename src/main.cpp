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
