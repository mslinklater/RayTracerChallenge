#pragma once

#include <ostream>
#include <ratio>
#include <type_traits>

// ─────────────────────────────────────────────────────────────────────────────
// Forward declarations
// ─────────────────────────────────────────────────────────────────────────────

template <typename Ratio> class SquaredLength;

// ─────────────────────────────────────────────────────────────────────────────
// Compile-time GCD helper (avoids reliance on std::ratio_gcd availability)
// ─────────────────────────────────────────────────────────────────────────────

namespace detail
{
    template <intmax_t A, intmax_t B>
    struct gcd_impl { static constexpr intmax_t value = gcd_impl<B, A % B>::value; };

    template <intmax_t A>
    struct gcd_impl<A, 0> { static constexpr intmax_t value = (A < 0 ? -A : A); };

    /**
     * @brief Computes the GCD of two std::ratio types as a new std::ratio.
     *
     * gcd(a/b, c/d) = gcd(a,c) / lcm(b,d),  where lcm(b,d) = b/gcd(b,d) × d.
     */
    template <typename R1, typename R2>
    struct ratio_gcd_impl
    {
        static constexpr intmax_t num = gcd_impl<R1::num, R2::num>::value;
        static constexpr intmax_t g   = gcd_impl<R1::den, R2::den>::value;
        static constexpr intmax_t den = (R1::den / g) * R2::den;
        using type = std::ratio<num, den>;
    };
} // namespace detail

// ─────────────────────────────────────────────────────────────────────────────
// Helper alias
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief The most-precise unit common to both @p R1 and @p R2.
 *
 * Implemented via @c detail::ratio_gcd_impl. The result is always the finer
 * of the two units (e.g., the common ratio of cm and m is cm).
 */
template <typename R1, typename R2>
using LengthCommonRatio = typename detail::ratio_gcd_impl<R1, R2>::type;

// ─────────────────────────────────────────────────────────────────────────────
// Length<Ratio>
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief A strongly-typed length value parameterised by an SI unit ratio.
 *
 * The @p Ratio template argument is a @c std::ratio representing how many
 * metres one unit equals:
 *   - @c std::milli  (1/1000) → millimetres
 *   - @c std::centi  (1/100)  → centimetres
 *   - @c std::ratio<1>        → metres
 *   - @c std::kilo  (1000/1)  → kilometres
 *
 * All arithmetic between @c Length values of different units is handled
 * automatically by the compiler. The result type is always the most precise
 * common unit (e.g., @c cm + @c m → @c cm). It is a compile-time error to
 * assign or compare a bare @c float directly with a @c Length.
 *
 * Use the user-defined literals from @c si_literals to construct values:
 * @code
 *   using namespace si_literals;
 *   auto total = 30.0_cm + 1.0_m;   // Centimeters(130)
 *   auto area  = 3.0_cm * 4.0_m;    // SquaredLength<centi>(1200) = 0.12 m²
 * @endcode
 *
 * @tparam Ratio A @c std::ratio representing the scale factor (metres per unit).
 */
template <typename Ratio>
class Length
{
public:
    /// @brief The @c std::ratio scale factor for this unit.
    using ratio_type = Ratio;

    /** @brief Default constructor — initialises to zero. */
    constexpr Length() : value_(0.f) {}

    /**
     * @brief Constructs a length with an explicit numeric value in this unit.
     * @param v The value in this unit's native scale.
     */
    constexpr explicit Length(float v) : value_(v) {}

    /**
     * @brief Implicit conversion to any other @c Length unit.
     *
     * Scales the stored value by the exact @c Ratio / @c OtherRatio factor
     * computed at compile time — no runtime branches required.
     *
     * @tparam OtherRatio The target unit's @c std::ratio scale factor.
     */
    template <typename OtherRatio>
    constexpr operator Length<OtherRatio>() const
    {
        using Conv = std::ratio_divide<Ratio, OtherRatio>;
        return Length<OtherRatio>(
            value_ * static_cast<float>(Conv::num) / static_cast<float>(Conv::den));
    }

    /** @brief Returns the raw value in this unit's native scale. */
    constexpr float GetValue() const { return value_; }

    /** @brief Converts and returns the value expressed in metres. */
    constexpr float ToMeters() const
    {
        return value_ * static_cast<float>(Ratio::num) / static_cast<float>(Ratio::den);
    }

    /** @brief Unary negation. */
    constexpr Length operator-() const { return Length(-value_); }

    /**
     * @brief Adds @p other (any unit) to this length, converting first.
     * @tparam OtherRatio The unit of @p other.
     */
    template <typename OtherRatio>
    constexpr Length &operator+=(Length<OtherRatio> other)
    {
        value_ += Length<Ratio>(other).GetValue();
        return *this;
    }

    /**
     * @brief Subtracts @p other (any unit) from this length, converting first.
     * @tparam OtherRatio The unit of @p other.
     */
    template <typename OtherRatio>
    constexpr Length &operator-=(Length<OtherRatio> other)
    {
        value_ -= Length<Ratio>(other).GetValue();
        return *this;
    }

    /** @brief Scales this length by a dimensionless scalar. */
    constexpr Length &operator*=(float s)
    {
        value_ *= s;
        return *this;
    }

    /** @brief Divides this length by a dimensionless scalar. */
    constexpr Length &operator/=(float s)
    {
        value_ /= s;
        return *this;
    }

private:
    float value_; ///< Raw value in this unit's native scale.
};

// ─────────────────────────────────────────────────────────────────────────────
// SquaredLength<Ratio>
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief An area value parameterised by the SI unit ratio of its linear dimension.
 *
 * Produced automatically when two @c Length values are multiplied. The stored
 * value is in @p Ratio² units (e.g., @c SquaredLength<std::centi> stores cm²).
 *
 * @tparam Ratio A @c std::ratio representing the linear unit's scale factor.
 */
template <typename Ratio>
class SquaredLength
{
public:
    /// @brief The @c std::ratio scale factor for the underlying linear unit.
    using ratio_type = Ratio;

    /** @brief Default constructor — initialises to zero. */
    constexpr SquaredLength() : value_(0.f) {}

    /**
     * @brief Constructs an area with an explicit value in this unit's squared scale.
     * @param v The value in @p Ratio² units.
     */
    constexpr explicit SquaredLength(float v) : value_(v) {}

    /** @brief Returns the raw value in this unit's squared scale. */
    constexpr float GetValue() const { return value_; }

    /** @brief Converts and returns the value expressed in square metres. */
    constexpr float ToSquareMeters() const
    {
        const float s = static_cast<float>(Ratio::num) / static_cast<float>(Ratio::den);
        return value_ * s * s;
    }

    /** @brief Unary negation. */
    constexpr SquaredLength operator-() const { return SquaredLength(-value_); }

    /** @brief Adds another area of the same unit. */
    constexpr SquaredLength &operator+=(SquaredLength other)
    {
        value_ += other.value_;
        return *this;
    }

    /** @brief Subtracts another area of the same unit. */
    constexpr SquaredLength &operator-=(SquaredLength other)
    {
        value_ -= other.value_;
        return *this;
    }

    /** @brief Scales this area by a dimensionless scalar. */
    constexpr SquaredLength &operator*=(float s)
    {
        value_ *= s;
        return *this;
    }

    /** @brief Divides this area by a dimensionless scalar. */
    constexpr SquaredLength &operator/=(float s)
    {
        value_ /= s;
        return *this;
    }

private:
    float value_; ///< Raw value in Ratio² units.
};

// ─────────────────────────────────────────────────────────────────────────────
// Length arithmetic operators
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Adds two lengths, converting to the most-precise common unit.
 * @return The sum expressed in @c LengthCommonRatio<R1,R2> units.
 */
template <typename R1, typename R2>
constexpr auto operator+(Length<R1> a, Length<R2> b) -> Length<LengthCommonRatio<R1, R2>>
{
    using CR = LengthCommonRatio<R1, R2>;
    return Length<CR>(Length<CR>(a).GetValue() + Length<CR>(b).GetValue());
}

/**
 * @brief Subtracts two lengths, converting to the most-precise common unit.
 * @return The difference expressed in @c LengthCommonRatio<R1,R2> units.
 */
template <typename R1, typename R2>
constexpr auto operator-(Length<R1> a, Length<R2> b) -> Length<LengthCommonRatio<R1, R2>>
{
    using CR = LengthCommonRatio<R1, R2>;
    return Length<CR>(Length<CR>(a).GetValue() - Length<CR>(b).GetValue());
}

/**
 * @brief Scales a length by a dimensionless scalar (length × scalar).
 * @return A length in the same unit as @p a.
 */
template <typename R>
constexpr Length<R> operator*(Length<R> a, float s) { return Length<R>(a.GetValue() * s); }

/**
 * @brief Scales a length by a dimensionless scalar (scalar × length).
 * @return A length in the same unit as @p a.
 */
template <typename R>
constexpr Length<R> operator*(float s, Length<R> a) { return Length<R>(a.GetValue() * s); }

/**
 * @brief Divides a length by a dimensionless scalar.
 * @return A length in the same unit as @p a.
 */
template <typename R>
constexpr Length<R> operator/(Length<R> a, float s) { return Length<R>(a.GetValue() / s); }

/**
 * @brief Multiplies two lengths to produce an area.
 *
 * Both operands are converted to the common unit before multiplication, so
 * the result is a @c SquaredLength in that common unit.
 * For example, @c 3_cm * 2_m → @c SquaredLength<centi>(600) (600 cm² = 0.06 m²).
 *
 * @return Area in @c SquaredLength<LengthCommonRatio<R1,R2>> units.
 */
template <typename R1, typename R2>
constexpr auto operator*(Length<R1> a, Length<R2> b) -> SquaredLength<LengthCommonRatio<R1, R2>>
{
    using CR = LengthCommonRatio<R1, R2>;
    return SquaredLength<CR>(Length<CR>(a).GetValue() * Length<CR>(b).GetValue());
}

/**
 * @brief Divides two lengths to produce a dimensionless ratio.
 *
 * Both operands are converted to the common unit before division.
 * For example, @c 1_m / 1_cm → @c 100.f
 *
 * @return The dimensionless ratio @p a / @p b.
 */
template <typename R1, typename R2>
constexpr float operator/(Length<R1> a, Length<R2> b)
{
    using CR = LengthCommonRatio<R1, R2>;
    return Length<CR>(a).GetValue() / Length<CR>(b).GetValue();
}

/**
 * @brief Divides an area by a length to produce a length.
 *
 * The @p len operand is converted to the area's unit (@p R1) before division,
 * so the result is a @c Length in @p R1 units.
 * For example, @c 100_cm² / 1_m → @c Length<centi>(1) (1 cm).
 *
 * @return A @c Length in @p R1 units.
 */
template <typename R1, typename R2>
constexpr Length<R1> operator/(SquaredLength<R1> area, Length<R2> len)
{
    const Length<R1> len_conv = len;
    return Length<R1>(area.GetValue() / len_conv.GetValue());
}

// ─────────────────────────────────────────────────────────────────────────────
// Length comparison operators
// ─────────────────────────────────────────────────────────────────────────────

/** @brief Returns true if @p a and @p b represent the same physical length. */
template <typename R1, typename R2>
constexpr bool operator==(Length<R1> a, Length<R2> b)
{
    using CR = LengthCommonRatio<R1, R2>;
    return Length<CR>(a).GetValue() == Length<CR>(b).GetValue();
}

/** @brief Returns true if @p a and @p b represent different physical lengths. */
template <typename R1, typename R2>
constexpr bool operator!=(Length<R1> a, Length<R2> b) { return !(a == b); }

/** @brief Returns true if @p a is strictly shorter than @p b. */
template <typename R1, typename R2>
constexpr bool operator<(Length<R1> a, Length<R2> b)
{
    using CR = LengthCommonRatio<R1, R2>;
    return Length<CR>(a).GetValue() < Length<CR>(b).GetValue();
}

/** @brief Returns true if @p a is shorter than or equal to @p b. */
template <typename R1, typename R2>
constexpr bool operator<=(Length<R1> a, Length<R2> b) { return !(b < a); }

/** @brief Returns true if @p a is strictly longer than @p b. */
template <typename R1, typename R2>
constexpr bool operator>(Length<R1> a, Length<R2> b) { return b < a; }

/** @brief Returns true if @p a is longer than or equal to @p b. */
template <typename R1, typename R2>
constexpr bool operator>=(Length<R1> a, Length<R2> b) { return !(a < b); }

// ─────────────────────────────────────────────────────────────────────────────
// Stream output (for debugging)
// ─────────────────────────────────────────────────────────────────────────────

/** @brief Writes a @c Length to @p os as @c "value [num/den m]". */
template <typename Ratio>
std::ostream &operator<<(std::ostream &os, Length<Ratio> len)
{
    return os << len.GetValue() << " [" << Ratio::num << "/" << Ratio::den << " m]";
}

/** @brief Writes a @c SquaredLength to @p os as @c "value [num/den m]²". */
template <typename Ratio>
std::ostream &operator<<(std::ostream &os, SquaredLength<Ratio> area)
{
    return os << area.GetValue() << " [" << Ratio::num << "/" << Ratio::den << " m]\xc2\xb2";
}

// ─────────────────────────────────────────────────────────────────────────────
// Named unit aliases
// ─────────────────────────────────────────────────────────────────────────────

using Millimeters = Length<std::milli>;    ///< 1 mm = 0.001 m
using Centimeters = Length<std::centi>;    ///< 1 cm = 0.01  m
using Meters      = Length<std::ratio<1>>; ///< SI base unit for length.
using Kilometers  = Length<std::kilo>;     ///< 1 km = 1000  m

using SquareMillimeters = SquaredLength<std::milli>;    ///< 1 mm² = 10⁻⁶ m²
using SquareCentimeters = SquaredLength<std::centi>;    ///< 1 cm² = 10⁻⁴ m²
using SquareMeters      = SquaredLength<std::ratio<1>>; ///< SI base unit for area.
using SquareKilometers  = SquaredLength<std::kilo>;     ///< 1 km² = 10⁶  m²

// ─────────────────────────────────────────────────────────────────────────────
// User-defined literals
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief User-defined literal suffixes for SI length units.
 *
 * Bring into scope with: @code using namespace si_literals; @endcode
 *
 * @code
 *   auto a = 30.0_mm;   // Millimeters
 *   auto b = 1.5_cm;    // Centimeters
 *   auto c = 2.0_m;     // Meters
 *   auto d = 0.5_km;    // Kilometers
 * @endcode
 */
namespace si_literals
{
    constexpr Millimeters operator""_mm(long double v)        { return Millimeters(static_cast<float>(v)); } ///< Millimetre literal.
    constexpr Millimeters operator""_mm(unsigned long long v) { return Millimeters(static_cast<float>(v)); } ///< Millimetre literal.
    constexpr Centimeters operator""_cm(long double v)        { return Centimeters(static_cast<float>(v)); } ///< Centimetre literal.
    constexpr Centimeters operator""_cm(unsigned long long v) { return Centimeters(static_cast<float>(v)); } ///< Centimetre literal.
    constexpr Meters      operator""_m(long double v)         { return Meters(static_cast<float>(v));      } ///< Metre literal.
    constexpr Meters      operator""_m(unsigned long long v)  { return Meters(static_cast<float>(v));      } ///< Metre literal.
    constexpr Kilometers  operator""_km(long double v)        { return Kilometers(static_cast<float>(v));  } ///< Kilometre literal.
    constexpr Kilometers  operator""_km(unsigned long long v) { return Kilometers(static_cast<float>(v));  } ///< Kilometre literal.
} // namespace si_literals
