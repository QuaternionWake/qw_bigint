#include "headers/qw_ulong.hpp"
#include <string>
#include <vector>
#include <stdexcept>

    //Constructors:

qw_ulong::qw_ulong(){
    digits.resize(1);
}

qw_ulong::qw_ulong(std::vector<unsigned> value){
    digits = value;
}

qw_ulong::qw_ulong(unsigned long value){
    if(value>>32){
        digits.resize(2);
        digits[0] = value;
        digits[1] = value >> 32;
    }else{
        digits.resize(1);
        digits[0] = value;
    }
}
    //Asignment operators:

qw_ulong& qw_ulong::operator=(const unsigned long value){
    if(value>>32){
        digits.resize(2);
        digits[0] = value;
        digits[1] = value >> 32;
        return *this;
    }else{
        digits.resize(1);
        digits[0] = value;
        return *this;
    }
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

void qw_ulong::operator++(){
    size_t size = this->size();
    int i;
    int carry = 1;
    for(i=0; carry && i<size; i++){
        digits[i] += carry;
        carry = !digits[i];
        if(carry && (i == size-1))
            digits.push_back(carry);
    }
}

void qw_ulong::operator--(){
    size_t size = this->size();
    if(size == 1 && digits[0] == 0)
        return;
    int i;
    int carry = 1;
    for(i=0; carry && i<size; i++){
        digits[i] -= carry;
        carry = !(digits[i]+1);
    }
    downsize();
}

qw_ulong operator+(const qw_ulong& lhs, const qw_ulong& rhs){
    size_t lhsSize = lhs.digits.size();
    size_t rhsSize = rhs.digits.size();
    const std::vector<unsigned> *longOne, *shortOne;
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
    
    unsigned long sum;
    unsigned long carry = 0;
    qw_ulong result;
    result.digits.resize(longSize+1);
    int i;
    for(i=0; i<shortSize; i++){
        sum = (unsigned long)(*shortOne)[i] + (*longOne)[i] + carry;
        result.digits[i] = sum;
        carry = sum >> 32;
    }
    for(; i<longSize; i++){
        sum = (unsigned long)(*longOne)[i] + carry;
        result.digits[i] = sum;
        carry = sum >> 32;
    }
    result.digits[i] = carry;
    result.downsize();
    return result;
}

qw_ulong operator-(const qw_ulong& lhs, const qw_ulong& rhs){
    if(lhs<=rhs){
        return 0;
    }
    size_t lhsSize = lhs.digits.size();
    size_t rhsSize = rhs.digits.size();
    
    unsigned long sum;
    unsigned long carry = 0;
    qw_ulong result;
    result.digits.resize(lhsSize);
    int i;
    for(i=0; i<rhsSize; i++){
        sum = (unsigned long)lhs.digits[i] - rhs.digits[i] - carry;
        result.digits[i] = sum;
        carry = -(int)(sum >> 32);
    }
    for(; i<lhsSize; i++){
        sum = (unsigned long)lhs.digits[i] - carry;
        result.digits[i] = sum;
        carry = -(int)(sum >> 32);
    }
    result.downsize();
    return result;
}

qw_ulong operator*(const qw_ulong& lhs, const qw_ulong& rhs){
    size_t lhsSize = lhs.digits.size();
    size_t rhsSize = rhs.digits.size();
    
    unsigned long sum;
    unsigned long carry;
    qw_ulong result;
    qw_ulong partialResult;
    int i, j;
    for(i=0; i<lhsSize; i++){
        carry = 0;
        partialResult = 0;
        partialResult.digits.resize(lhsSize+rhsSize);
        for(j=0; j<rhsSize; j++){
            sum = (unsigned long)lhs.digits[i] * rhs.digits[j] + carry;
            partialResult.digits[i+j] += sum;
            carry = sum >> 32;
        }
        if(carry)
            partialResult.digits[i+j] = carry;
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
        result.digits[shift/32] |= 1 << (shift%32);
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
    int bigShift = shiftAmount / 32;
    int smallShift = shiftAmount % 32;

    size_t newSize = oldSize + bigShift;
    qw_ulong result;
    result.digits.resize(newSize+1);
    std::vector<unsigned long> temp(newSize);
    int i;
    for(i=0; i<oldSize; i++){
        temp[i+bigShift] = (unsigned long)original.digits[i] << smallShift;
    }
    result.digits[0] = temp[0];
    for(i=1; i<newSize; i++){
        result.digits[i] = ((temp[i-1] >> 32) | temp[i]);
    }
    result.digits[i] = (temp[i-1]>>32);
    result.downsize();
    return result;
}

qw_ulong operator>>(const qw_ulong& original, const int shiftAmount){
    if(shiftAmount < 0){
        return original << -shiftAmount;
    }
    size_t oldSize = original.digits.size();
    int bigShift =  shiftAmount / 32;
    int smallShift =  shiftAmount % 32;

    size_t newSize = oldSize - bigShift;
    qw_ulong result;
    result.digits.resize(newSize+1);
    std::vector<unsigned long> temp(newSize);
    int i;
    for(i=bigShift; i<oldSize; i++){
        temp[i-bigShift] = (unsigned long)original.digits[i] << 32 >> smallShift;
    }
    result.digits[0] = (temp[0] >> 32) | temp[1];
    for(i=1; i<newSize-1; i++){
        result.digits[i] = (temp[i] >> 32) | temp[i+1];
    }
    result.digits.push_back(temp[i]>>32);
    result.downsize();
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
    const std::vector<unsigned> *longOne, *shortOne;
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
    const std::vector<unsigned> *longOne, *shortOne;
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
    size_t size = original.digits.size();
    
    qw_ulong result;
    result.digits.resize(size);
    int i;
    for(i=0; i<size; i++){
        result.digits[i] = ~original.digits[i];
    }
    for(i=31; (result.digits[size-1]>>i&1); i--) {}
    result.digits[size-1] &= 0xFFFFFFFF >> (31-i);
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
qw_ulong::operator long(){
    if(digits.size() == 1)
        return digits[0];
    else
        return digits[0] | ((unsigned long)digits[1] << 32);
}

qw_ulong::operator unsigned char(){return digits[0];}
qw_ulong::operator unsigned short(){return digits[0];}
qw_ulong::operator unsigned int(){return digits[0];}
qw_ulong::operator unsigned long(){
    if(digits.size() == 1)
        return digits[0];
    else
        return digits[0] | ((unsigned long)digits[1] << 32);
}

qw_ulong::operator std::vector<unsigned>(){return digits;}

    //Other methods:

size_t qw_ulong::size() const{
    return digits.size();
}

size_t qw_ulong::msb() const{
    size_t msb = 0;                   //most significant bit
    size_t msd = digits.size() - 1;   //most significant digit
    unsigned digit = digits[msd];
    while(digit){
        msb++;
        digit >>= 1;
    }
    return msd*32 + msb;
}

void qw_ulong::downsize(){
    if(*this == 0){
        digits.resize(1);
        return;
    }
    size_t size = digits.size();
    int i;
    for(i=size; i>0; i--)
        if(digits[i-1])
            break;
    digits.resize(i);
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
        result.digits[shift/32] |= 1 << (shift%32);
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
    int i;
    for(i=0; i<size; i++){
        hexRepresentstionRaw += ((digits[i]>>0) & 0xF);
        hexRepresentstionRaw += ((digits[i]>>4) & 0xF);
        hexRepresentstionRaw += ((digits[i]>>8) & 0xF);
        hexRepresentstionRaw += ((digits[i]>>12) & 0xF);
        hexRepresentstionRaw += ((digits[i]>>16) & 0xF);
        hexRepresentstionRaw += ((digits[i]>>20) & 0xF);
        hexRepresentstionRaw += ((digits[i]>>24) & 0xF);
        hexRepresentstionRaw += ((digits[i]>>28) & 0xF);
    }
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
        for(j=0; j<32; j++)
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
    std::string decRepresentationReverse;
    qw_ulong copy = *this;
    qw_ulong remainder;
    int i = 0;
    while(copy){
        divmod(copy, 10, copy, remainder);
        decRepresentationReverse += ((char)remainder + '0');
    }
    std::string decRepresentation;
    for(i=decRepresentationReverse.size()-1; i>=0; i--){
        decRepresentation += decRepresentationReverse[i];
        if(spacing && !(i%spacing) && i)
            decRepresentation += separator;
    }
    return decRepresentation;
}
