/*
 * ----------------------------------------------------------------
 * BigInt.cpp
 * 
 * Copyright (c) Tangent65536, 2018-2022. All Rights Reserved.
 * 
 *  This is the implementation of the header "BigInt.h", providing
 *   basic big-integer computation functions. The data is stored
 *   as binary in unsigned char arrays.
 * ----------------------------------------------------------------
 */

#include <string.h>
#include "BigInt.h"
#include "internal_util.h"

BigInt::BigInt()
{
	this->byteLen = this->numLen = 0;
	this->number = nullptr;
	this->isNegative = false;
}

BigInt::BigInt(const int _byteLen, void* dummy)
{
	this->byteLen = _byteLen;
	this->numLen = 0;
	this->number = allocZerosMem(this->byteLen);
	this->isNegative = false;
}

// "void* copy" is just a dummy indicating this constructor is called.
BigInt::BigInt(unsigned char* _num, bool _isNeg, int _len, void* copy = nullptr)
{
	this->byteLen = this->numLen = _len;
	for(int i = _len - 1 ; i >= 0 ; i--)
	{
		if(_num[i] != 0)
		{
			break;
		}
		this->numLen--;
	}
	this->number = _num;
	this->isNegative = _isNeg;
}

BigInt::BigInt(const char* _num, bool _isNeg, int _len)
{
	for(int i = _len - 1 ; i >= 0 ; i--)
	{
		if(_num[i] != 0)
		{
			break;
		}
		_len--;
	}
	this->byteLen = this->numLen = _len;
	this->number = new unsigned char[this->byteLen];
	
	// Just like "strcpy()", but copies whatever inside the specified length, including zeros.
	memcpy(this->number, _num, this->byteLen);
	this->isNegative = _isNeg;
}

BigInt::BigInt(const BigInt& copyFrom)
{
	this->byteLen = this->numLen = copyFrom.numLen;
	this->number = new unsigned char[this->byteLen];
	memcpy(this->number, copyFrom.number, this->byteLen);
	this->isNegative = copyFrom.isNegative;
}

BigInt::~BigInt()
{
	delete [] this->number;
}

int BigInt::getByteLength() const
{
	return this->byteLen;
}

// This WILL NOT copy the input data!
void BigInt::setValues(unsigned char* newNumber, int newByteLen, bool newNegative)
{
	delete [] this->number;
	this->number = newNumber;
	this->byteLen = newByteLen;
	this->isNegative = newNegative;
	
	for(int i = (this->byteLen - 1) ; i >= 0 ; i--)
	{
		if(this->number[i] != 0)
		{
			this->numLen = i + 1;
			return;
		}
	}
	this->numLen = 0;
}

void BigInt::byteWiseAdditionNoCopy(const unsigned char* cand1, int len1, const unsigned char* cand2, int len2, unsigned char* ret, int bLenOut)
{
	unsigned short cache = 0;
	cache = 0;
	
	// Pointing to the lower byte in the short "cache".
	unsigned char *cPtr = castPtr<unsigned char>(&cache);
	
	int i;
	for(i = 0 ; i < len2 ; i++)
	{
		cache += cand1[i];
		cache += cand2[i];
		ret[i] = *cPtr;
		// The carry (higher byte in the "cache") of this partial addition should be added into the next byte.
		cache >>= 8;
	}
	
	for(i = len2 ; i < len1 ; i++)
	{
		cache += cand1[i];
		ret[i] = *cPtr;
		// The carry (higher byte in the "cache") of this partial addition should be added into the next byte.
		cache >>= 8;
	}
	
	if(bLenOut > len1)
	{
		ret[len1] = *cPtr;
	}
}

unsigned char* BigInt::byteWiseAddition(const unsigned char* cand1, int len1, const unsigned char* cand2, int len2, int& bLenOut)
{
	bool lenComp = (len1 > len2);
	bLenOut = (lenComp ? len1 : len2) + 1;
	unsigned char *ret = new unsigned char[bLenOut];
	if(lenComp)
	{
		byteWiseAdditionNoCopy(cand1, len1, cand2, len2, ret, bLenOut);
	}
	else
	{
		byteWiseAdditionNoCopy(cand2, len2, cand1, len1, ret, bLenOut);
	}
	return ret;
}

void BigInt::byteWiseNegationNoCopy(const unsigned char* cand1, int len1, const unsigned char* cand2, int len2, unsigned char* ret, int bLenOut)
{
	unsigned short cache = 0;
	
	// Pointing to the lower byte in the short "cache".
	unsigned char *cPtr = castPtr<unsigned char>(&cache);
	char temp = 0;

	int i;
	// The input is made sure that cand1 > cand2 is always true.
	for(i = 0 ; i < len2 ; i++)
	{
		cache = cand1[i] - cache;
		cache -= cand2[i];
		ret[i] = *cPtr;
		
		// The carry (higher byte in the "cache") of this partial addition should be added into the next byte.
		// In this case, the "extra 1" that has to be negated in the next byte.
		*cPtr = 0x100 - (cache >>= 8);
	}
	
	for(i = len2 ; i < len1 ; i++)
	{
		cache = cand1[i] - cache;
		ret[i] = *cPtr;
		
		// The carry (higher byte in the "cache") of this partial addition should be added into the next byte.
		// In this case, the "extra 1" that has to be negated in the next byte.
		*cPtr = 0x100 - (cache >>= 8);
	}
}

unsigned char* BigInt::byteWiseNegation(const unsigned char* cand1, int len1, const unsigned char* cand2, int len2, int& bLenOut)
{
	bLenOut = len1;
	unsigned char *ret = new unsigned char[bLenOut];
	byteWiseNegationNoCopy(cand1, len1, cand2, len2, ret, bLenOut);
	
	return ret;
}

bool BigInt::byteWiseGreater(const unsigned char* cand1, const unsigned char* cand2, int len, bool equal = false)
{
	for(int i = len - 1 ; i >= 0 ; i--)
	{
		if(cand1[i] > cand2[i])
		{
			return true;
		}
		else if(cand1[i] < cand2[i])
		{
			return false;
		}
	}
	
	// Equal.
	return equal;
}

bool BigInt::absGreater(const BigInt& comp) const
{
	if(this->numLen == comp.numLen)
	{
		return byteWiseGreater(this->number, comp.number, this->numLen);
	}
	else
	{
		return (this->numLen > comp.numLen);
	}
}

unsigned char* BigInt::shiftBits(int offset, const unsigned char* cand1, int len1, int& bLenOut)
{
	unsigned char *ret = nullptr;
	if(offset > 0)
	{
		int offBytes = offset / 8;
		int offBits = offset % 8;
		int offBitsCompliment = 8 - offBits;
		bLenOut = len1 + offBytes + 1;
	
		ret = allocZerosMem(bLenOut);
		for(int i = 0 ; i < len1 ; i++)
		{
			ret[i + offBytes] += cand1[i] << offBits;
			ret[i + offBytes + 1] = cand1[i] >> offBitsCompliment; // The "head" of the byte after that is "carried" to this byte.
		}
	}
	else if(offset < 0)
	{
		offset = -offset;
		int offBytes = offset / 8;
		int offBits = offset % 8;
		int offBitsCompliment = 8 - offBits;
		bLenOut = len1 - offBytes;
		ret = new unsigned char[bLenOut];
		for(int i = offBytes ; i < len1 - 1 ; i++)
		{
			ret[i - offBytes] = (cand1[i] >> offBits) + (cand1[i + 1] << offBitsCompliment);
		}
		ret[len1 - offBytes - 1] = (cand1[len1 - 1] >> offBits);
	}
	else // offset == 0 is only used in division processing.
	{
		bLenOut = len1 + 1;
		ret = new unsigned char[bLenOut];
		ret[len1] = 0;
		memcpy(ret, cand1, len1);
	}
	return ret;
}

unsigned char* BigInt::allocZerosMem(int _len)
{
	unsigned char* ret = new unsigned char[_len];
	memset(ret, 0, _len);
	return ret;
}

const BigInt& BigInt::operator=(const BigInt& copyFrom)
{
	if(this == &copyFrom)
	{
		return *this;
	}
	
	if(this->byteLen != copyFrom.numLen)
	{
		delete [] this->number;
		this->byteLen = this->numLen = copyFrom.numLen;
		this->number = new unsigned char[this->byteLen];
	}
	
	memcpy(this->number, copyFrom.number, this->byteLen);
	this->isNegative = copyFrom.isNegative;
	
	return *this;
}

const BigInt BigInt::operator-() const
{
	BigInt ret(*this);
	ret.isNegative = !(this->isNegative);
	return ret;
}

const BigInt BigInt::abs() const
{
	BigInt ret(*this);
	ret.isNegative = false;
	return ret;
}

const BigInt BigInt::operator+(const BigInt& addi) const
{
	if(this->numLen == 0)
	{
		return addi;
	}
	else if(addi.numLen == 0)
	{
		return *this;
	}
	
	int newBLen;
	if(this->isNegative ^ addi.isNegative)
	{
		if(this->absGreater(addi))
		{
			unsigned char* cRet = byteWiseNegation(this->number, this->numLen, addi.number, addi.numLen, newBLen);
			
			// this->isNegative is true ==> (---) + (+) === (-), else (+++) + (-) === (+).
			return BigInt(cRet, this->isNegative, newBLen, nullptr);
		}
		else
		{
			unsigned char* cRet = byteWiseNegation(addi.number, addi.numLen, this->number, this->numLen, newBLen);
			
			// this->isNegative is true ==> addi is positive ==> (-) + (+++) === (+), else (+) + (---) === (-).
			return BigInt(cRet, addi.isNegative, newBLen, nullptr);
		}
	}
	else
	{
		unsigned char* cRet = byteWiseAddition(this->number, this->numLen, addi.number, addi.numLen, newBLen);
		return BigInt(cRet, this->isNegative, newBLen, nullptr);
	}
}

const BigInt BigInt::operator-(const BigInt& nega) const
{
	if(this->numLen == 0)
	{
		BigInt ret = BigInt(nega);
		ret.isNegative = !(ret.isNegative);
		return ret;
	}
	else if(nega.numLen == 0)
	{
		return *this;
	}
	
	int newBLen;
	if(this->isNegative ^ nega.isNegative)
	{
		unsigned char* cRet = byteWiseAddition(this->number, this->numLen, nega.number, nega.numLen, newBLen);
		
		// this->isNegative is true ==> (-) - (+) === (-), else (+) - (-) === (+).
		return BigInt(cRet, this->isNegative, newBLen, nullptr);
	}
	else
	{
		if(this->absGreater(nega))
		{
			unsigned char* cRet = byteWiseNegation(this->number, this->numLen, nega.number, nega.numLen, newBLen);
			
			// this->isNegative is true ==> (---) - (-) === (-), else (+++) - (+) === (+).
			return BigInt(cRet, this->isNegative, newBLen, nullptr);
		}
		else
		{
			unsigned char* cRet = byteWiseNegation(nega.number, nega.numLen, this->number, this->numLen, newBLen);
			
			// this->isNegative is true ==> (-) - (---) === (+), else (+) - (+++) === (-).
			return BigInt(cRet, !(this->isNegative), newBLen, nullptr);
		}
	}
}

unsigned char* BigInt::multiplicationUtil(const unsigned char* cand1, int len1, const unsigned char* cand2, int len2, int& newBLen)
{
	int newLen = len1 + len2;
	unsigned char* retVal = allocZerosMem(newLen + 1);
	
	unsigned char* cacheVal = allocZerosMem(newLen);
	
	unsigned short cache = 0;
	unsigned char *cPtr = castPtr<unsigned char>(&cache);
	unsigned short space = 0;
	for(int i = 0 ; i < len1 ; i++)
	{
		cache = 0;
		for(int j = 0 ; j < len2 ; j++)
		{
			space = cand1[i];
			cache += space * cand2[j];
			cacheVal[i + j] = *cPtr;
			cache >>= 8;
		}
		cacheVal[i + len2] = *cPtr;
		
		byteWiseAdditionNoCopy(cacheVal, newLen, retVal, newLen, retVal, newLen + 1);
		
		memset(cacheVal + i, 0, len2 + 1);
	}
	delete [] cacheVal;
	
	newBLen = newLen + 1;
	return retVal;
}

const BigInt BigInt::operator*(const BigInt& mult) const
{
	// One of them is zero, than the product is zero.
	if(this->numLen == 0 || mult.numLen == 0)
	{
		return BigInt(); // ZERO
	}
	
	int newBLen;
	unsigned char* retVal = multiplicationUtil(this->number, this->numLen, mult.number, mult.numLen, newBLen);
	
	return BigInt(retVal, this->isNegative ^ mult.isNegative, newBLen, nullptr);
}

// The "remain" input is the number to be divided.
void BigInt::divisionUtil(const unsigned char* divi, int diviLen, unsigned char* quotient, int qLen, unsigned char* remain)
{
	int dividersCacheLen[8];
	unsigned char* dividersCache[8];
	
	// Creates the 8 shifted caches of the divisor.
	for(int i = 0 ; i < 8 ; i++)
	{
		dividersCache[i] = shiftBits(i, divi, diviLen, dividersCacheLen[i]);

		/*
		if(dividersCache[i][dividersCacheLen[i] - 1] == 0)
		{
			dividersCacheLen[i]--;
		}
		*/
	}
	
	// The division is implemented in binary format. Each byte contains 8 bits, or 8 binary 'digits'.
	//  In binary division, the divisor is left shifted to be aligned with the head of the current
	//  remain, and checked if the value is less than the leading bytes of the remain.
	//  If true, then the remain is substracted by the shifted divisor, and the corresponding bit of
	//  the quotient is added.
	for(int i = qLen - 1 ; i >= 0 ; i--)
	{
		for(int j = 7 ; j >= 0 ; j--)
		{
			if(byteWiseGreater(remain + i, dividersCache[j], dividersCacheLen[j], true))
			{
				quotient[i] += (1 << j);
				byteWiseNegationNoCopy(remain + i, dividersCacheLen[j], dividersCache[j], dividersCacheLen[j], remain + i, dividersCacheLen[j]);
			}
		}
	}
	
	for(int i = 0 ; i < 8 ; i++)
	{
		delete [] dividersCache[i];
	}
}

unsigned char* BigInt::divisionUtil(const BigInt& divi, int& bOutLen, bool q_than_r) const // true -> q ; false -> r
{
	if(divi.absGreater(*this))
	{
		if(q_than_r)
		{
			bOutLen = 0;
			return nullptr;
		}
		else
		{
			bOutLen = this->numLen;
			unsigned char* copy = new unsigned char[this->numLen];
			memcpy(copy, this->number, this->numLen);
			return copy;
		}
	}
	else if(divi.numLen == 0) // Divided by zero.
	{
		char* error = nullptr;
		*error = 0; // Crash the program on purpose. Get rekt for dividing sth by zero. lol.
	}
	
	unsigned char* cache = new unsigned char[this->numLen + 1];
	cache[this->numLen] = 0;
	memcpy(cache, this->number, this->numLen);
	
	int qLen = this->numLen - divi.numLen + 1;
	unsigned char* quotient = allocZerosMem(qLen);
	
	divisionUtil(divi.number, divi.numLen, quotient, qLen, cache);
	
	if(q_than_r)
	{
		delete [] cache;
		bOutLen = qLen;
		return quotient;
	}
	else
	{
		delete [] quotient;
		bOutLen = this->numLen + 1;
		return cache;
	}
}

const BigInt BigInt::operator/(const BigInt& divi) const
{
	int bOutLen;
	unsigned char* retVal = this->divisionUtil(divi, bOutLen, true);
	return BigInt(retVal, this->isNegative ^ divi.isNegative, bOutLen, nullptr);
}

const BigInt BigInt::operator%(const BigInt& divi) const
{
	int bOutLen;
	unsigned char* retVal = this->divisionUtil(divi, bOutLen, false);
	return BigInt(retVal, this->isNegative, bOutLen, nullptr);
}

const BigInt& BigInt::operator+=(const BigInt& addi)
{
	if(this->numLen == 0)
	{
		this->numLen = this->byteLen = addi.numLen;
		delete [] this->number;
		this->number = new unsigned char[addi.numLen];
		memcpy(this->number, addi.number, this->byteLen);
		this->isNegative = addi.isNegative;
		return *this;
	}
	else if(addi.numLen == 0)
	{
		return *this;
	}
	
	int newBLen;
	if(this->isNegative ^ addi.isNegative)
	{
		if(this->absGreater(addi))
		{
			unsigned char* cRet = byteWiseNegation(this->number, this->numLen, addi.number, addi.numLen, newBLen);
			
			// this->isNegative is true ==> (---) + (+) === (-), else (+++) + (-) === (+).
			this->setValues(cRet, newBLen, this->isNegative);
			
			return *this;
		}
		else
		{
			unsigned char* cRet = byteWiseNegation(addi.number, addi.numLen, this->number, this->numLen, newBLen);
			
			// this->isNegative is true ==> addi is positive ==> (-) + (+++) === (+), else (+) + (---) === (-).
			this->setValues(cRet, newBLen, addi.isNegative);
			
			return *this;
		}
	}
	else
	{
		unsigned char* cRet = byteWiseAddition(this->number, this->numLen, addi.number, addi.numLen, newBLen);
		
		this->setValues(cRet, newBLen, this->isNegative);
		
		return *this;
	}
}

const BigInt& BigInt::operator-=(const BigInt& nega)
{
	if(this->numLen == 0)
	{
		this->numLen = this->byteLen = nega.numLen;
		delete [] this->number;
		this->number = new unsigned char[nega.numLen];
		memcpy(this->number, nega.number, this->byteLen);
		this->isNegative = !(nega.isNegative);
		return *this;
	}
	else if(nega.numLen == 0)
	{
		return *this;
	}
	
	int newBLen;
	if(this->isNegative ^ nega.isNegative)
	{
		unsigned char* cRet = byteWiseAddition(this->number, this->numLen, nega.number, nega.numLen, newBLen);
		
		// this->isNegative is true ==> (-) - (+) === (-), else (+) - (-) === (+).
		this->setValues(cRet, newBLen, this->isNegative);
		
		return *this;
	}
	else
	{
		if(this->absGreater(nega))
		{
			unsigned char* cRet = byteWiseNegation(this->number, this->numLen, nega.number, nega.numLen, newBLen);
			
			// this->isNegative is true ==> (---) - (-) === (-), else (+++) - (+) === (+).
			this->setValues(cRet, newBLen, this->isNegative);
			
			return *this;
		}
		else
		{
			unsigned char* cRet = byteWiseNegation(nega.number, nega.numLen, this->number, this->numLen, newBLen);
			
			// this->isNegative is true ==> (-) - (---) === (+), else (+) - (+++) === (-).
			this->setValues(cRet, newBLen, !(this->isNegative));
			
			return *this;
		}
	}
}

const BigInt& BigInt::operator*=(const BigInt& mult)
{
	if(this->numLen == 0)
	{
		return *this; // ZERO
	}
	else if(mult.numLen == 0)
	{
		this->byteLen = this->numLen = 0;
		delete [] this->number;
		this->number = nullptr;
		return *this;
	}
	
	int newBLen;
	unsigned char* retVal = multiplicationUtil(this->number, this->numLen, mult.number, mult.numLen, newBLen);
	this->setValues(retVal, newBLen, this->isNegative ^ mult.isNegative);
	return *this;
}

const BigInt& BigInt::operator/=(const BigInt& divi)
{
	int bOutLen;
	unsigned char* retVal = this->divisionUtil(divi, bOutLen, true);
	this->setValues(retVal, bOutLen, this->isNegative ^ divi.isNegative);
	return *this;
}

const BigInt& BigInt::operator%=(const BigInt& divi)
{
	int bOutLen;
	unsigned char* retVal = this->divisionUtil(divi, bOutLen, false);
	this->setValues(retVal, bOutLen, this->isNegative);
	return *this;
}

const BigInt& BigInt::operator++()
{
	static const unsigned char *ONE = new unsigned char(1);
	
	if(this->numLen == 0) // zero
	{
		this->byteLen = this->numLen = 1;
		delete [] this->number;
		this->number = new unsigned char[1];
		this->number[0] = 1;
		this->isNegative = false;
	}
	else
	{
		if(this->isNegative)
		{
			byteWiseNegationNoCopy(this->number, this->numLen, ONE, 1, this->number, this->numLen);
		
			// When negating only by 1, we'd check if the leading byte is zero after the operation.
			//  If it's zero, it means that the carry went all the way from the 0th order to the leading byte,
			//  and "removed" that byte. Just like "010000 - 1 = 00FFFF".
			if(this->number[this->numLen - 1] == 0)
			{
				this->numLen--;
			}
		}
		else
		{
			byteWiseAdditionNoCopy(this->number, this->numLen, ONE, 1, this->number, this->numLen);
		
			// When adding only by 1, we'd check if the leading byte is zero after the operation.
			//  If it's zero, it means that the carry went all the way from the 0th order to the leading byte,
			//  and "overflow"ed that byte. Just like "FFFF + 1 = (01)0000". The "(01)" is the overflow.
			if(this->number[this->numLen - 1] == 0)
			{
				// Extra space available
				//  -> Do not create a new memory chunk and do crazy stuffs.
				if(this->byteLen > this->numLen)
				{
					this->number[this->numLen] = 1;
					this->numLen++;
				}
				else
				{
					unsigned char *old = this->number;
					this->byteLen = ++(this->numLen); // ++nl, NOT nl++ !!!!!
					this->number = new unsigned char[this->byteLen];
					memcpy(this->number, old, this->numLen - 1); // copy all the zeros. :>
					this->number[this->numLen - 1] = 1;
				}
			}
		}
	}
	return *this;
}

const BigInt& BigInt::operator--()
{
	static const unsigned char *ONE = new unsigned char(1);
	
	if(this->numLen == 0) // zero
	{
		this->byteLen = this->numLen = 1;
		delete [] this->number;
		this->number = new unsigned char[1];
		this->number[0] = 1;
		this->isNegative = true; // Set to -1;
	}
	else
	{
		if(this->isNegative)
		{
			byteWiseAdditionNoCopy(this->number, this->numLen, ONE, 1, this->number, this->numLen);
			if(this->number[this->numLen - 1] == 0)
			{
				// Extra space available
				//  -> Do not create a new memory chunk and do crazy stuffs.
				if(this->byteLen > this->numLen)
				{
					this->number[this->numLen] = 1;
					this->numLen++;
				}
				else
				{
					unsigned char *old = this->number;
					this->byteLen = ++(this->numLen); // ++nl, NOT nl++ !!!!!
					this->number = new unsigned char[this->numLen];
					memcpy(this->number, old, this->numLen - 1);
					this->number[this->numLen - 1] = 1;
				}
			}
		}
		else
		{
			byteWiseNegationNoCopy(this->number, this->numLen, ONE, 1, this->number, this->numLen);
			if(this->number[this->numLen - 1] == 0)
			{
				this->numLen--;
			}
		}
	}
	return *this;
}

void BigInt::operator++(int)
{
	++(*this);
}

void BigInt::operator--(int)
{
	--(*this);
}

bool BigInt::operator==(const BigInt& comp) const
{
	if(this->numLen == comp.numLen && this->isNegative == comp.isNegative)
	{
		if(this->numLen == 0)
		{
			return true;
		}
		for(int i = 0 ; i < this->numLen ; i++)
		{
			if(this->number[i] != comp.number[i])
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

bool BigInt::operator!=(const BigInt& comp) const
{
	return !(*this == comp);
}

bool BigInt::operator>=(const BigInt& comp) const
{
	if(this->isNegative ^ comp.isNegative)
	{
		// comp.isNegative == true --> "this" is positive.
		return comp.isNegative;
	}
	else if(this->numLen != comp.numLen)
	{
		// Both negative -> if "this" is "shorter". Otherwise, reverse.
		return (this->isNegative ^ (this->numLen > comp.numLen));
	}
	else
	{
		return byteWiseGreater(this->number, comp.number, this->numLen, true);
	}
}

bool BigInt::operator<=(const BigInt& comp) const
{
	return (comp >= *this);
}

bool BigInt::operator>(const BigInt& comp) const
{
	if(this->isNegative ^ comp.isNegative)
	{
		// comp.isNegative == true --> "this" is positive.
		return comp.isNegative;
	}
	else if(this->numLen != comp.numLen)
	{
		// Both positive -> if "this" is longer. Otherwise, reverse.
		return (this->isNegative ^ (this->numLen > comp.numLen));
	}
	else
	{
		return byteWiseGreater(this->number, comp.number, this->numLen, false);
	}
}

bool BigInt::operator<(const BigInt& comp) const
{
	return (comp > *this);
}

const BigInt BigInt::operator<<(const int offset) const
{
	if(offset <= 0 || this->numLen == 0)
	{
		return *this;
	}
	
	int newLen;
	unsigned char *retVal = shiftBits(offset, this->number, this->numLen, newLen);
	
	return BigInt(retVal, this->isNegative, newLen, nullptr);
}

const BigInt BigInt::operator>>(const int offset) const
{
	if(offset <= 0 || this->numLen == 0)
	{
		return *this;
	}
	
	int newLen;
	unsigned char *retVal = shiftBits(-offset, this->number, this->numLen, newLen);
	
	return BigInt(retVal, this->isNegative, newLen, nullptr);
}

const BigInt BigInt::square() const
{
	return *this * *this;
}

// Integer square root implemented in binary format.
//  It's similar to manual-sqrt. taughted in the school,
//  but in binary. :)
const BigInt BigInt::sqrt(bool ignoreNegative) const
{
	if(!ignoreNegative && this->isNegative)
	{
		// lol sqrt(-1)?
		return BigInt();
	}
	
	unsigned char* copy = new unsigned char[this->numLen + 1];
	copy[this->numLen] = 0;
	memcpy(copy, this->number, this->numLen);
	
	// Each "chunk" contains two bits, just like manual-sqrt. grouping the digits by 2.
	//  Thus, each byte is seperated into 4 chunks.
	int leadingSqrtBit;
	char c;
	for(leadingSqrtBit = 3 ; leadingSqrtBit > 0 ; leadingSqrtBit--)
	{
		if(copy[this->numLen - 1] >= (c = (1 << (2 * leadingSqrtBit))))
		{
			// Now the "leadingSqrtBit" is the bit wise 'index' of the leading byte.
			break;
		}
	}
	
	int sqrtBinLen = (this->numLen - 1) * 4 + leadingSqrtBit;
	int sqrtHexLen = (this->numLen + 1) / 2;
	
	unsigned char* sqrtCache = allocZerosMem(sqrtHexLen + 1);
	
	// Adding 2 is just for convience. Or runtime errors may sometimes occur. D:
	unsigned char* multCache = new unsigned char[this->numLen + 2];
	
	unsigned char* retVal = allocZerosMem(sqrtHexLen);
	
	int byteIndex = sqrtBinLen / 8;
	int bitOffset = sqrtBinLen % 8;
	int cacheLen, cacheShift, retIterLen;
	unsigned char byteVal;
	
	unsigned short cache = 0;
	unsigned char* cPtr = castPtr<unsigned char>(&cache);
	unsigned short space = 0;
	
	while(byteIndex >= 0)
	{
		while(bitOffset >= 0)
		{
			byteVal = 1 << bitOffset;
		
			sqrtCache[byteIndex] += byteVal;
		
			cache = 0;
			for(int j = byteIndex ; j <= sqrtHexLen + 1 ; j++)
			{
				space = sqrtCache[j];
				cache += space * byteVal;
				multCache[byteIndex + j] = *cPtr;
				cache >>= 8;
			}
			
			cacheLen = this->numLen - 2 * byteIndex;
			cacheShift = 2 * byteIndex;
			if(byteWiseGreater(copy + cacheShift, multCache + cacheShift, cacheLen, true))
			{
				byteWiseNegationNoCopy(copy + cacheShift, cacheLen, multCache + cacheShift, cacheLen, copy + cacheShift, cacheLen);
				byteWiseAdditionNoCopy(sqrtCache + byteIndex, 2, &byteVal, 1, sqrtCache + byteIndex, 2);
				retVal[byteIndex] += byteVal;
			}
			else
			{
				sqrtCache[byteIndex] -= byteVal;
			}
			
			bitOffset--;
		}
		byteIndex--;
		bitOffset += 8;
	}
	
	delete [] copy;
	delete [] sqrtCache;
	delete [] multCache;
	
	return BigInt(retVal, false, sqrtHexLen, nullptr);
}

bool BigInt::isPrime() const
{
	static const unsigned char* TWO = new unsigned char(2);
	
	if(this->numLen == 0)
	{
		return false;
	}
	else if(this->numLen == 1)
	{
		if(this->number[0] == 1)
		{
			return false;
		}
		else if(this->number[0] == 2)
		{
			return true;
		}
	}
	
	if(this->number[0] % 2 == 0)
	{
		return false;
	}
	
	const BigInt bi = this->sqrt(true);
	unsigned char* cache = allocZerosMem(bi.numLen + 1);
	cache[0] = 3;
	int cacheLen = 1;
	int qLen = this->numLen - cacheLen + 1;
	
	unsigned char* copy = new unsigned char[this->numLen + 1];
	unsigned char* ignored = new unsigned char[this->numLen];
	
	bool fullyDivided = false;
	while(byteWiseGreater(bi.number, cache, bi.numLen, true))
	{
		copy[this->numLen] = 0;
		memcpy(copy, this->number, this->numLen);
		divisionUtil(cache, cacheLen, ignored, qLen, copy);
		
		fullyDivided = true;
		for(int i = 0 ; i < cacheLen ; i++)
		{
			if(copy[i] != 0)
			{
				fullyDivided = false;
				break;
			}
		}
		
		if(fullyDivided)
		{
			break;
		}
		
		byteWiseAdditionNoCopy(cache, cacheLen, TWO, 1, cache, cacheLen + 1);
		if(cache[cacheLen] != 0)
		{
			// carry!!;
			cacheLen++;
			qLen--;
		}
	}
	
	delete [] cache;
	delete [] copy;
	delete [] ignored;
	
	return !fullyDivided;
}

char* BigInt::getHexString() const
{
	if(this->numLen == 0)
	{
		char *ret = new char[3];
		ret[0] = ret[1] = '0';
		ret[2] = 0;
		return ret;
	}
	
	static const char digits[] = "0123456789ABCDEF";
	
	const int retLen = this->numLen * 2 + (this->isNegative ? 1 : 0) + 1;
	
	char* ret = new char[retLen];
	ret[retLen - 1] = 0;
	
	for(int i = 0 ; i < this->numLen ; i++)
	{
		ret[retLen - 2 * i - 2] = digits[this->number[i] % 16];
		ret[retLen - 2 * i - 3] = digits[this->number[i] / 16];
	}
	
	if(this->isNegative)
	{
		ret[0] = '-';
	}
	
	return ret;
}

unsigned char** BigInt::CREATE_TENS()
{
	unsigned char** ret = new unsigned char*[8];
	static unsigned short* temp = nullptr;
	for(int i = 0 ; i < 8 ; i++)
	{
		ret[i] = new unsigned char[2];
		
		temp = castPtr<unsigned short>(ret[i]);
		*temp = (10 << i);
	}
	
	return ret;
}

char* BigInt::getDecimalString() const
{
	static const unsigned char* const* TENS = CREATE_TENS();
	
	if(this->numLen == 0)
	{
		char* ret = new char[2];
		ret[0] = '0';
		ret[1] = 0;
		return ret;
	}
	
	// {[Length of x(256)] + 1} * log_10(256) gives the length upper bound of the number is decimal.
	// The +1 at the tail is for the null character, +2 is for the negative sign.
	int outLen = (this->numLen + 1) * 2.40824 + (this->isNegative ? 2 : 1);
	int outIndex = outLen - 2;
	char* _ret = new char[outLen];
	_ret[outLen - 1] = 0; //null character at the end of the string.
	
	unsigned char* quotient = allocZerosMem(this->numLen + 1);
	unsigned char* cache = new unsigned char[this->numLen + 1];
	cache[this->numLen] = 0;
	memcpy(cache, this->number, this->numLen);
	
	static unsigned char* swap = nullptr;
	
	int qLen = this->numLen;

	while(qLen > 0)
	{
		bool leadingZero = true;
		for(int i = qLen - 1 ; i >= 0 ; i--)
		{
			for(int j = 7 ; j >= 0 ; j--)
			{
				if(byteWiseGreater(cache + i, TENS[j], 2, true))
				{
					leadingZero = false;
					quotient[i] += (1 << j);
					byteWiseNegationNoCopy(cache + i, 2, TENS[j], 2, cache + i, 2);
				}
			}
			if(leadingZero)
			{
				qLen--;
			}
		}
		
		_ret[outIndex] = cache[0] + '0';
		cache[0] = 0;
		outIndex--;
		
		swap = cache;
		cache = quotient;
		quotient = swap;
	}
	
	if(this->isNegative)
	{
		_ret[outIndex] = '-';
		outIndex--;
	}
	
	delete [] quotient;
	delete [] cache;
	
	int shift = outIndex + 1;
	outLen -= shift;
	char* ret = new char[outLen];
	memcpy(ret, _ret + shift, outLen);
	delete [] _ret;
	return ret;
}

short BigInt::operator[](const int index) const
{
	static const unsigned char* const* TENS = CREATE_TENS();
	
	int decLen = (this->numLen + 1) * 2.40824 + 1;
	
	if(index > decLen || index < 0)
	{
		return -1;
	}
	
	unsigned char* quotient = allocZerosMem(this->numLen + 1);
	unsigned char* cache = new unsigned char[this->numLen + 1];
	cache[this->numLen] = 0;
	memcpy(cache, this->number, this->numLen);
	
	static unsigned char* switcher = nullptr;
	
	int qLen = this->numLen;

	int indexCount = 0;
	short ret = -1;
	while(qLen > 0)
	{
		bool leadingZero = true;
		for(int i = qLen - 1 ; i >= 0 ; i--)
		{
			for(int j = 7 ; j >= 0 ; j--)
			{
				if(byteWiseGreater(cache + i, TENS[j], 2, true))
				{
					leadingZero = false;
					quotient[i] += (1 << j);
					byteWiseNegationNoCopy(cache + i, 2, TENS[j], 2, cache + i, 2);
				}
			}
			if(leadingZero)
			{
				qLen--;
			}
		}
		
		if(indexCount == index)
		{
			ret = cache[0];
			break;
		}
		
		cache[0] = 0;
		switcher = cache;
		cache = quotient;
		quotient = switcher;
		
		indexCount++;
	}
	
	delete [] quotient;
	delete [] cache;
	return ret;
}

unsigned char* BigInt::createFromDecimal(const char* decimalString, int len, bool& isNeg, int& retLen)
{
	isNeg = (decimalString[0] == '-');
	retLen = (len + (isNeg ? 0 : 1)) / 2;

	unsigned char* retVal = allocZerosMem(retLen);
	
	unsigned char* digit = new unsigned char(0);
	
	unsigned short cache = 0;
	unsigned char* cPtr = castPtr<unsigned char>(&cache);
	
	unsigned short space = 0;
	
	for(int i = (isNeg ? 1 : 0) ; i < len ; i++)
	{
		cache = 0;
		for(int j = 0 ; j < retLen ; j++)
		{
			space = retVal[j];
			cache += space * 10;
			retVal[j] = *cPtr;
			cache >>= 8;
		}
		
		if((*digit = (decimalString[i] - '0')) >= 10)
		{
			delete [] retVal;
			isNeg = false;
			retLen = 0;
			return nullptr; // error (invalid input character) -> return 0.
		}
		byteWiseAdditionNoCopy(retVal, retLen, digit, 1, retVal, retLen);
	}
	
	return retVal;
}

BigInt BigInt::createFromDecimal(const char* decimalString, int len)
{
	int retLen;
	bool isNeg;
	unsigned char* retVal = createFromDecimal(decimalString, len, isNeg, retLen);
	return BigInt(retVal, isNeg, retLen, nullptr);
}

BigInt BigInt::createFromDecimal(const char* decimalString)
{
	return createFromDecimal(decimalString, strlen(decimalString));
}

BigInt* BigInt::createFromDecimal(const char* decimalString, int len, void* dummy)
{
	int retLen;
	bool isNeg;
	unsigned char* retVal = createFromDecimal(decimalString, len, isNeg, retLen);
	return new BigInt(retVal, isNeg, retLen, nullptr);
}

BigInt* BigInt::createFromDecimal(const char* decimalString, void* dummy)
{
	return createFromDecimal(decimalString, strlen(decimalString), dummy);
}

BigInt::BigInt(const int& copyFrom)
{
	int* iPtr = new int(copyFrom);
	this->isNegative = (*iPtr < 0);
	if(this->isNegative)
	{
		*iPtr = -(*iPtr);
	}
	this->byteLen = sizeof(int);
	this->number = castPtr<unsigned char>(iPtr);
	for(int i = this->byteLen ; i > 0 ; i--)
	{
		if(this->number[i - 1] != 0)
		{
			this->numLen = i;
			return;
		}
	}
	this->numLen = 0;
}

const BigInt BigInt::operator+(const int& rhs) const
{
	return *this + BigInt(rhs);
}

const BigInt BigInt::operator-(const int& rhs) const
{
	return *this - BigInt(rhs);
}

const BigInt BigInt::operator*(const int& rhs) const
{
	return *this * BigInt(rhs);
}

const BigInt BigInt::operator/(const int& rhs) const
{
	return *this / BigInt(rhs);
}

const BigInt BigInt::operator%(const int& rhs) const
{
	return *this % BigInt(rhs);
}

const unsigned char * BigInt::getRawBytes() const
{
	return this->number;
}

const BigInt operator+(const int& lhs, const BigInt& _this)
{
	return BigInt(lhs) + _this;
}

const BigInt operator-(const int& lhs, const BigInt& _this)
{
	return BigInt(lhs) - _this;
}

const BigInt operator*(const int& lhs, const BigInt& _this)
{
	return BigInt(lhs) * _this;
}

const BigInt operator/(const int& lhs, const BigInt& _this)
{
	return BigInt(lhs) / _this;
}

const BigInt operator%(const int& lhs, const BigInt& _this)
{
	return BigInt(lhs) % _this;
}
