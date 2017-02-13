/******************************************************************************
 * @file   fap.h
 * @author Federico Iannucci
 * @date   29 dic 2015
 * @brief  A Flexible Arbitrary Precision Library Header File - C++
 *         TODO: Optimize!
 ******************************************************************************/

#ifndef INCLUDE_FAP_H_
#define INCLUDE_FAP_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <map>

///////////////////////////////////////////////////////////////////////////////
/// Utilities for uin128_t, to enable compile with gcc >= 4.7.0 and -m64 flag
#ifdef __SIZEOF_INT128__

// New types
typedef __uint128_t uint128_t;
typedef __int128_t int128_t;

#define INT128_MASK_BIT_HIGH(pos)     (((uint128_t)0x01) << pos)
#define INT128_MASK_BIT_LOW(pos)      ~(INT128_MASK_BIT_HIGH(pos))
#define INT128_MASK_LOWER_LOW(num)    (((((uint128_t)0xFFFFFFFFFFFFFFFF) << 64) | 0xFFFFFFFFFFFFFFFF) << num)
#define INT128_MASK_LOWER_HIGH(num)   (~INT128_MASK_LOWER_LOW(num))
#define INT128_MASK_HIGHER_HIGH(num)  (INT128_MASK_LOWER_LOW(128 - num))

// I/O functions
#define print_uint128_hex(val) \
  do {\
    printf("%016lx", (uint64_t) (val >> 64));\
    printf("%016lx", (uint64_t) val);\
  } while(0)

#define print_int128_hex(val) print_uint128_hex(val)

inline ::std::ostream& operator<<(::std::ostream& out, uint128_t val) {
  out << ::std::setw(16) << ::std::setfill('0') << ::std::hex
      << (uint64_t) (val >> 64);
  out << ::std::setw(16) << ::std::setfill('0') << ::std::hex << (uint64_t) val
      << ::std::dec;
  return out;
}
inline ::std::ostream& operator<<(::std::ostream& out, int128_t val) {
  out << (uint128_t)val;
  return out;
}
#endif
///////////////////////////////////////////////////////////////////////////////
//#define _FAP_DEBUG_

#define _FAP_LIBRARY_

#define FLOAT_SIGN_SIZE               1
#define FLOAT_EXP_SIZE                8
#define FLOAT_MANT_SIZE               23
#define FLOAT_SIZE                    FLOAT_EXP_SIZE + FLOAT_MANT_SIZE + FLOAT_SIGN_SIZE

#define DOUBLE_SIGN_SIZE              1
#define DOUBLE_EXP_SIZE               11
#define DOUBLE_MANT_SIZE              52
#define DOUBLE_SIZE                   DOUBLE_EXP_SIZE + DOUBLE_MANT_SIZE + DOUBLE_SIGN_SIZE

#define GENERATE_RAND_FLOAT           (float)(((double)rand()/RAND_MAX) * 100)
#define GENERATE_RAND_DOUBLE          (((double)rand()/RAND_MAX)*100)

typedef uint8_t SignType;
typedef uint16_t ExpType;
typedef uint128_t MantType;

#define EXPONENT_BIAS(prec)           MASK_LOWER_HIGH(ExpType, (prec - 1))

#define MASK_BIT_HIGH(type, pos)      (type)(INT128_MASK_BIT_HIGH(pos)) ///< Create a mask with the pos-nth bit high
#define MASK_BIT_LOW(type, pos)       (type)((~MASK_BIT_HIGH(type, pos))) ///< Create a mask with the pos-nth bit low where the others are high
#define MASK_LOWER_LOW(type, num)     (type)(INT128_MASK_LOWER_LOW(num)) ///< Create a mask with all lower bit low for num positions
#define MASK_LOWER_HIGH(type, num)    (type)((~MASK_LOWER_LOW(type, num))) ///< Create a mask with all lower bit high for num positions
#define MASK_HIGHER_HIGH(type, num)   (type)(INT128_MASK_HIGHER_HIGH(num)) ///< Create a mask with all lower bit high for num positions

/// @brief Rounding methods
typedef enum {
  FAP_FP_ROUND_TOWARD_0 = 0,
  FAP_FP_ROUND_TOWARD_PINF,
  FAP_FP_ROUND_TOWARD_NINF,
  FAP_FP_ROUND_NEAREST
} FAP_rounding_method;

///// Flexible arithmetic floating point type
//typedef struct {
//  SignType sign :1;  ///< Sign used 1 bit
//  ExpType exp;  ///< Exponent on max 64 bit
//  MantType mant;  ///< Exponent on max 64 bit
//  uint8_t grs;  ///< Guard, round and sticky bits of the mantissa
//  FloatPrecTy prec;  ///< Information about the precision
//} FAP_fp_t;

///@defgroup FAP_FP_SHIFTING_FUNCTIONS FAP Floating Point Shifting functions
/// @{
void fap_shift_right_(uint128_t*, int to_shift, uint8_t* grs);
void fap_shift_left_(uint128_t*, int to_shift, uint8_t* grs);
/// @}

namespace fap {

using IntegerPrecision = uint8_t;

/// @brief Class for integer type
class IntegerType {
 public:
  IntegerType()
      : bits(0),
        oriPrecision(0),
        actualPrecision(0),
        neglectedBitsStatus(0),
        compensate(false) {
  }

  /// @brief Take a compatible int128_t
  template<typename intType>
  IntegerType(intType i, IntegerPrecision n_prec = sizeof(intType) * 8, bool c =
                  false)
      : compensate(c) {
    bits = i;
    oriPrecision = sizeof(intType) * 8;
    actualPrecision = oriPrecision;
    neglectedBitsStatus = 1;
    this->changePrec(n_prec);
  }

  // Getters and Setters
  int128_t getBits() const {
    return this->bits;
  }

  int128_t getActualBits() const {return 0;}

  void setBits(int128_t bits) {
    this->bits = bits;
  }

  int getActualPrecision() const {
    return actualPrecision;
  }

  void setActualPrecision(IntegerPrecision actualPrecision) {
    this->actualPrecision = actualPrecision;
  }

  int getOriPrecision() const {
    return oriPrecision;
  }

  void setOriPrecision(IntegerPrecision oriPrecision) {
    this->oriPrecision = oriPrecision;
  }

  uint8_t getNeglectedBitsStatus() const {
    return neglectedBitsStatus;
  }

  void setNeglectedBitsStatus(uint8_t neglectedBitsStatus) {
    this->neglectedBitsStatus = neglectedBitsStatus;
  }

  bool isCompensate() const {
    return compensate;
  }

  void setCompensate(bool compensate) {
    this->compensate = compensate;
  }

  // Overloaded operators
  /// @brief Conversion to integer types
  template<typename intType>
  explicit operator intType() const {
    return (intType) this->bits;
  }

  // Arithmetic operators
  IntegerType& operator+=(IntegerType);
  IntegerType& operator-=(IntegerType);
  IntegerType& operator*=(IntegerType);
  IntegerType& operator/=(IntegerType);

  friend IntegerType operator+(IntegerType lhs, const IntegerType& rhs) {
    lhs += rhs;
    return lhs;
  }
  friend IntegerType operator-(IntegerType lhs, const IntegerType& rhs) {
    lhs -= rhs;
    return lhs;
  }
  friend IntegerType operator*(IntegerType lhs, const IntegerType& rhs) {
    lhs *= rhs;
    return lhs;
  }
  friend IntegerType operator/(IntegerType lhs, const IntegerType& rhs) {
    lhs /= rhs;
    return lhs;
  }

  // Public methods
  /// @brief Change the precision of this integer
  void changePrec(IntegerPrecision);

  /// @brief Adapt the precision of this and \p i IntegerType to the lowest
  void adaptPrec(IntegerType& i);

 private:
  int128_t bits;
  IntegerPrecision oriPrecision;  ///< Original Integer Precision
  IntegerPrecision actualPrecision;  ///< Actual Integer precision
  uint8_t neglectedBitsStatus;  ///< Information about the neglected bits
  bool compensate;  ///< If the compensation is enabled
};

/// @brief Precision type
struct FloatPrecTy {
  /// @brief Ctor
  FloatPrecTy(uint16_t e_size = 0, uint16_t m_size = 0)
      : exp_size(e_size),
        mant_size(m_size) {
  }

  uint16_t exp_size;  ///< Size of the exponent
  uint16_t mant_size;  ///< Size of the mantissa
};

/// @brief Class for floating point type
class FloatingPointType {
 public:
  /// \{
  /// \brief Default ctor
  FloatingPointType()
      : sign(0),
        exp(0),
        mant(0),
        grs(0),
        prec(FloatPrecTy()) {
  }

  /// @brief Conversion from FAP_fp_t
//  FloatingPointType(FAP_fp_t fp) {
//    *this = fp;
//  }

  /// @brief Conversion from float
  FloatingPointType(float f, FloatPrecTy n_prec = {FLOAT_EXP_SIZE,
  FLOAT_MANT_SIZE}) {
    *this = f;
    this->changePrec(n_prec);
  }

  /// @brief Conversion from double
  FloatingPointType(double d, FloatPrecTy n_prec = {DOUBLE_EXP_SIZE,
  DOUBLE_MANT_SIZE}) {
    *this = d;
    this->changePrec(n_prec);
  }

  /// @brief Conversion from int
  FloatingPointType(int i, FloatPrecTy n_prec = {DOUBLE_EXP_SIZE,
  DOUBLE_MANT_SIZE}) {
    *this = (double) i;
    this->changePrec(n_prec);
  }
  /// \}

  /// \{
  // Getters and Setters
  SignType getSign() const {
    return (this->sign & MASK_BIT_HIGH(SignType, 0));
  }

  void setSign(SignType sign) {
    this->sign = (sign & MASK_BIT_HIGH(SignType, 0));
  }

  ExpType getExp() const {
    return (this->exp & MASK_LOWER_HIGH(ExpType, this->prec.exp_size));
  }

  void setExp(ExpType exp) {
    this->exp = (exp & MASK_LOWER_HIGH(ExpType, this->prec.exp_size));
  }

  MantType getMant() const {
//    return (this->mant & MASK_LOWER_HIGH(fap_fp_mant_t, this->prec.mant_size));
    return this->mant;
  }

  void setMant(MantType mant) {
    this->mant = (mant & MASK_LOWER_HIGH(MantType, this->prec.mant_size));
  }

  MantType getMantHb() const {
    // If mant is 0 this is the zero floating point
    if (this->isZero()) {
      return (MantType) 0;
    }
    return (MASK_BIT_HIGH(MantType, this->prec.mant_size) | this->mant);
  }

  void setMantHb() {
    this->mant = this->getMantHb();
  }

  uint8_t getGrs() const {
    return grs;
  }

  void setGrs(uint8_t grs) {
    this->grs = grs;
  }

  FloatPrecTy getPrec() const {
    return prec;
  }

  void setPrec(FloatPrecTy prec) {
    this->prec = prec;
  }

  const ::std::string& getName() const {
    return name;
  }

  void setName(const ::std::string& name) {
    this->name = name;
  }
  /// \}

  // Overloaded operators

  /// brief Conversion to FAP_fp_t
//  explicit operator FAP_fp_t() const;
  /// brief Conversion to float
  explicit operator float() const;
  /// brief Conversion to double
  explicit operator double() const;
  /// brief Conversion to int
  explicit operator int() const {
    return (int) (double) *this;
  }
  /// brief Conversion to unsigned char
  explicit operator unsigned char() const {
    return (unsigned char) (int) *this;
  }

  // Assign operators
//  FloatingPointType& operator=(FAP_fp_t);
  FloatingPointType& operator=(float);
  FloatingPointType& operator=(double);
  FloatingPointType& operator=(int i) {
    *this = (double) i;
    return *this;
  }

  // Arithmetic operators
  // Each operations is done at the minimum precision between the operands
  FloatingPointType& operator+=(const FloatingPointType& fp);
  FloatingPointType& operator-=(const FloatingPointType& fp);
  FloatingPointType& operator*=(const FloatingPointType& fp);
  FloatingPointType& operator/=(const FloatingPointType& fp);

  friend FloatingPointType operator+(FloatingPointType lhs,
                                     const FloatingPointType& rhs) {
    lhs += rhs;
    return lhs;
  }
  friend FloatingPointType operator-(FloatingPointType lhs,
                                     const FloatingPointType& rhs) {
    lhs -= rhs;
    return lhs;
  }
  friend FloatingPointType operator*(FloatingPointType lhs,
                                     const FloatingPointType& rhs) {
    lhs *= rhs;
    return lhs;
  }
  friend FloatingPointType operator/(FloatingPointType lhs,
                                     const FloatingPointType& rhs) {
    lhs /= rhs;
    return lhs;
  }

  // Unary Operator
  friend FloatingPointType operator-(FloatingPointType lhs) {
    lhs.setSign(~lhs.getSign());
    return lhs;
  }

  // Relational operators
  bool operator<=(const FloatingPointType& rhs) {
    return static_cast<double>(*this) <= static_cast<double>(rhs);
  }
  bool operator<(const FloatingPointType& rhs) {
    return static_cast<double>(*this) < static_cast<double>(rhs);
  }
  bool operator>=(const FloatingPointType& rhs) {
    return static_cast<double>(*this) >= static_cast<double>(rhs);
  }
  bool operator>(const FloatingPointType& rhs) {
    return static_cast<double>(*this) > static_cast<double>(rhs);
  }

  bool isZero() const {
    return (this->getMant() == 0 && this->getExp() == 0);
  }
  bool isSubN() const {
    return (this->getMant() != 0 && this->getExp() == 0);
  }
  bool isInf() const {
    return (this->getMant() == 0
        && this->getExp() == (MASK_LOWER_HIGH(ExpType, this->prec.exp_size)));
  }
  bool isPinf() const {
    return (this->isInf() && this->getSign() == 0);
  }
  bool isNinf() const {
    return (this->isInf() && this->getSign() != 0);
  }
  bool isNaN() const {
    return (this->getMant() != 0
        && this->getExp() == (MASK_LOWER_HIGH(ExpType, this->prec.exp_size)));
  }

  void setZero() {
    this->setExp(0);
    this->setMant(0);
  }
  void setInf() {
    this->setMant(0);
    this->setExp((MASK_LOWER_HIGH(ExpType, this->prec.exp_size)));
  }
  void setPinf() {
    this->setInf();
    this->setSign(0);
  }
  void setNinf() {
    this->setInf();
    this->setSign(1);
  }
  void setNaN() {
    this->setInf();
    this->setMant(1);
  }

  void adaptPrec(FloatingPointType&);
  void changePrec(FloatPrecTy);

  static void test(float op1, float op2);
  static void test(double op1, double op2);

  void shift(int);
  void normalize(int, int);
  void round(FAP_rounding_method method = FAP_FP_ROUND_NEAREST);

 private:
  ::std::string name; ///< For debug purposes
  SignType sign;  ///< Sign used 1 bit
  ExpType exp;  ///< Exponent on max 16 bit
  MantType mant;  ///< Mantissa on max 64 bit
  uint8_t grs;  ///< Guard, round and sticky bits of the mantissa
  FloatPrecTy prec;  ///< Information about the precision
};
}  // end fap namespace

/// @defgroup OPERATOR_OVERLOAD_INPUT_OUTPUT Input/Output overloaded operators
/// @{
::std::ostream& operator<<(::std::ostream&, const ::fap::FloatingPointType&);
::std::ostream& operator<<(::std::ostream&, const ::fap::IntegerType&);
/// @}

#endif /* INCLUDE_FAP_H_ */
