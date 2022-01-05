#include <numeric>
#include <type_traits>
#include <iostream>
#include <compare>
#include <algorithm>
namespace DevUt{
template<typename T,typename = std::enable_if_t<std::is_integral_v<T>>>
class frac{
    T num,den;
    void normalize(){
        T tempgcd = std::gcd(num,den);
        num /= tempgcd; den /= tempgcd;
        if(den < 0){
            num -= num;
            den = -den;
        }
    }
    public :
    frac(const T& numerator=0,const T& denominator=1):num(numerator),den(denominator){
        normalize();
    }
    frac(const T&& numerator=0,const T&& denominator=1):num(numerator),den(denominator){
        normalize();
    }
    ~frac() = default;

    const T& numerator() const{
        return num;
    }
    T& numerator(){
        return num;
    }

    const T& denominator() const{
        return den;
    }

    T& denominator(){
        return den;
    }
    frac& operator=(const frac&  right){
        num = right.numerator();
        den = right.denominator();
        return *this;
    }
    void swap(frac& right){
        std::swap((*this).num,right.num);
        std::swap((*this).den,right.den);
    }
    auto operator<=>(const frac& right) const{
        return right.denominator()*numerator() <=> denominator()*right.numerator();
    }

    frac operator+(const frac& right) const{
        auto result = *this;
        return result += right;
    }

    frac operator-(const frac& right) const{
        auto result = *this;
        return result -= right;
    }

    frac operator*(const frac& right) const{
        auto result = *this;
        return result *= right;
    }

    frac operator/(const frac& right) const{
        auto result = *this;
        return result /= right;
    }

    frac& operator+=(const frac& right){
        T tempnum = numerator()*right.denominator() + denominator()*right.numerator();
        T tempden = denominator()*right.denominator();
        num = tempnum;
        den = tempden;
        normalize();
        return (*this);
    }

    frac& operator-=(const frac& right){
        T tempnum = numerator()*right.denominator() - denominator()*right.numerator();
        T tempden = denominator()*right.denominator();
        num = tempnum;
        den = tempden;
        normalize();
        return (*this);
    }

    frac& operator*=(const frac& right){
        num = numerator()*right.numerator();
        den = denominator()*right.denominator();
        normalize();
        return (*this);
    }

    frac& operator/=(const frac& right){
        num = numerator()*right.denominator();
        den = denominator()*right.numerator();
        normalize();
        return (*this);
    }

};
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const frac<T>& right){
        stream<<right.numerator()<<'/'<<right.denominator();
        return stream;
    }
}