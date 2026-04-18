#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include <algorithm>


class Int128 {
    
private:
    int64_t high;
    uint64_t low;

public:
    // конструкторы 
    Int128() : high(0), low(0) {}

    Int128(int64_t value) {
        high = (value < 0 ? -1 : 0);
        low = static_cast<uint64_t>(value);
    }

    Int128(std::string_view str) : Int128(0) {
        bool neg = false;
        size_t i = 0;

        if (str[0] == '-') {
            neg = true;
            i = 1;
        }

        for (; i < str.size(); i++) {
            *this *= Int128(10);
            *this += Int128(str[i] - '0');
        }

        if (neg) *this = -*this;
    }

    // преобразования
    explicit operator int64_t() const {
        return static_cast<int64_t>(low);
    }

    explicit operator double() const {
        return (double)high * (1ULL << 63) * 2 + (double)low;
    }
    
    
    std::string str() const {
        if (*this == Int128(0)) return "0";

        Int128 tmp = *this;
        bool neg = false;

        if (tmp.high < 0) {
            neg = true;
            tmp = -tmp;
        }

        std::string res;

        while (tmp != Int128(0)) {
            Int128 d = tmp % Int128(10);
            res.push_back('0' + (int)d.low);
            tmp /= Int128(10);
        }

        if (neg) res.push_back('-');
        std::reverse(res.begin(), res.end());
        return res;
    }

        // арифметика
    Int128 operator+(const Int128& o) const {
        Int128 r;
        r.low = low + o.low;
        r.high = high + o.high + (r.low < low);
        return r;
    }

    Int128 operator-(const Int128& o) const {
        return *this + (-o);
    }

    Int128 operator-() const {
        Int128 r;
        r.low = ~low + 1;
        r.high = ~high + (r.low == 0);
        return r;
    }

        // умножение
    Int128 operator*(const Int128& o) const {
        Int128 a = *this;
        Int128 b = o;
        Int128 res(0);

        bool neg = false;
        if (a.high < 0) { a = -a; neg = !neg; }
        if (b.high < 0) { b = -b; neg = !neg; }

        while (b != Int128(0)) {
            res += a;
            b -= Int128(1);
        }

        return neg ? -res : res;
    }

    // деление
    Int128 operator/(const Int128& o) const {
        Int128 a = *this;
        Int128 b = o;
        Int128 res(0);

        bool neg = false;
        if (a.high < 0) { a = -a; neg = !neg; }
        if (b.high < 0) { b = -b; neg = !neg; }

        while (a >= b) {
            a -= b;
            res += Int128(1);
        }

        return neg ? -res : res;
    }

    Int128 operator%(const Int128& o) const {
        Int128 a = *this;
        Int128 b = o;

        if (a.high < 0) a = -a;
        if (b.high < 0) b = -b;

        while (a >= b) {
            a -= b;
        }
        return a;
    }


    Int128& operator+=(const Int128& o) { return *this = *this + o; }
    Int128& operator-=(const Int128& o) { return *this = *this - o; }
    Int128& operator*=(const Int128& o) { return *this = *this * o; }
    Int128& operator/=(const Int128& o) { return *this = *this / o; }

    // сравнение
    bool operator==(const Int128& o) const {
        return high == o.high && low == o.low;
    }
    
    

    bool operator!=(const Int128& o) const {
        return !(*this == o);
    }

    bool operator<(const Int128& o) const {
        if (high != o.high) return high < o.high;
        return low < o.low;
    }

    bool operator>=(const Int128& o) const {
        return !(*this < o);
    }

    friend std::ostream& operator<<(std::ostream& out, const Int128& x) {
        return out << x.str();
    }
};