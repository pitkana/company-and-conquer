#ifndef HELPERTOOLS
#define HELPERTOOLS

#include <algorithm>
#include <cstdint>
#include <memory>
#include <vector>
#include <cmath>
#include <string>
#include <array>



// create a namespace to stop any namespace errors
namespace helper
{


enum class Directions : uint8_t
{
    North = 0,
    East = 1,
    South = 2,
    West = 3
};



// we create a clamp function to only choose the value if its
// in the accepted range
template <typename T>
constexpr inline T clamp( const T& value, const T& smallest, const T& largest ) noexcept
{
    return std::min(largest, std::max(value, smallest));
}




template<typename T>
struct coordinates 
{
    T x = 0;
    T y = 0;
    

    //add a constructor for std::make_unique. Modified parameter names for clarity
    coordinates( T x1 = 0, T y1 = 0 ) noexcept : x(x1), y(y1) { } 


    coordinates(const coordinates<T>& a ) noexcept : x(a.x), y(a.y) { }

    // we create some basic operations inside the struct
    inline coordinates<T> operator + ( const coordinates<T>& a ) noexcept
    {
        return { x+a.x, y+a.y };
    }

    inline coordinates<T> operator - ( const coordinates<T>& a ) noexcept
    {
        return { x-a.x, y-a.y };
    }

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

    
    inline bool operator ==  ( const coordinates<T>& a ) noexcept
    {
        return { x == a.x && y == a.y };
    }


    inline bool operator != ( const coordinates<T>& a ) noexcept
    {
        return { x != a.x || y != a.y };
    }

    template<typename D>
    inline coordinates operator * ( const D& a ) noexcept
    {
        return { x*a, y*a };
    }

    inline std::string toString()
    {
        return std::to_string(x) + " " + std::to_string(y);
    }


    // increment() is a method which only 
    // raises or lowers the value if its not 0.
    // If the value is < 0, it lowers it,
    // if its > 0, it raises it.
    void increment()
    {   
        // basically here we calculate:  x = x + (-1)^(1 + 0 or 1), which will give us x+1 or x-1,
        if ( x != 0 ) x = x + pow(-1, 1 + (x > 0));
        if ( y != 0 ) y = y + pow(-1, 1 + (y > 0));
    }



};


template<typename T, typename D>
constexpr inline coordinates<T> square_to_pos( const coordinates<T>& coords, const D screen_width, const D screen_height, 
                                                bool use_clamp, const size_t amount_of_squares )
{
    T x = coords.x;
    T y = coords.y;
    
    if ( use_clamp ) {
        x = clamp<T>(x, 0, amount_of_squares);
        y = clamp<T>(y, 0, amount_of_squares);
    }
    

    D square_width = screen_width/amount_of_squares;
    D square_height = screen_height/amount_of_squares;

    T x1 = x*square_width;
    T y1 = y*square_height;

    return coordinates<T>{x1, y1};
}


class RGBA
{   
    private:
        /*
         The following hex values we'll use in bitwise operations to modify the RGBA values into a integer and vice-versa,
         the compiler should replace calling these variables with just their values.

         By making them static we use the same values for every RGBA_t class, so we save alot of memory
        */
        static const uint32_t red_hex_ = 0xff000000;
        static const uint32_t green_hex_ = 0x00ff0000;
        static const uint32_t blue_hex_ = 0x0000ff00;
        static const uint32_t alpha_hex_ = 0x000000ff;

        uint32_t color_ = 0;
        

    public:

        // define some constructors
        constexpr RGBA( ) noexcept { }

        constexpr RGBA( uint32_t red, uint32_t green, uint32_t blue , uint32_t alpha = 255) noexcept : 
            color_( ( (red & 0xff) << 24 ) + ( (green & 0xff) << 16 ) + ((blue & 0xff) << 8) + (alpha & 0xff) ) { } 

        constexpr RGBA( uint32_t hex) noexcept :
            color_(hex) {}
        
        inline uint32_t get_color() const noexcept
        {
            return color_;
        }

        inline uint32_t get_red() const noexcept
        {
            return  ( color_ & red_hex_ ) >> 24;
        }

        inline uint32_t get_green() const noexcept
        {
            return  ( color_ & green_hex_ ) >> 16;
        }

        inline uint32_t get_blue() const noexcept
        {
            return  ( color_ & blue_hex_ ) >> 8;
        }

        // assign certain color values
        // assign the red value
        inline void set_red( uint32_t num ) noexcept
        {
            color_ = ( color_ & ~red_hex_ ) | ( ( num << 24 ) & red_hex_ );
        }

        // assign the green value
        inline void set_green( uint32_t num ) noexcept
        {
            color_ = ( color_ & ~green_hex_ ) | ( ( num << 16 ) & green_hex_ );
        }

        // assign the blue value
        inline void set_blue( uint32_t num ) noexcept
        {
            color_ = ( color_ & ~blue_hex_ ) | ( (num << 8) & blue_hex_ );
        }

        // assign the alpha value
        inline void set_alpha ( uint32_t num ) noexcept
        {
            color_ = (color_ & ~alpha_hex_) | (num & alpha_hex_);
        }


        inline RGBA operator + ( RGBA a_color ) noexcept 
        {
            return { this->get_red() + a_color.get_red(), this->get_green() + a_color.get_green(), this->get_blue() + a_color.get_blue() };
        }

        inline RGBA operator - ( RGBA a_color ) noexcept
        {
            return { this->get_red() - a_color.get_red(), this->get_green() - a_color.get_green(), this->get_blue() - a_color.get_blue() };
        }

        inline RGBA operator = ( uint32_t num ) noexcept
        {
            return { this->to_rgba(num) };
        }


        inline RGBA to_rgba( uint32_t hex ) noexcept
        {
            return { this->color_ = hex };
        }

        template<typename T>
        inline bool operator < ( T value ) const noexcept
        {
            return { this->get_color() < value };
        }

        template<typename T>
        inline bool operator > ( T value ) const noexcept
        {
            return { this->get_color() > value };
        }
        
        template<typename T>
        inline bool operator == ( T value ) const noexcept
        {
            return { this->get_color() == value };
        }
};

/**
 * @brief class definition for a Matrix class that contains a std::vector
 * as the underlying container. The class is more used as a grid based container 
 * and not used to make mathematical matrix operations on its data.
 */
template<typename T>
class Matrix
{
    private: 
        std::vector<T> data_;
        size_t width_ = 0;
        size_t height_ = 0;


    public:
        // initialise a n x n matrix
        Matrix( size_t n ) noexcept : data_(n * n), width_(n), height_(n) { }

        // initialise a n x m matrix
        Matrix( size_t n, size_t m ) noexcept : data_(n * m), width_(n), height_(m) { }


        // initialise a n x n matrix with the <value> at every cell
        template<typename D>
        Matrix( size_t n, D value ) noexcept : data_(n * n, value), width_(n), height_(n) { }


        // initialise a n x m matrix with the <value> initialised at every cell
        template<typename D>
        Matrix( size_t n, size_t m, D value ) noexcept : data_(n * m, value), width_(n), height_(m) { }


        constexpr size_t size() noexcept
        {
            return width_ * height_;
        }

        constexpr size_t width() noexcept
        {
            return width_;
        }

        constexpr size_t height() noexcept
        {
            return width_ * height_;
        }


        // works the same way as accessing with [i][j] as std::vector<std::vector<T>>
        constexpr inline T& operator () ( const size_t i, const size_t j ) const noexcept
        {
            return data_[ i * width_ + j ];
        }

        // this is a simpler way of accessing [x][y] by using a set of coordinates
        template<typename D>
        constexpr inline T& operator [] ( const coordinates<D>& a_coordinates ) const noexcept
        {
            return data_[ a_coordinates.x * width_ + a_coordinates.y ];
        }


};
// here we calculate if any of the given 2 vectors is a multiple of the other vector.
template<typename T>
inline bool same_direction(coordinates<T> a, coordinates<T> b)
{   
    //Two vectors are parallel (= multiple of the other) if their cross product is 0 (a.x * b.y - a.y * b.x == 0)
    return a.x * b.y == a.y * b.x;
}
}

#endif
