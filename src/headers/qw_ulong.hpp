#ifndef QW_ULONG_HPP
#define QW_ULONG_HPP

#include <vector>
#include <string>

class qw_ulong{
    private:
        std::vector<unsigned> digits;

    public:
        //Constructors:
        qw_ulong();
        qw_ulong(std::vector<unsigned> value);
        qw_ulong(unsigned long value);

        //Asignment operators:
        qw_ulong& operator=(const unsigned long value);
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
        void operator++();
        void operator--();
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
        explicit operator unsigned char();
        explicit operator unsigned short();
        explicit operator unsigned int();
        explicit operator unsigned long();
        explicit operator std::vector<unsigned>();

        //Other methods:
        size_t size() const;
        size_t msb() const;
        void downsize();
        static void divmod(const qw_ulong& lhs, const qw_ulong& rhs, qw_ulong& quotient, qw_ulong& remainder);
        std::string toHex(int spacing = 0, char separator = ' ', bool truncate = 1) const;
        std::string toBin(int spacing = 0, char separator = ' ', bool truncate = 1) const;
        std::string toDec(int spacing = 0, char separator = ' ', bool truncate = 1) const;
};

#endif
