#include "headers/qw_ulong.hpp"
#include "headers/qw_long.hpp"

#include <cstdio>
#include <string>
#include <vector>
#include <stdexcept>
#include <immintrin.h>

    //Constructors:

qw_ulong::qw_ulong(){
    digits.resize(1);
}

qw_ulong::qw_ulong(std::vector<QW_BIGINT_DIGITTYPE> value){
    digits = value;
}

qw_ulong::qw_ulong(QW_BIGINT_DIGITTYPE value){
    digits.resize(1);
    digits[0] = value;
}

qw_ulong::qw_ulong(qw_long& value){
    *this = value.getDigits();
}

    //Asignment operators:

qw_ulong& qw_ulong::operator=(const QW_BIGINT_DIGITTYPE value){
    digits.resize(1);
    digits[0] = value;
    return *this;
}

qw_ulong& qw_ulong::operator=(const std::vector<QW_BIGINT_DIGITTYPE>& value){
    digits = value;
    return *this;
}

qw_ulong& qw_ulong::operator=(const qw_long& value){
    *this = value.getDigits();
    return *this;
}

qw_ulong& qw_ulong::operator+=(const qw_ulong& value){
    *this = *this + value;
    return *this;
}

qw_ulong& qw_ulong::operator-=(const qw_ulong& value){
    *this = *this - value;
    return *this;
}

qw_ulong& qw_ulong::operator*=(const qw_ulong& value){
    *this = *this * value;
    return *this;
}

qw_ulong& qw_ulong::operator/=(const qw_ulong& value){
    *this = *this / value;
    return *this;
}

qw_ulong& qw_ulong::operator%=(const qw_ulong& value){
    *this = *this % value;
    return *this;
}

qw_ulong& qw_ulong::operator<<=(const int& value){
    *this = *this << value;
    return *this;
}

qw_ulong& qw_ulong::operator>>=(const int& value){
    *this = *this >> value;
    return *this;
}

qw_ulong& qw_ulong::operator&=(const qw_ulong& value){
    *this = *this & value;
    return *this;
}

qw_ulong& qw_ulong::operator|=(const qw_ulong& value){
    *this = *this | value;
    return *this;
}

qw_ulong& qw_ulong::operator^=(const qw_ulong& value){
    *this = *this ^ value;
    return *this;
}

    //Arithmetic operators:

qw_ulong& qw_ulong::operator++(){
    size_t size = this->size();
    int i;
    int carry = 1;
    for(i=0; carry && i<size; i++){
        digits[i] += carry;
        carry = !digits[i];
    }
    if(carry)
        digits.push_back(carry);
    return *this;
}

qw_ulong& qw_ulong::operator--(){
    size_t size = this->size();
    if(size == 1 && digits[0] == 0)
        return *this;
    int i;
    int carry = 1;
    for(i=0; carry && i<size; i++){
        digits[i] -= carry;
        carry = !(digits[i]+1);
    }
    downsize();
    return *this;
}

const qw_ulong qw_ulong::operator++(int){
    qw_ulong copy = *this;
    ++(*this);
    return copy;
}

const qw_ulong qw_ulong::operator--(int){
    qw_ulong copy = *this;
    --(*this);
    return copy;
}

qw_ulong operator+(const qw_ulong& lhs, const qw_ulong& rhs){
    size_t lhsSize = lhs.digits.size();
    size_t rhsSize = rhs.digits.size();
    const std::vector<QW_BIGINT_DIGITTYPE> *longOne, *shortOne;
    size_t longSize, shortSize;
    if(lhsSize>rhsSize){
        longSize = lhsSize;
        shortSize = rhsSize;
        longOne = &(lhs.digits);
        shortOne = &(rhs.digits);
    }else{
        longSize = rhsSize;
        shortSize = lhsSize;
        longOne = &(rhs.digits);
        shortOne = &(lhs.digits);
    }

    char carry = 0;
    qw_ulong result;
    result.digits.resize(longSize+1);
    int i;
    for(i=0; i<shortSize; i++){
        carry = _addcarryx_u64(carry, (*longOne)[i], (*shortOne)[i], &result.digits[i]); //64 bit
    }
    for(; i<longSize; i++){
        carry = _addcarryx_u64(carry, (*longOne)[i], 0, &result.digits[i]); //64 bit
    }
    if(carry && i==longSize)
        result.digits.push_back(carry);
    return result;
}

qw_ulong operator-(const qw_ulong& lhs, const qw_ulong& rhs){
    if(lhs<=rhs){
        return 0;
    }
    size_t lhsSize = lhs.digits.size();
    size_t rhsSize = rhs.digits.size();

    QW_BIGINT_DIGITTYPE sum;
    char carry = 0;
    qw_ulong result;
    result.digits.resize(lhsSize);
    int i;
    for(i=0; i<rhsSize; i++){
        carry = _subborrow_u64(carry, lhs.digits[i], rhs.digits[i], &result.digits[i]); //64 bit
    }
    for(; i<lhsSize; i++){
        carry = _subborrow_u64(carry, lhs.digits[i], 0, &result.digits[i]); //64 bit
    }
    result.downsize();
    return result;
}

qw_ulong operator*(const qw_ulong& lhs, const qw_ulong& rhs){
    size_t lhsSize = lhs.digits.size();
    size_t rhsSize = rhs.digits.size();

    QW_BIGINT_DIGITTYPE digitProduct;
    qw_ulong result;
    qw_ulong partialResult;
    int i, j;
    for(i=0; i<lhsSize; i++){
        partialResult = 0;
        partialResult.digits.resize(lhsSize+rhsSize);
        for(j=0; j<rhsSize; j++){
            digitProduct = _mulx_u64(lhs.digits[i], rhs.digits[j], &partialResult.digits[i+j+1]); //64 bit //needs -mbmi2
            partialResult.digits[i+j+1] += _addcarryx_u64(0, partialResult.digits[i+j], digitProduct, &partialResult.digits[i+j]); //64 bit
        }
        result += partialResult;
    }
    result.downsize();
    return result;
}

qw_ulong operator/(const qw_ulong& lhs, const qw_ulong& rhs){
    if(rhs==0){
        throw std::invalid_argument("Divide by zero error");
    }
    qw_ulong divident = lhs;
    qw_ulong divisor = rhs;
    qw_ulong result;
    result.digits.resize(lhs.digits.size());
    size_t shift = lhs.msb() - rhs.msb();
    while(divident >= rhs){
        divisor = rhs << shift;
        if(divisor>divident){
            shift = divident.msb() - rhs.msb() - 1;
            continue;
        }
        divident -= divisor;
        result.digits[shift/QW_BIGINT_DIGITSIZE] |= (QW_BIGINT_DIGITTYPE)1 << (shift%QW_BIGINT_DIGITSIZE);
        shift = divident.msb() - rhs.msb();
    }
    result.downsize();
    return result;
}

qw_ulong operator%(const qw_ulong& lhs, const qw_ulong& rhs){
    if(rhs==0){
        throw std::invalid_argument("Remainder by zero error");
    }
    qw_ulong divident = lhs;
    qw_ulong divisor = rhs;
    size_t shift = lhs.msb() - rhs.msb();
    while(divident >= rhs){
        divisor = rhs << shift;
        if(divisor>divident){
            shift = divident.msb() - rhs.msb() - 1;
            continue;
        }
        divident -= divisor;
        shift = divident.msb() - rhs.msb();
    }
    return divident;
}

    //Bitwise operators:

qw_ulong operator<<(const qw_ulong& original, const int shiftAmount){
    if(shiftAmount < 0){
        return original >> -shiftAmount;
    }
    size_t oldSize = original.digits.size();
    int bigShift = shiftAmount / QW_BIGINT_DIGITSIZE;
    int smallShift = shiftAmount % QW_BIGINT_DIGITSIZE;

    size_t newSize = oldSize + bigShift;
    qw_ulong result;
    int i;
    if(!smallShift){
        result.digits.resize(newSize);
        for(i=0; i<oldSize; i++){
            result.digits[i+bigShift] = original.digits[i];
        }
    }else{
        result.digits.resize(newSize+1);
        for(i=0; i<oldSize; i++){
            result.digits[i+bigShift] |= original.digits[i] << smallShift;
            result.digits[i+bigShift+1] = original.digits[i] >> (QW_BIGINT_DIGITSIZE-smallShift);
        }
        result.downsize();
    }
    return result;
}

qw_ulong operator>>(const qw_ulong& original, const int shiftAmount){
    if(shiftAmount < 0){
        return original << -shiftAmount;
    }
    size_t oldSize = original.digits.size();
    int bigShift =  shiftAmount / QW_BIGINT_DIGITSIZE;
    int smallShift =  shiftAmount % QW_BIGINT_DIGITSIZE;

    size_t newSize = oldSize - bigShift;
    qw_ulong result;
    int i;
    if(!smallShift){
        result.digits.resize(newSize);
        for(i=bigShift; i<oldSize; i++){
            result.digits[i-bigShift] = original.digits[i];
        }
    }else{
        result.digits.resize(newSize+1);
        i = bigShift;
        result.digits[i-bigShift] = original.digits[i] >> smallShift;
        for(i++; i<oldSize; i++){
            result.digits[i-bigShift] = original.digits[i] >> smallShift;
            result.digits[i-bigShift-1] |= original.digits[i] << (QW_BIGINT_DIGITSIZE-smallShift);
        }
        result.downsize();
    }
    return result;
}

qw_ulong operator&(const qw_ulong& lhs, const qw_ulong& rhs){
    size_t lhsSize = lhs.digits.size();
    size_t rhsSize = rhs.digits.size();
    size_t size = std::min(lhsSize, rhsSize);

    qw_ulong result;
    result.digits.resize(size);
    int i;
    for(i=0; i<size; i++){
        result.digits[i] = lhs.digits[i] & rhs.digits[i];
    }
    result.downsize();
    return result;
}

qw_ulong operator|(const qw_ulong& lhs, const qw_ulong& rhs){
    size_t lhsSize = lhs.digits.size();
    size_t rhsSize = rhs.digits.size();
    const std::vector<QW_BIGINT_DIGITTYPE> *longOne, *shortOne;
    size_t longSize, shortSize;
    if(lhsSize>rhsSize){
        longSize = lhsSize;
        shortSize = rhsSize;
        longOne = &(lhs.digits);
        shortOne = &(rhs.digits);
    }else{
        longSize = rhsSize;
        shortSize = lhsSize;
        longOne = &(rhs.digits);
        shortOne = &(lhs.digits);
    }

    qw_ulong result;
    result.digits.resize(longSize);
    int i;
    for(i=0; i<shortSize; i++){
        result.digits[i] = (*shortOne)[i] | (*longOne)[i];
    }
    for(; i<longSize; i++){
        result.digits[i] = (*longOne)[i];
    }
    return result;
}

qw_ulong operator^(const qw_ulong& lhs, const qw_ulong& rhs){
    size_t lhsSize = lhs.digits.size();
    size_t rhsSize = rhs.digits.size();
    const std::vector<QW_BIGINT_DIGITTYPE> *longOne, *shortOne;
    size_t longSize, shortSize;
    if(lhsSize>rhsSize){
        longSize = lhsSize;
        shortSize = rhsSize;
        longOne = &(lhs.digits);
        shortOne = &(rhs.digits);
    }else{
        longSize = rhsSize;
        shortSize = lhsSize;
        longOne = &(rhs.digits);
        shortOne = &(lhs.digits);
    }

    qw_ulong result;
    result.digits.resize(longSize);
    int i;
    for(i=0; i<shortSize; i++){
        result.digits[i] = (*shortOne)[i] ^ (*longOne)[i];
    }
    for(; i<longSize; i++){
        result.digits[i] = (*longOne)[i];
    }
    result.downsize();
    return result;
}

qw_ulong operator~(const qw_ulong& original){
    if(original == 0)
        return 0;
    size_t size = original.digits.size();

    qw_ulong result;
    result.digits.resize(size);
    int i;
    for(i=0; i<size; i++){
        result.digits[i] = ~original.digits[i];
    }
    for(i=QW_BIGINT_DIGITSIZE-1; (result.digits[size-1]>>i&1); i--) {}
    result.digits[size-1] &= QW_BIGINT_DIGITMAX >> (QW_BIGINT_DIGITSIZE-i-1);
    result.downsize();
    return result;
}

    //Comparison operators:

bool operator==(const qw_ulong& lhs, const qw_ulong& rhs){
    size_t lhsSize = lhs.digits.size();
    size_t rhsSize = rhs.digits.size();
    if(lhsSize != rhsSize)
        return 0;
    int i;
    for(i=0; i<lhsSize; i++)
        if(lhs.digits[i] != rhs.digits[i])
            return 0;
    return 1;
}

bool operator!=(const qw_ulong& lhs, const qw_ulong& rhs){
    size_t lhsSize = lhs.digits.size();
    size_t rhsSize = rhs.digits.size();
    if(lhsSize != rhsSize)
        return 1;
    int i;
    for(i=0; i<lhsSize; i++)
        if(lhs.digits[i] != rhs.digits[i])
            return 1;
    return 0;
}

bool operator>(const qw_ulong& lhs, const qw_ulong& rhs){
    size_t lhsSize = lhs.digits.size();
    size_t rhsSize = rhs.digits.size();
    if(lhsSize > rhsSize)
        return 1;
    if(lhsSize < rhsSize)
        return 0;
    int i;
    for(i=lhsSize-1; i>=0; i--)
        if(lhs.digits[i] > rhs.digits[i])
            return 1;
        else if(lhs.digits[i] < rhs.digits[i])
            return 0;
    return 0;
}

bool operator<(const qw_ulong& lhs, const qw_ulong& rhs){
    size_t lhsSize = lhs.digits.size();
    size_t rhsSize = rhs.digits.size();
    if(lhsSize < rhsSize)
        return 1;
    if(lhsSize > rhsSize)
        return 0;
    int i;
    for(i=lhsSize-1; i>=0; i--)
        if(lhs.digits[i] < rhs.digits[i])
            return 1;
        else if(lhs.digits[i] > rhs.digits[i])
            return 0;
    return 0;
}

bool operator>=(const qw_ulong& lhs, const qw_ulong& rhs){
    size_t lhsSize = lhs.digits.size();
    size_t rhsSize = rhs.digits.size();
    if(lhsSize > rhsSize)
        return 1;
    if(lhsSize < rhsSize)
        return 0;
    int i;
    for(i=lhsSize-1; i>=0; i--)
        if(lhs.digits[i] > rhs.digits[i])
            return 1;
        else if(lhs.digits[i] < rhs.digits[i])
            return 0;
    return 1;
}

bool operator<=(const qw_ulong& lhs, const qw_ulong& rhs){
    size_t lhsSize = lhs.digits.size();
    size_t rhsSize = rhs.digits.size();
    if(lhsSize < rhsSize)
        return 1;
    if(lhsSize > rhsSize)
        return 0;
    int i;
    for(i=lhsSize-1; i>=0; i--)
        if(lhs.digits[i] < rhs.digits[i])
            return 1;
        else if(lhs.digits[i] > rhs.digits[i])
            return 0;
    return 1;
}

    //Cast operators:

qw_ulong::operator bool(){
    return (digits.size()!=1) || (digits[0]!=0);
}

qw_ulong::operator char(){return digits[0];}
qw_ulong::operator short(){return digits[0];}
qw_ulong::operator int(){return digits[0];}
qw_ulong::operator long(){return digits[0];}
qw_ulong::operator long long(){return digits[0];}
qw_ulong::operator unsigned char(){return digits[0];}
qw_ulong::operator unsigned short(){return digits[0];}
qw_ulong::operator unsigned int(){return digits[0];}
qw_ulong::operator unsigned long(){return digits[0];}
qw_ulong::operator unsigned long long(){return digits[0];}

qw_ulong::operator std::vector<QW_BIGINT_DIGITTYPE>(){return digits;}

    //Other methods:

size_t qw_ulong::size() const{
    return digits.size();
}

size_t qw_ulong::msb() const{
    size_t msb = 0;                   //most significant bit
    size_t msd = digits.size() - 1;   //most significant digit
    QW_BIGINT_DIGITTYPE digit = digits[msd];
    while(digit){
        msb++;
        digit >>= 1;
    }
    return msd*QW_BIGINT_DIGITSIZE + msb;
}

void qw_ulong::downsize(){
    size_t size = digits.size();
    int i;
    for(i=size; i>0; i--)
        if(digits[i-1])
            break;
    if(i==0)
        i = 1;
    digits.resize(i);
}

static qw_ulong gcd(const qw_ulong& const_a, const qw_ulong& const_b){
    if(const_a == 0 && const_b == 0)
        return 0;
    if(const_a == 0)
        return const_b;
    if(const_b == 0)
        return const_a;
    qw_ulong a = const_a;
    qw_ulong b = const_b;
    while(a!=0 && b!=0){
        if(a > b)
            a %= b;
        else
            b %= a;
    }
    return a==0 ? b : a;
}

static qw_ulong lcm(const qw_ulong& const_a, const qw_ulong& const_b){
    return const_a*const_b / gcd(const_a,const_b);
}

void qw_ulong::divmod(const qw_ulong& lhs, const qw_ulong& rhs, qw_ulong& quotient, qw_ulong& remainder){
    if(rhs==0){
        throw std::invalid_argument("Divide/remainder by zero error");
    }
    qw_ulong divident = lhs;
    qw_ulong divisor = rhs;
    qw_ulong result;
    result.digits.resize(lhs.digits.size());
    size_t shift = lhs.msb() - rhs.msb();
    while(divident >= rhs){
        divisor = rhs << shift;
        if(divisor>divident){
            shift = divident.msb() - rhs.msb() - 1;
            continue;
        }
        divident -= divisor;
        result.digits[shift/QW_BIGINT_DIGITSIZE] |= (QW_BIGINT_DIGITTYPE)1 << (shift%QW_BIGINT_DIGITSIZE);
        shift = divident.msb() - rhs.msb();
    }
    result.downsize();
    divident.downsize();
    quotient = result;
    remainder = divident;
}

std::string qw_ulong::toHex(int spacing, char separator, bool truncate) const{
    std::string hexRepresentstionRaw;
    size_t size = digits.size();
    int i, j;
    for(i=0; i<size; i++)
        for (j=0; j<QW_BIGINT_DIGITSIZE/4; j++)
            hexRepresentstionRaw += (digits[i]>>j*4) & 0xF;

    std::string hexRepresentstion;
    size = hexRepresentstionRaw.size();
    i = size - 1;
    if(truncate)
        for(; hexRepresentstionRaw[i]==0 && i>0; i--){}
    for(; i>=0; i--){
        if(hexRepresentstionRaw[i] < 10)
            hexRepresentstion += (hexRepresentstionRaw[i] + '0');
        else
            hexRepresentstion += (hexRepresentstionRaw[i] + 'A' - 10);
        if(spacing && !(i%spacing) && i)
            hexRepresentstion += separator;
    }
    return hexRepresentstion;
}

std::string qw_ulong::toBin(int spacing, char separator, bool truncate) const{
    std::string binRepresentstionRaw;
    size_t size = digits.size();
    int i, j;
    for(i=0; i<size; i++)
        for(j=0; j<QW_BIGINT_DIGITSIZE; j++)
            binRepresentstionRaw += ((digits[i]>>j) & 1);

    std::string binRepresentstion;
    size = binRepresentstionRaw.size();
    i = size - 1;
    if(truncate)
        for(; binRepresentstionRaw[i]==0 && i>0; i--){}
    for(; i>=0; i--){
        binRepresentstion += binRepresentstionRaw[i] + '0';
        if(spacing && !(i%spacing) && i)
            binRepresentstion += separator;
    }
    return binRepresentstion;
}

std::string qw_ulong::toDec(int spacing, char separator, bool truncate) const{
    std::string decRepresentationRaw;
    qw_ulong copy = *this;
    qw_ulong remainder;
    int i = 0;
    do{
        divmod(copy, 10, copy, remainder);
        decRepresentationRaw += ((char)remainder + '0');
    }while(copy);
    std::string decRepresentation;
    for(i=decRepresentationRaw.size()-1; i>=0; i--){
        decRepresentation += decRepresentationRaw[i];
        if(spacing && !(i%spacing) && i)
            decRepresentation += separator;
    }
    return decRepresentation;
}
