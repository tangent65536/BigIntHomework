/*
 * ----------------------------------------------------------------
 * BigInt.h
 * 
 * Copyright (c) Tangent65536, 2018. All Rights Reserved.
 * ----------------------------------------------------------------
 */

#ifndef _TANGENTS_BIGINT_H
#define _TANGENTS_BIGINT_H 65536

class BigInt
{
    /*
     * DO NOT ATTEMPT TO ACCESS ANY OF THESE METHODS/FUNCTIONS FROM OUTSIDE
     *  OF THE IMPLEMENTATION!
     * 
     * THESE METHODS ARE PRIVATELY USED AND MOST OF THEM MUST BE INVOKED
     *  WITH CERTAIN PRESETS MET. INVOKING THESE METHODS WITH WRONG INPUT
     *  MAY CAUSE FATAL ERRORS TO OCCUR AND/OR SPAWN DANGEROUS VULNERABILI-
     *  TIES.
     * 
     * If you HAVE to use these functions for some reason(s), PLEASE READ
     *  THE DESCRIPTION CAREFULLY BEFORE USING THEM!
     *
     * Notes: "byte array" and "char array" stand for literally the same
     *  variable type in the following descriptions.
     */
    private:
        // Length of the content char array which stores the value.
        int byteLen;
        
        // Length of the char array part without the leading zero(s).
        int numLen;
        
        // The content (value).
        unsigned char* number;
        
        // Negative.
        bool isNegative;
        
        /*
         * Creates an empty BigInt with _byteLen bytes of space.
         *
         * Params:
         *     _byteLen    -> (in) Length of the content byte array (which will be filled with zeros).
         *     dummy       -> (in) Just for recognizing this constructor.
         */
        BigInt(const int _byteLen, void* dummy);
        
        /*
         * Creates a BigInt with the content _num.
         *
         * Params:
         *     _num     -> (in/dangerous) Value of this BigInt WITHOUT copying.
         *     _isNeg   -> (in) hether this BigInt should be negative.
         *     _len     -> (in) Length of the content byte array.
         *     copy     -> (in) Dummy place holder just for recognizing this constructor.
         */
        BigInt(unsigned char* _num, bool _isNeg, int _len, void* copy);
        
        /*
         * Creates a BigInt with the content _num.
         *
         * Params:
         *     newNumber      -> (in/dangerous) New value of this BigInt WITHOUT copying.
         *     newByteLen     -> (in) Length of the content byte array.
         *     isNegative     -> (in) hether this BigInt should be negative.
         */
        void setValues(unsigned char* newNumber, int newByteLen, bool isNegative);
        
        /*
         * Preforms absolute-value byte-wise addition of two byte arrays and stores the result into a third one. The length of the first byte array MUST be not less than the second one.
         *
         * Params:
         *     cand1      -> (in) The value of the first candidate, a.k.a augend.
         *     len1       -> (in) Length of <cand1> in bytes.
         *     cand2      -> (in) The value of the second candidate, a.k.a addends.
         *     len2       -> (in) Length of <cand2> in bytes.
         *     ret        -> (out) The byte array where the result will be stored in.
         *     bLenOut    -> (in) Length of <ret> in bytes.
         */
        static void byteWiseAdditionNoCopy(const unsigned char* cand1, int len1, const unsigned char* cand2, int len2, unsigned char* ret, int bLenOut);
        
        /*
         * Preforms absolute-value byte-wise negation of two byte arrays and stores the result into a third one. The first candidate MUST be greater than the second one.
         *
         * Params:
         *     cand1      -> (in) The value of the first candidate, a.k.a. minuend.
         *     len1       -> (in) Length of <cand1> in bytes, which MUST be NOT LESS than len2.
         *     cand2      -> (in) The value of the second candidate, a.k.a subtrahend.
         *     len2       -> (in) Length of <cand2> in bytes.
         *     ret        -> (out) The byte array where the result will be stored in.
         *     bLenOut    -> (in) Length of <ret> in bytes.
         */
		static void byteWiseNegationNoCopy(const unsigned char* cand1, int len1, const unsigned char* cand2, int len2, unsigned char* ret, int bLenOut);
        
        /*
         * Preforms absolute-value byte-wise addition of two byte arrays and returns the result.
         *
         * Params:
         *     cand1      -> (in) The value of the first candidate, a.k.a augend.
         *     len1       -> (in) Length of <cand1> in bytes.
         *     cand2      -> (in) The value of the second candidate, a.k.a addends.
         *     len2       -> (in) Length of <cand2> in bytes.
         *     bLenOut    -> (out) Length of the returned char array in bytes. This will be automatically calculated.
         *
         * Returns:
         *     _ret       -> The byte array where the result is stored in, with it's length equals to <bLenOut>.
         */
        static unsigned char* byteWiseAddition(const unsigned char* cand1, int len1, const unsigned char* cand2, int len2, int& bLenOut);
        
        /*
         * Preforms absolute-value byte-wise negation of two byte arrays and returns the result. The first candidate MUST be greater than the second one.
         *
         * Params:
         *     cand1      -> (in) The value of the first candidate, a.k.a. minuend.
         *     len1       -> (in) Length of <cand1> in bytes, which MUST be NOT LESS than len2.
         *     cand2      -> (in) The value of the second candidate, a.k.a subtrahend.
         *     len2       -> (in) Length of <cand2> in bytes.
         *     bLenOut    -> (out) Length of the returned char array in bytes. This will be automatically calculated.
         *
         * Returns:
         *     _ret       -> The byte array where the result is stored in, with it's length equals to <bLenOut>.
         */
		static unsigned char* byteWiseNegation(const unsigned char* cand1, int len1, const unsigned char* cand2, int len2, int& bLenOut);
        
        /*
         * Shifts the input byte array by specific bits and returns the result as a NEW char array.
         *
         * Params:
         *     offset     -> (in) Bits of the shifting. Positive values stand for left-shifting, and negative values stand for right-shifting.
         *     cand1      -> (in) The value to be shifted.
         *     len1       -> (in) Length of <cand1> in bytes.
         *     bLenOut    -> (out) Length of the returned char array in bytes. This will be automatically calculated.
         *
         * Returns:
         *     _ret       -> The byte array where the result is stored in, with it's length equals to <bLenOut>.
         */
        static unsigned char* shiftBits(const int offset, const unsigned char* cand1, int len1, int& bLenOut);
        
        /*
         * Multiplies two byte arrays and returns the result.
         *
         * Params:
         *     cand1      -> (in) The value of the first candidate, a.k.a multiplier.
         *     len1       -> (in) Length of <cand1> in bytes.
         *     cand2      -> (in) The value of the second candidate, a.k.a multiplicand.
         *     len2       -> (in) Length of <cand2> in bytes.
         *     bLenOut    -> (out) Length of the returned char array in bytes. This will be automatically calculated.
         *
         * Returns:
         *     _ret       -> The byte array where the result is stored in, with it's length equals to <bLenOut>.
         */
        static unsigned char* multiplicationUtil(const unsigned char* cand1, int len1, const unsigned char* cand2, int len2, int& newBLen);
        
        /*
         * Divides the third byte array by the first one and stores the result in the second one.
         *
         * Params:
         *     divi        -> (in) The value of the divisor.
         *     diviLen     -> (in) Length of <divi> in bytes. [<diviLen> + <qLen> - 1] MUST BE THE LENGTH OF THE <remain>!
         *     quotient    -> (out) The value of the quotient.
         *     qLen        -> (in) Length of <quotient> in bytes. [<diviLen> + <qLen> - 1] MUST BE THE LENGTH OF THE <remain>!
         *     remain      -> (in/out) Passed into the function as the dividend, and stores the remainder when the function returns.
         */
        static void divisionUtil(const unsigned char* divi, int diviLen, unsigned char* quotient, int qLen, unsigned char* remain);
        
        /*
         * Divides this BigInt by the divisor and returns either the quotient or the remainder.
         *
         * Params:
         *     divi        -> (in) The divisor.
         *     bLenOut     -> (out) Length of the returned char array in bytes. This will be automatically calculated.
         *     q_than_r    -> (out) Whether the function returns the quotient or the remainder.
         *                      true  : returns the quotient;
         *                      false : returns the remainder.
         *
         * Returns:
         *     _ret        -> The byte array where the result is stored in, with it's length equals to <bLenOut>.
         */
		unsigned char* divisionUtil(const BigInt& divi, int& bOutLen, bool q_than_r) const;
        
        /*
         * Whether the first byte array is greater in value than the second one.
         *
         * Params:
         *     cand1      -> (in) The value of the first candidate.
         *     cand2      -> (in) The value of the second candidate.
         *     len        -> (in) Length of both <cand1> and <cand2> in bytes.
         *     equal      -> (in/out) The bool value returned when two byte arrays are equal in values.
         *
         * Returns:
         *     _ret       -> Whether the first candidate is greater than the second one, or <equal>.
         */
        static bool byteWiseGreater(const unsigned char* cand1, const unsigned char* cand2, int len, bool equal);
        
        /*
         * Whether this BigInt is greater than the input one in absolute value.
         *
         * Param:
         *     compare    -> (in) The value to be compared to.
         *
         * Returns:
         *     _ret       -> Whether this BigInt is greater than the input one. Returns false when two values are equal.
         */
        bool absGreater(const BigInt& comp) const;
        
        /*
         * Creates a array of char arrays storing the values of TENs left-shifted by 0 to 7 bits.
         *  The result is used in generating decimal string output.
         *
         * Returns:
         *     _ret    -> The returned values are listed below:
         *                  [0] -> 10 << 0, 2 bytes in length.
         *                  [1] -> 10 << 1, 2 bytes in length.
         *                  [2] -> 10 << 2, 2 bytes in length.
         *                  [3] -> 10 << 3, 2 bytes in length.
         *                  [4] -> 10 << 4, 2 bytes in length.
         *                  [5] -> 10 << 5, 2 bytes in length.
         *                  [6] -> 10 << 6, 2 bytes in length.
         *                  [7] -> 10 << 7, 2 bytes in length.
         */
        static unsigned char** CREATE_TENS();
        
        /*
         * Creates a byte array, which may be used to create a BigInt object, storing the value parsed from a decimal string.
         *
         * Params:
         *     decimalString    -> (in) The string represents the number in decimal.
         *     len              -> (in) Length of the input string.
         *     isNeg            -> (out) Whether the returned value should be negative.
         *     retLen           -> (out) Length of the returned byte array.
         *
         * Returns:
         *     _ret             -> The byte array storing the binary value of the input decimal string.
         */
        static unsigned char* createFromDecimal(const char* decimalString, int len, bool& isNeg, int& retLen);
        
        /*
         * Allocate a chunk of memory of certain length and fill it with zeros.
         *
         * Param:
         *     _len    -> (in) Length in bytes of the memory to be allocated.
         *
         * Returns:
         *     _ret    -> The byte array with the length <_len> filled with zeros.
         */
        static unsigned char* allocZerosMem(int _len);
    
    /*
     * THESE METHODS ARE AVAILABLE FOR PUBLIC USES.
     */
    public:
    	
    	/*
    	 * Creates a BigInt with the value of 0.
    	 */
        BigInt();
        
        BigInt(const int& copyFrom);
        
    	/*
    	 * Creates a BigInt with the value of the input byte array.
    	 *
    	 * PLEASE NOTE THAT THE VALUE OF THE CREATED OBJECT IS NOT THE DECIMAL
    	 *  VALUE OF THE STRING! EACH char CAN STORE FROM 0 TO 255 AND THE
    	 *  CORRESPONDED VALUE OF THE OBJECT IS REPRESENTED LIKE THAT!
    	 *
    	 * IF YOU'RE PARSING A BigInt BY DECIMAL FORMAT, PLEASE USES THE FUNCTION
    	 *  "BigInt::CreateFromDecimal()".
    	 *
    	 * Params:
    	 *     _num     -> (in) The absolute value of the BigInt to be created.
    	 *     isNeg    -> (in) Whether this BigInt should be negative.
    	 *     _len     -> (in) Length of <_num> in bytes.
    	 */
        BigInt(const char* _num, bool isNeg, int _len);
        
    	/*
    	 * (Deep) Copy a BigInt from another BigInt.
    	 *
    	 * Param:
    	 *     copyFrom    -> (in) The BigInt to be copied.
    	 */
        BigInt(const BigInt& copyFrom);
        
        /*
         * Destructor.
         */
        ~BigInt();
        
        /*
         * Set the value of this BigInt to be same as the input one and returns the new value.
         *  The returned value MAY NOT be set to any other value(s).
         *
         * Usage:
         *     <varName1> = <varName2>
         *
         * Param:
         *     copyFrom    -> (in) The value to be copied from.
         *
         * Returns:
         *     _ret        -> The value of this BigInt after setting the value to the new one.
         */
        const BigInt& operator=(const BigInt& copyFrom);
        
        /*
         * Returns the opposite number of this BigInt. The returned value MAY NOT be set to any
		 *  other value(s).
		 *
		 * Usage:
		 *     -<varName>
         *
         * Returns:
         *     _ret        -> The opposite number of this BigInt.
         */
        const BigInt operator-() const;
        
        /*
         * Returns the absolute number of this BigInt. The returned value MAY NOT be set to any
		 *  other value(s).
         *
         * Returns:
         *     _ret        -> The absolute number of this BigInt.
         */
        const BigInt abs() const;
        
        /*
         * Returns the summation of this BigInt and the input one. The returned value MAY NOT be
		 *  set to any other value(s).
		 *
		 * Usage:
		 *     <varName1> + <varName2>
		 *
         * Param:
         *     addi    -> (in) The addend.
         *
         * Returns:
         *     _ret    -> The summation of the two BigInts.
         */
        const BigInt operator+(const BigInt& addi) const;
        
        /*
         * Returns the signed difference of this BigInt and the input one. The returned value
		 *  MAY NOT be set to any other value(s).
		 *
		 * Usage:
		 *     <varName1> - <varName2>
		 *
         * Param:
         *     nega    -> (in) The subtrahend.
         *
         * Returns:
         *     _ret    -> The signed difference of the two BigInts.
         */
        const BigInt operator-(const BigInt& nega) const;
        
        /*
         * Returns the product of this BigInt and the input one. The returned value MAY NOT be
		 *  set to any other value(s).
		 *
		 * Usage:
		 *     <varName1> * <varName2>
		 *
         * Param:
         *     mult    -> (in) The multiplicand.
         *
         * Returns:
         *     _ret    -> The product of the two BigInts.
         */
        const BigInt operator*(const BigInt& mult) const;
        
        /*
         * Returns the quotient of this BigInt divided by the input one. The returned value MAY
		 *  NOT be set to any other value(s).
		 *
		 * Usage:
		 *     <varName1> / <varName2>
		 *
         * Param:
         *     divi    -> (in) The divisor.
         *
         * Returns:
         *     _ret    -> The quotient of this BigInt divided by the input one.
         */
        const BigInt operator/(const BigInt& divi) const;
        
        /*
         * Returns the remainder of this BigInt divided by the input one. The returned value MAY
		 *  NOT be set to any other value(s).
		 *
		 * Usage:
		 *     <varName1> % <varName2>
		 *
         * Param:
         *     divi    -> (in) The divisor.
         *
         * Returns:
         *     _ret    -> The remainder of this BigInt divided by the input one.
         */
        const BigInt operator%(const BigInt& divi) const;
        
        /*
         * Increases this BigInt by 1 and returns this BigInt after the operation. The returned
		 *  value MAY NOT be set to any other value(s).
		 *
		 * Usage:
		 *     ++<varname>
		 *
         * Returns:
         *     _ret    -> The value of this BigInt after the increment.
         */
        const BigInt& operator++();
        
        /*
         * Decreases this BigInt by 1 and returns this BigInt after the operation. The returned
		 *  value MAY NOT be set to any other value(s).
		 *
		 * Usage:
		 *     --<varname>
		 *
         * Returns:
         *     _ret    -> The value of this BigInt after the decrement.
         */
        const BigInt& operator--();
        
        /*
         * Increases this BigInt by 1 and returns NOTHING.
         *
         * Conventionally these operations should return the unmodified value of this object.
         *  However, copying and deleting chunks of memory costs extra time, making this operation
         *  inefficient for loops and recursions. As the result, the operation is implemented to
         *  return nothing. Manually overriding is needed if you'd like to have a value returned.
		 *
		 * Usage:
		 *     <varname>++
         */
        void operator++(int);
        
        /*
         * Decreases this BigInt by 1 and returns NOTHING.
         *  Note: Please see the description of the operation "<varName>++" above.
		 *
		 * Usage:
		 *     <varname>--
         */
        void operator--(int);
        
        /*
         * Adds the value of the input BigInt to this BigInt and returns the new value. The returned
		 *  value MAY NOT be set to any other value(s).
		 *
         * Usage:
         *     <varName1> += <varName2>
         *
         * Param:
         *     addi    -> (in) The value to be added into this BigInt.
         *
         * Returns:
         *     _ret    -> The result of this operation.
         */
        const BigInt& operator+=(const BigInt& addi);
        
        /*
         * Substracts the value of the input BigInt from this BigInt and returns the new value. The
		 *  returned value MAY NOT be set to any other value(s).
		 *
         * Usage:
         *     <varName1> -= <varName2>
         *
         * Param:
         *     nega    -> (in) The value to be substracted from this BigInt.
         *
         * Returns:
         *     _ret    -> The result of this operation.
         */
        const BigInt& operator-=(const BigInt& nega);
        
        /*
         * Multiplys the value of this BigInt by the input one. The returned value MAY NOT be set to
		 *  any other value(s).
		 *
         * Usage:
         *     <varName1> *= <varName2>
         *
         * Param:
         *     mult    -> (in) The value this BigInt to be multiplied by.
         *
         * Returns:
         *     _ret    -> The result of this operation.
         */
        const BigInt& operator*=(const BigInt& mult);
        
        /*
         * Divides the value of this BigInt by the input one. The returned value MAY NOT be set to
		 *  any other value(s).
		 *
         * Usage:
         *     <varName1> /= <varName2>
         *
         * Param:
         *     mult    -> (in) The value this BigInt to be devided by.
         *
         * Returns:
         *     _ret    -> The result of this operation.
         */
        const BigInt& operator/=(const BigInt& divi);
        
        /*
         * Sets this BigInt to the remainder of this BigInt divided by the input one. The returned
		 *  value MAY NOT be set to any other value(s).
		 *
         * Usage:
         *     <varName1> %= <varName2>
         *
		 * Param:
		 *     divi    -> (in) The value this BigInt to be devided by.
		 *
		 * Returns:
         *     _ret    -> The result of this operation.
         */
        const BigInt& operator%=(const BigInt& divi);
        
        /*
         * Compares if the value of two BigInts are equal.
         *
         * Usage:
         *     <varName1> == <varName2>
         *
         * Returns:
         *     _ret    -> Whether two BigInts have the same value.
         *                 true  : Two values are the same;
         *                 false : Otherwise.
         */
        bool operator==(const BigInt& comp) const;
        
        /*
         * Compares if the value of two BigInts are NOT equal.
         *
         * Usage:
         *     <varName1> != <varName2>
         *
         * Returns:
         *     _ret    -> Whether two BigInts have different values.
         *                 true  : Two values are NOT the same;
         *                 false : Otherwise.
         */
        bool operator!=(const BigInt& comp) const;
        
        /*
         * Compares if this BigInt is greater than or equal to the input one.
         *
         * Usage:
         *     <varName1> >= <varName2>
         *
         * Returns:
         *     _ret    -> Result ==>
         *                 true  : If this BigInt is NOT LESS than the input one;
         *                 false : Otherwise.
         */
        bool operator>=(const BigInt& comp) const;
        
        /*
         * Compares if this BigInt is less than or equal to the input one.
         *
         * Usage:
         *     <varName1> <= <varName2>
         *
         * Returns:
         *     _ret    -> Result ==>
         *                 true  : If this BigInt is NOT GREATER than the input one;
         *                 false : Otherwise.
         */
        bool operator<=(const BigInt& comp) const;
        
        /*
         * Compares if this BigInt is greater than the input one.
         *
         * Usage:
         *     <varName1> > <varName2>
         *
         * Returns:
         *     _ret    -> Result ==>
         *                 true  : If this BigInt is greater than the input one;
         *                 false : Otherwise.
         */
        bool operator>(const BigInt& comp) const;
        
        /*
         * Compares if this BigInt is less than the input one.
         *
         * Usage:
         *     <varName1> < <varName2>
         *
         * Returns:
         *     _ret    -> Result ==>
         *                 true  : If this BigInt is less than the input one;
         *                 false : Otherwise.
         */
        bool operator<(const BigInt& comp) const;
        
        /*
         * Returns the value of this BigInt left-shifted by certain bits. The returned value
		 *  MAY NOT be set to any other value(s).
         *
         * Usage:
         *     <varName> << <number of bits>
         *
         * Returns:
         *     _ret    -> result of this operation.
         */
        const BigInt operator<<(const int bits) const;
        
        /*
         * Returns the value of this BigInt right-shifted by certain bits. The returned value
		 *  MAY NOT be set to any other value(s).
         *
         * Usage:
         *     <varName> >> <number of bits>
         *
         * Returns:
         *     _ret    -> result of this operation.
         */
        const BigInt operator>>(const int bits) const;
        
        // Not implemented yet. :(
        const BigInt& operator<<=(const int bits);
        
        // Not implemented yet. :(
        const BigInt& operator>>=(const int bits);
        
        /*
         * Returns the certain digit of this BigInt represented in decimal.
         *
         * Usage:
         *     <varName>[<index>]
         *
         * Returns:
         *     _ret    -> The <index>-th digit (from right to left, starting from 0) of this
		 *                 BigInt represented in decimal.
         */
        short operator[](const int index) const;
        
        // char& operator[](const int index); <NOT POSSIBLE USING HEX-BASED ALGORITHM>
        
        /*
         * Returns the square of this BigInt. The returned value MAY NOT be set to any other
		 *  value(s).
		 *
		 * Returns:
		 *     _ret    -> The square of this BigInt.
         */
        const BigInt square() const;
        
        /*
         * Returns the integer-square-root of this BigInt. The returned value MAY NOT be set
		 *  to any other value(s).
		 *
		 * Param:
		 *     ignoreNegative    -> (in) Whether being a negative BigInt should return the result.
		 *
		 * Returns:
		 *     _ret              -> The square root as an BigInt of this BigInt. If ignoreNegative
		 *                           is false and this BigInt is negative, the value of 0 is
		 *                           returned.
         */
        const BigInt sqrt(bool ignoreNegative = false) const;
        
        /*
         * Returns whether this BigInt is a prime.
         *
         * WARNING: TIME INEFFICIENT! This costs A LOT OF time if the value is large! (Cracking RSA?)
		 *
		 * Returns:
		 *     _ret    -> Whether this BigInt is a prime.
         */
        bool isPrime() const;
        
        /*
         * Returns the signed hexadecimal representation of this BigInt.
		 *
		 * Returns:
		 *     _ret    -> The c-string represents the hexadecimal value of this BigInt. The returned pointer
		 *                 should be deleted on yourself manually.
         */
        char* getHexString() const;
        
        /*
         * Returns the signed decimal representation of this BigInt.
		 *
		 * Returns:
		 *     _ret    -> The c-string represents the decimal value of this BigInt. The returned pointer
		 *                 should be deleted on yourself manually.
         */
        char* getDecimalString() const;
        
        /*
         * Creates a BigInt parsed from a signed decimal representation stored in a c-string. The string
         *  must be terminated by a null character '\0';
         *
         * Param:
         *     decimalString    -> (in) The c-string containing the signed decimal representation of the value.
         *
         * Returns:
         *     _ret             -> The BigInt with it's value represented in signed decimal equals to the
         *                          input string.
         */
        static BigInt createFromDecimal(const char* decimalString);
        
        /*
         * Creates a BigInt parsed from a signed decimal representation stored in a c-string with the
		 *  specified length.
         *
         * Params:
         *     decimalString    -> (in) The c-string containing the signed decimal representation of the value.
         *     len              -> (in) Length of the input c-string.
         *
         * Returns:
         *     _ret             -> The BigInt with it's value represented in signed decimal equals to the
         *                          input string.
         */
        static BigInt createFromDecimal(const char* decimalString, int len);
        
        /*
         * Creates a BigInt parsed from a signed decimal representation stored in a c-string with the
		 *  specified length.
         *
         * Params:
         *     decimalString    -> (in) The c-string containing the signed decimal representation of the value.
         *     dummy            -> (in) Just a dummy for recognizing this function.
         *
         * Returns:
         *     _ret             -> The BigInt POINTER with it's value represented in signed decimal equals
		 *                          to the input string.
         */
        static BigInt* createFromDecimal(const char* decimalString, void* dummy);
        
        /*
         * Creates a BigInt parsed from a signed decimal representation stored in a c-string with the
		 *  specified length.
         *
         * Params:
         *     decimalString    -> (in) The c-string containing the signed decimal representation of the value.
         *     len              -> (in) Length of the input c-string.
         *     dummy            -> (in) Just a dummy for recognizing this function.
         *
         * Returns:
         *     _ret             -> The BigInt with it's value represented in signed decimal equals to the
         *                          input string.
         */
        static BigInt* createFromDecimal(const char* decimalString, int len, void* dummy);
        
        /*
         * @DEPRECATED :: INEFFICIENT
         *
         * Please use pure BigInt operations for efficiency!
         */
        const BigInt operator+(const int& addi) const;
        const BigInt operator-(const int& nega) const;
        const BigInt operator*(const int& mult) const;
        const BigInt operator/(const int& divi) const;
        const BigInt operator%(const int& divi) const;
};

/*
 * @DEPRECATED :: INEFFICIENT
 *
 * Please use pure BigInt operations for efficiency!
 */
const BigInt operator+(const int& addi, const BigInt& _this);
const BigInt operator-(const int& nega, const BigInt& _this);
const BigInt operator*(const int& mult, const BigInt& _this);
const BigInt operator/(const int& divi, const BigInt& _this);
const BigInt operator%(const int& divi, const BigInt& _this);

/*
 * DO NOT ACCESS THIS FUNCTION IF YOU'RE NOT SURE ABOUT HAT YOU'RE DOING! THIS IS
 *  A DIRTY HACKERY AND CAN EASILY GO WRONG WHEN BEING USED INCORRECTLY!
 *
 * Casting a pointer of type <T1> to a pointer of type <T2>. The returned pointer
 *  still points to the same area/chunk of the memory.
 *
 * Param:
 *     _ptr    -> (in) The pointer to be casted.
 *
 * Returns:
 *     _ret    -> The casted pointer.
 */
template <typename T1> T1* castPtr(auto _ptr);

#endif
