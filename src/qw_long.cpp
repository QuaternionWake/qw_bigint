#include "headers/qw_ulong.hpp"
#include "headers/qw_long.hpp"

#include <string>
#include <vector>

    //Constructors:

qw_long::qw_long(){
    digits = 0;
    sign = 0;
}

qw_long::qw_long(QW_BIGINT_DIGITTYPE value){
    digits = value;
    sign = 0;
}

qw_long::qw_long(std::vector<QW_BIGINT_DIGITTYPE> value){
    digits = value;
    sign = 0;
}

qw_long::qw_long(qw_ulong value){
    digits = value;
    sign = 0;
}

    //Asignment operators:

qw_long& qw_long::operator=(const QW_BIGINT_DIGITTYPE value){
    digits = value;
    return *this;
}

qw_long& qw_long::operator+=(const qw_long& value){
    *this = *this + value;
    return *this;
}

qw_long& qw_long::operator-=(const qw_long& value){
    *this = *this - value;
    return *this;
}

qw_long& qw_long::operator*=(const qw_long& value){
    *this = *this * value;
    return *this;
}

qw_long& qw_long::operator/=(const qw_long& value){
    *this = *this / value;
    return *this;
}

qw_long& qw_long::operator%=(const qw_long& value){
    *this = *this % value;
    return *this;
}

qw_long& qw_long::operator<<=(const int& value){
    *this = *this << value;
    return *this;
}

qw_long& qw_long::operator>>=(const int& value){
    *this = *this >> value;
    return *this;
}

qw_long& qw_long::operator&=(const qw_long& value){
    *this = *this & value;
    return *this;
}

qw_long& qw_long::operator|=(const qw_long& value){
    *this = *this | value;
    return *this;
}

qw_long& qw_long::operator^=(const qw_long& value){
    *this = *this ^ value;
    return *this;
}

    //Arithmetic operators:

qw_long& qw_long::operator++(){
    if(!sign)
        ++digits;
    else
        --digits;
    fixSign();
    return *this;
}

qw_long& qw_long::operator--(){
    if(digits == 0){
        sign = 1;
        ++digits;
    }else if(!sign)
        --digits;
    else
        ++digits;
    return *this;
}

const qw_long qw_long::operator++(int){
    qw_long copy = *this;
    ++(*this);
    return copy;
}

const qw_long qw_long::operator--(int){
    qw_long copy = *this;
    --(*this);
    return copy;
}

qw_long qw_long::operator-(){
    qw_long copy = *this;
    copy.sign = copy.sign != 1;
    return copy;
}


qw_long operator+(const qw_long& lhs, const qw_long& rhs){
    qw_long result;
    if(lhs.sign == rhs.sign){
        result.digits = lhs.digits + rhs.digits;
        result.sign = lhs.sign;
    }else if(lhs.digits > rhs.digits){
        result.digits = lhs.digits - rhs.digits;
        result.sign = lhs.sign;
    }else{
        result.digits = rhs.digits - lhs.digits;
        result.sign = rhs.sign;
        result.fixSign();
    }
    return result;
}

qw_long operator-(const qw_long& lhs, const qw_long& rhs){
    qw_long result;
    if(lhs.sign != rhs.sign){
        result.digits = lhs.digits + rhs.digits;
        result.sign = lhs.sign;
    }else if(lhs.digits > rhs.digits){
        result.digits = lhs.digits - rhs.digits;
        result.sign = lhs.sign;
    }else{
        result.digits = rhs.digits - lhs.digits;
        result.sign = !lhs.sign;
        result.fixSign();
    }
    return result;
}

qw_long operator*(const qw_long& lhs, const qw_long& rhs){
    qw_long result = lhs.digits * rhs.digits;
    result.sign = lhs.sign != rhs.sign;
    result.fixSign();
    return result;
}

qw_long operator/(const qw_long& lhs, const qw_long& rhs){
    qw_long result = lhs.digits / rhs.digits;
    result.sign = lhs.sign != rhs.sign;
    return result;
}

qw_long operator%(const qw_long& lhs, const qw_long& rhs){
    qw_long result = lhs.digits % rhs.digits;
    result.sign = lhs.sign;
    return result;
}

    //Bitwise operators:

qw_long operator<<(const qw_long& original, const int shiftAmount){
    qw_long result = original.digits<<shiftAmount;
    result.sign = original.sign;
    return result;
}

qw_long operator>>(const qw_long& original, const int shiftAmount){
    qw_long result = original.digits>>shiftAmount;
    result.sign = original.sign;
    return result;
}

qw_ulong operator&(const qw_long& lhs, const qw_long& rhs){
    return lhs.digits & rhs.digits;
}

qw_ulong operator|(const qw_long& lhs, const qw_long& rhs){
    return lhs.digits | rhs.digits;
}

qw_ulong operator^(const qw_long& lhs, const qw_long& rhs){
    return lhs.digits ^ rhs.digits;
}

qw_long operator~(const qw_long& original){
    qw_long result = ~original.digits;
    result.sign = original.sign;
    return result;
}

    //Comparison operators:

bool operator==(const qw_long& lhs, const qw_long& rhs){
    if(rhs.sign != lhs.sign)
        return 0;
    return rhs.digits == lhs.digits;
}

bool operator!=(const qw_long& lhs, const qw_long& rhs){
    if(rhs.sign != lhs.sign)
        return 1;
    return rhs.digits != lhs.digits;
}

bool operator>(const qw_long& lhs, const qw_long& rhs){
    if(rhs.sign != lhs.sign)
        return !lhs.sign;
    else if(!lhs.sign)
        return lhs.digits > rhs.digits;
    else
        return lhs.digits < rhs.digits;
}

bool operator<(const qw_long& lhs, const qw_long& rhs){
    if(rhs.sign != lhs.sign)
        return lhs.sign;
    else if(!lhs.sign)
        return lhs.digits < rhs.digits;
    else
        return lhs.digits > rhs.digits;
}

bool operator>=(const qw_long& lhs, const qw_long& rhs){
    if(rhs.sign != lhs.sign)
        return !lhs.sign;
    else if(!lhs.sign)
        return lhs.digits >= rhs.digits;
    else
        return lhs.digits <= rhs.digits;
}

bool operator<=(const qw_long& lhs, const qw_long& rhs){
    if(rhs.sign != lhs.sign)
        return lhs.sign;
    else if(!lhs.sign)
        return lhs.digits <= rhs.digits;
    else
        return lhs.digits >= rhs.digits;
}

    //Cast operators:

qw_long::operator bool(){return (bool)digits;}

qw_long::operator char(){return (char)digits;}
qw_long::operator short(){return (short)digits;}
qw_long::operator int(){return (int)digits;}
qw_long::operator long(){return (long)digits;}
qw_long::operator long long(){return (long long)digits;}
qw_long::operator unsigned char(){return (unsigned char)digits;}
qw_long::operator unsigned short(){return (unsigned short)digits;}
qw_long::operator unsigned int(){return (unsigned int)digits;}
qw_long::operator unsigned long(){return (unsigned long)digits;}
qw_long::operator unsigned long long(){return (unsigned long long)digits;}

qw_long::operator std::vector<QW_BIGINT_DIGITTYPE>(){return (std::vector<QW_BIGINT_DIGITTYPE>)digits;}
qw_long::operator qw_ulong(){return digits;}

    //Other methods:

size_t qw_long::size() const{
    return digits.size();
}

size_t qw_long::msb() const{
    return digits.msb();
}

qw_ulong qw_long::getDigits() const{
    return digits;
}

void qw_long::downsize(){
    digits.downsize();
}

void qw_long::fixSign(){
    sign = sign && digits != 0;
}

static qw_ulong gcd(const qw_long& const_a, const qw_long& const_b){
    return gcd(const_a.getDigits(), const_b.getDigits());
}

static qw_ulong lcm(const qw_long& const_a, const qw_long& const_b){
    return lcm(const_a.getDigits(), const_b.getDigits());
}

static qw_ulong abs(const qw_long& value){
    return value.getDigits();
}

inline void qw_long::divmod(const qw_long& lhs, const qw_long& rhs, qw_long& quotient, qw_long& remainder){
    qw_ulong::divmod(lhs.digits, rhs.digits, quotient.digits, remainder.digits);
    quotient.sign =  lhs.sign != rhs.sign;
    quotient.sign =  lhs.sign;
}

std::string qw_long::toHex(int spacing, char separator, bool truncate) const{
    std::string hexRepresentstion;
    if(sign)
        hexRepresentstion = "-";
    hexRepresentstion += digits.toHex(spacing, separator, truncate);
    return hexRepresentstion;
}

std::string qw_long::toBin(int spacing, char separator, bool truncate) const{
    std::string binRepresentstion;
    if(sign)
        binRepresentstion = "-";
    binRepresentstion += digits.toBin(spacing, separator, truncate);
    return binRepresentstion;
}

std::string qw_long::toDec(int spacing, char separator, bool truncate) const{
    std::string decRepresentstion;
    if(sign)
        decRepresentstion = "-";
    decRepresentstion += digits.toDec(spacing, separator, truncate);
    return decRepresentstion;
}
