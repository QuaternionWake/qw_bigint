#ifndef QW_ULONG_HPP
#define QW_ULONG_HPP

#include <vector>
#include <string>

#define QW_BIGINT_DIGITTYPE unsigned long long
#define QW_BIGINT_DIGITSIZE 64
#define QW_BIGINT_DIGITMAX 0xFFFF'FFFF'FFFF'FFFF

class qw_long;

class qw_ulong{
    private:
        std::vector<QW_BIGINT_DIGITTYPE> digits;

    public:
        //Constructors:
        qw_ulong();
        qw_ulong(QW_BIGINT_DIGITTYPE value);
        qw_ulong(std::vector<QW_BIGINT_DIGITTYPE> value);
        qw_ulong(qw_long& value);

        //Asignment operators:
        qw_ulong& operator=(const QW_BIGINT_DIGITTYPE value);
        qw_ulong& operator=(const std::vector<QW_BIGINT_DIGITTYPE>& value);
        qw_ulong& operator=(const qw_long& value);
        qw_ulong& operator+=(const qw_ulong& value);
        qw_ulong& operator-=(const qw_ulong& value);
        qw_ulong& operator*=(const qw_ulong& value);
        qw_ulong& operator/=(const qw_ulong& value);
        qw_ulong& operator%=(const qw_ulong& value);
        qw_ulong& operator<<=(const int& value);
        qw_ulong& operator>>=(const int& value);
        qw_ulong& operator&=(const qw_ulong& value);
        qw_ulong& operator|=(const qw_ulong& value);
        qw_ulong& operator^=(const qw_ulong& value);

        //Arithmetic operators:
        qw_ulong& operator++();
        qw_ulong& operator--();
        const qw_ulong operator++(int);
        const qw_ulong operator--(int);
        friend qw_ulong operator+(const qw_ulong& lhs, const qw_ulong& rhs);
        friend qw_ulong operator-(const qw_ulong& lhs, const qw_ulong& rhs);
        friend qw_ulong operator*(const qw_ulong& lhs, const qw_ulong& rhs);
        friend qw_ulong operator/(const qw_ulong& lhs, const qw_ulong& rhs);
        friend qw_ulong operator%(const qw_ulong& lhs, const qw_ulong& rhs);

        //Bitwise operators:
        friend qw_ulong operator<<(const qw_ulong& original, const int shiftAmount);
        friend qw_ulong operator>>(const qw_ulong& original, const int shiftAmount);
        friend qw_ulong operator&(const qw_ulong& lhs, const qw_ulong& rhs);
        friend qw_ulong operator|(const qw_ulong& lhs, const qw_ulong& rhs);
        friend qw_ulong operator^(const qw_ulong& lhs, const qw_ulong& rhs);
        friend qw_ulong operator~(const qw_ulong& original);

        //Comparison operators:
        friend bool operator==(const qw_ulong& lhs, const qw_ulong& rhs);
        friend bool operator!=(const qw_ulong& lhs, const qw_ulong& rhs);
        friend bool operator>(const qw_ulong& lhs, const qw_ulong& rhs);
        friend bool operator<(const qw_ulong& lhs, const qw_ulong& rhs);
        friend bool operator>=(const qw_ulong& lhs, const qw_ulong& rhs);
        friend bool operator<=(const qw_ulong& lhs, const qw_ulong& rhs);

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

        //Other methods:
        size_t size() const;
        size_t msb() const;
        void downsize();
        static qw_ulong gcd(const qw_ulong& const_a, const qw_ulong& const_b);
        static qw_ulong lcm(const qw_ulong& const_a, const qw_ulong& const_b);
        static void divmod(const qw_ulong& lhs, const qw_ulong& rhs, qw_ulong& quotient, qw_ulong& remainder);
        std::string toHex(int spacing = 0, char separator = ' ', bool truncate = 1) const;
        std::string toBin(int spacing = 0, char separator = ' ', bool truncate = 1) const;
        std::string toDec(int spacing = 0, char separator = ' ', bool truncate = 1) const;
};

#endif
