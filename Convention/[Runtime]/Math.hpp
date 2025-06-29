#pragma once
#ifndef Convention_Runtime_Math_hpp
#define Convention_Runtime_Math_hpp

#include "Config.hpp"

namespace Convention
{
    namespace Math
    {
        // Mathematical constants
        template<typename T>
        struct Constants
        {
            static constexpr T PI = static_cast<T>(3.14159265358979323846);
            static constexpr T E = static_cast<T>(2.71828182845904523536);
            static constexpr T SQRT2 = static_cast<T>(1.41421356237309504880);
            static constexpr T SQRT3 = static_cast<T>(1.73205080756887729353);
            static constexpr T LN2 = static_cast<T>(0.69314718055994530942);
            static constexpr T LN10 = static_cast<T>(2.30258509299404568402);
        };

        // Basic math utilities
        template<typename T>
        constexpr T Abs(const T& value)
        {
            return value < T{} ? -value : value;
        }

        template<typename T>
        constexpr T Min(const T& a, const T& b)
        {
            return a < b ? a : b;
        }

        template<typename T>
        constexpr T Max(const T& a, const T& b)
        {
            return a > b ? a : b;
        }

        template<typename T>
        constexpr T Clamp(const T& value, const T& min, const T& max)
        {
            return Min(Max(value, min), max);
        }

        template<typename T>
        constexpr T Sign(const T& value)
        {
            return value > T{} ? T{1} : (value < T{} ? T{-1} : T{0});
        }

        // Power and root functions
        template<typename T>
        T Pow(const T& base, const T& exponent)
        {
            return std::pow(base, exponent);
        }

        template<typename T>
        T Sqrt(const T& value)
        {
            return std::sqrt(value);
        }

        template<typename T>
        T Cbrt(const T& value)
        {
            return std::cbrt(value);
        }

        // Trigonometric functions
        template<typename T>
        T Sin(const T& radians)
        {
            return std::sin(radians);
        }

        template<typename T>
        T Cos(const T& radians)
        {
            return std::cos(radians);
        }

        template<typename T>
        T Tan(const T& radians)
        {
            return std::tan(radians);
        }

        template<typename T>
        T Asin(const T& value)
        {
            return std::asin(value);
        }

        template<typename T>
        T Acos(const T& value)
        {
            return std::acos(value);
        }

        template<typename T>
        T Atan(const T& value)
        {
            return std::atan(value);
        }

        template<typename T>
        T Atan2(const T& y, const T& x)
        {
            return std::atan2(y, x);
        }

        // Degree/Radian conversion
        template<typename T>
        constexpr T DegToRad(const T& degrees)
        {
            return degrees * Constants<T>::PI / static_cast<T>(180);
        }

        template<typename T>
        constexpr T RadToDeg(const T& radians)
        {
            return radians * static_cast<T>(180) / Constants<T>::PI;
        }

        // Logarithmic functions
        template<typename T>
        T Log(const T& value)
        {
            return std::log(value);
        }

        template<typename T>
        T Log10(const T& value)
        {
            return std::log10(value);
        }

        template<typename T>
        T Log2(const T& value)
        {
            return std::log2(value);
        }

        template<typename T>
        T Exp(const T& value)
        {
            return std::exp(value);
        }

        // Floating point utilities
        template<typename T>
        bool IsNaN(const T& value)
        {
            return std::isnan(value);
        }

        template<typename T>
        bool IsInfinite(const T& value)
        {
            return std::isinf(value);
        }

        template<typename T>
        bool IsFinite(const T& value)
        {
            return std::isfinite(value);
        }

        template<typename T>
        bool Equal(const T& a, const T& b, const T& epsilon = std::numeric_limits<T>::epsilon())
        {
            return Abs(a - b) <= epsilon;
        }

        // Interpolation
        template<typename T>
        constexpr T Lerp(const T& a, const T& b, const T& t)
        {
            return a + t * (b - a);
        }

        template<typename T>
        constexpr T InverseLerp(const T& a, const T& b, const T& value)
        {
            return (value - a) / (b - a);
        }

        // Rounding functions
        template<typename T>
        T Floor(const T& value)
        {
            return std::floor(value);
        }

        template<typename T>
        T Ceil(const T& value)
        {
            return std::ceil(value);
        }

        template<typename T>
        T Round(const T& value)
        {
            return std::round(value);
        }

        template<typename T>
        T Trunc(const T& value)
        {
            return std::trunc(value);
        }

        // Modulo
        template<typename T>
        T Mod(const T& a, const T& b)
        {
            return std::fmod(a, b);
        }

        // Random number generation utilities
        class Random
        {
        private:
            std::mt19937 generator;

        public:
            Random() : generator(std::random_device{}()) {}
            explicit Random(uint32_t seed) : generator(seed) {}

            // Generate random integer in range [min, max]
            template<typename T = int>
            T Range(T min, T max)
            {
                static_assert(std::is_integral_v<T>, "T must be an integral type");
                std::uniform_int_distribution<T> dist(min, max);
                return dist(generator);
            }

            // Generate random float in range [min, max)
            template<typename T = float>
            T Range(T min, T max)
            {
                static_assert(std::is_floating_point_v<T>, "T must be a floating point type");
                std::uniform_real_distribution<T> dist(min, max);
                return dist(generator);
            }

            // Generate random float in range [0, 1)
            template<typename T = float>
            T Value()
            {
                static_assert(std::is_floating_point_v<T>, "T must be a floating point type");
                return Range<T>(T{0}, T{1});
            }

            // Generate random boolean
            bool Bool()
            {
                return Range<int>(0, 1) == 1;
            }
        };

        // Global random instance
        inline Random& GetGlobalRandom()
        {
            static Random instance;
            return instance;
        }

        // Convenience functions using global random
        template<typename T = int>
        T RandomRange(T min, T max)
        {
            return GetGlobalRandom().Range(min, max);
        }

        template<typename T = float>
        T RandomValue()
        {
            return GetGlobalRandom().Value<T>();
        }

        inline bool RandomBool()
        {
            return GetGlobalRandom().Bool();
        }
    }
}

#endif // Convention_Runtime_Math_hpp