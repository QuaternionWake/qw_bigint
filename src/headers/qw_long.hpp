#ifndef QW_LONG_HPP
#define QW_LONG_HPP

#include <string>
#include <vector>
#include "qw_ulong.hpp"

class qw_long{
    private:
        bool sign;
        qw_ulong digits;

    public:
        //Constructors:
        qw_long();
        qw_long(QW_BIGINT_DIGITTYPE value);
        qw_long(std::vector<QW_BIGINT_DIGITTYPE> value);
        qw_long(qw_ulong value);

        //Asignment operators:
        qw_long& operator=(const QW_BIGINT_DIGITTYPE value);
        qw_long& operator+=(const qw_long& value);
        qw_long& operator-=(const qw_long& value);
        qw_long& operator*=(const qw_long& value);
        qw_long& operator/=(const qw_long& value);
        qw_long& operator%=(const qw_long& value);
        qw_long& operator<<=(const int& value);
        qw_long& operator>>=(const int& value);
        qw_long& operator&=(const qw_long& value);
        qw_long& operator|=(const qw_long& value);
        qw_long& operator^=(const qw_long& value);

        //Arithmetic operators:
        qw_long& operator++();
        qw_long& operator--();
        const qw_long operator++(int);
        const qw_long operator--(int);
        qw_long operator-();
        friend qw_long operator+(const qw_long& lhs, const qw_long& rhs);
        friend qw_long operator-(const qw_long& lhs, const qw_long& rhs);
        friend qw_long operator*(const qw_long& lhs, const qw_long& rhs);
        friend qw_long operator/(const qw_long& lhs, const qw_long& rhs);
        friend qw_long operator%(const qw_long& lhs, const qw_long& rhs);

        //Bitwise operators:
        friend qw_long operator<<(const qw_long& original, const int shiftAmount);
        friend qw_long operator>>(const qw_long& original, const int shiftAmount);
        friend qw_ulong operator&(const qw_long& lhs, const qw_long& rhs);
        friend qw_ulong operator|(const qw_long& lhs, const qw_long& rhs);
        friend qw_ulong operator^(const qw_long& lhs, const qw_long& rhs);
        friend qw_long operator~(const qw_long& original);

        //Comparison operators:
        friend bool operator==(const qw_long& lhs, const qw_long& rhs);
        friend bool operator!=(const qw_long& lhs, const qw_long& rhs);
        friend bool operator>(const qw_long& lhs, const qw_long& rhs);
        friend bool operator<(const qw_long& lhs, const qw_long& rhs);
        friend bool operator>=(const qw_long& lhs, const qw_long& rhs);
        friend bool operator<=(const qw_long& lhs, const qw_long& rhs);

        //Cast operators:
        explicit operator bool();
        explicit operator char();
        explicit operator short();
        explicit operator int();
        explicit operator long();
        explicit operator long long();
        explicit operator unsigned char();
        explicit operator unsigned short();
        explicit operator unsigned int();
        explicit operator unsigned long();
        explicit operator unsigned long long();
        explicit operator std::vector<QW_BIGINT_DIGITTYPE>();
        explicit operator qw_ulong();

        //Other methods:
        size_t size() const;
        size_t msb() const;
        qw_ulong getDigits() const;
        void downsize();
        void fixSign();
        static qw_ulong gcd(const qw_long& const_a, const qw_long& const_b);
        static qw_ulong lcm(const qw_long& const_a, const qw_long& const_b);
        static qw_ulong abs(const qw_long& value);
        static void divmod(const qw_long& lhs, const qw_long& rhs, qw_long& quotient, qw_long& remainder);
        std::string toHex(int spacing = 0, char separator = ' ', bool truncate = 1) const;
        std::string toBin(int spacing = 0, char separator = ' ', bool truncate = 1) const;
        std::string toDec(int spacing = 0, char separator = ' ', bool truncate = 1) const;
};

#endif
