#pragma once
#include "coordinates.hpp"

// create a namespace to stop any namespace errors
namespace Helper
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
        
        [[nodiscard]]
        inline uint32_t get_color() const noexcept
        {
            return color_;
        }

        [[nodiscard]]
        inline uint32_t get_red() const noexcept
        {
            return  ( color_ & red_hex_ ) >> 24;
        }

        [[nodiscard]]
        inline uint32_t get_green() const noexcept
        {
            return  ( color_ & green_hex_ ) >> 16;
        }

        [[nodiscard]]
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


        [[nodiscard]]
        inline RGBA operator + ( RGBA a_color ) noexcept 
        {
            return { this->get_red() + a_color.get_red(), this->get_green() + a_color.get_green(), this->get_blue() + a_color.get_blue() };
        }

        [[nodiscard]]
        inline RGBA operator - ( RGBA a_color ) noexcept
        {
            return { this->get_red() - a_color.get_red(), this->get_green() - a_color.get_green(), this->get_blue() - a_color.get_blue() };
        }

        [[nodiscard]]
        inline RGBA operator = ( uint32_t num ) noexcept
        {
            return { this->to_rgba(num) };
        }


        [[nodiscard]]
        inline RGBA to_rgba( uint32_t hex ) noexcept
        {
            return { this->color_ = hex };
        }

        template<typename T>
        [[nodiscard]]
        inline bool operator < ( T value ) const noexcept
        {
            return { this->get_color() < value };
        }

        template<typename T>
        [[nodiscard]]
        inline bool operator > ( T value ) const noexcept
        {
            return { this->get_color() > value };
        }
        
        template<typename T>
        [[nodiscard]]
        inline bool operator == ( T value ) const noexcept
        {
            return { this->get_color() == value };
        }
};


// here we calculate if any of the given 2 vectors is a multiple of the other vector.
template<typename T>
inline bool same_direction(coordinates<T> a, coordinates<T> b)
{   
    //Two vectors are parallel (= multiple of the other) if their cross product is 0 (a.x * b.y - a.y * b.x == 0)
    return a.x * b.y == a.y * b.x;
}

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
}

