#pragma once

#include <string>
#include <cstdint>
#include <ostream>

template<typename T>
struct coordinates 
{
    T x = 0;
    T y = 0;
    

    //add a constructor for std::make_unique. Modified parameter names for clarity
    coordinates( T x1 = 0, T y1 = 0 ) noexcept : x(x1), y(y1) { } 


    coordinates(const coordinates<T>& a ) noexcept : x(a.x), y(a.y) { }


    // copy assignment operator
    inline coordinates<T>& operator = ( const coordinates<T>& a ) noexcept
    { 
        x = a.x;
        y = a.y;
        return *this;
    }

    // move assignment operator
    coordinates<T>& operator = ( const coordinates<T>&& a ) noexcept
    { 
        x = a.x;
        y = a.y;
        return *this;
    }

    
    [[nodiscard]]
    inline bool operator ==  ( const coordinates<T>& a ) const noexcept
    {
        return { x == a.x && y == a.y };
    }


    [[nodiscard]]
    inline bool operator != ( const coordinates<T>& a ) const noexcept
    {
        return { x != a.x || y != a.y };
    }

    // we create some basic operations inside the struct
    [[nodiscard]]
    inline coordinates<T> operator + ( const coordinates<T>& a ) const noexcept
    {
        return { x+a.x, y+a.y };
    }

    template<typename E>
    [[nodiscard]]
    inline coordinates<T> operator + ( const coordinates<E>& a ) const noexcept
    {
        return { x+a.x, y+a.y };
    }

    [[nodiscard]]
    inline coordinates<T> operator - ( const coordinates<T>& a ) const noexcept
    {
        return { x-a.x, y-a.y };
    }

    [[nodiscard]]
    inline bool operator < ( const coordinates<T>& a ) const noexcept
    {
        return y == a.y ? x < a.x : y < a.y;
    }

    [[nodiscard]]
    inline bool operator <= ( const coordinates<T>& a ) const noexcept
    {
        return y == a.y ? x <= a.x : y <= a.y;
    }

    [[nodiscard]]
    inline bool operator > ( const coordinates<T>& a ) const noexcept
    {
        return y == a.y ? x > a.x : y > a.y;
    }

    [[nodiscard]]
    inline bool operator >= ( const coordinates<T>& a ) const noexcept
    {
        return y == a.y ? x >= a.x : y >= a.y;
    }


    template<typename D>
    [[nodiscard]]
    inline coordinates operator * ( const D& a ) const noexcept
    {
        return { x*a, y*a };
    }

    [[nodiscard]]
    inline std::string toString() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    [[nodiscard]]
    inline size_t distance_to(const coordinates<T>& other) const {
        return (abs(x - other.x) + abs(y - other.y));
    }

};

template <typename T>
std::ostream& operator<<(std::ostream& os, const coordinates<T>& coords) {
    return os << '(' << coords.x << ", " << coords.y << ')';
}


