/******************************************************************************
 * @file   test.cpp
 * @author Mario Barbareschi
 * @date   09 apr 2016
 * @brief  A source file used to test FLAP Operator
 ******************************************************************************/

#include <iostream>
#include <math.h>
#include "fap.h"

/// \brief Returns something
/// @param a
/// @param b
/// \param n
/// @return
::fap::FloatPrecTy OP_1(8,23);
::fap::FloatPrecTy OP_0(8,23);
float product(float a[], float b[], int n) {
  float p = 0;
  for (unsigned i = 0; i < n; i++) {
    p = (float)(::fap::FloatingPointType((float) p, OP_0)) + ((float)(::fap::FloatingPointType((float) a[i], OP_1)) * (float)(::fap::FloatingPointType((float) b[i], OP_1)));
  }
  return p;
}

static float a[] = {0.0625, 0.0625, 0.0937, 0.1250, 0.0313, 0.2187, 0.0625, 0.1875, 0.1563};
static float b[] = {144.5, 103.9, 0.0012, 0.5877, 96544.2178, 63.22, 541.25, 0.2565, 3873.512};
static int n = 9;

extern "C" double BELLERO_getError() {
  double golden = 3690.56518554688;
  return double(fabs(product(a, b, n)));
}

extern "C" double BELLERO_Penality() {
  return 0;
}
int main(int argc, char **argv) {
  ::std::cout << "Product: " << ::std::setprecision(15) << product(a, b, n)
              << "\n"; // 3.685547225000000e+03
  return 0;
}
