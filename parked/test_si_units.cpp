#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include "si_units.hpp"

using namespace si_literals;
using Catch::Approx;

// ─────────────────────────────────────────────────────────────────────────────
// Construction & raw value
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Length default-constructs to zero", "[si_units]")
{
    CHECK(Millimeters{}.GetValue() == Approx(0.f));
    CHECK(Centimeters{}.GetValue() == Approx(0.f));
    CHECK(Meters{}.GetValue()      == Approx(0.f));
    CHECK(Kilometers{}.GetValue()  == Approx(0.f));
}

TEST_CASE("Length stores explicit value in native unit", "[si_units]")
{
    CHECK(Centimeters(5.f).GetValue() == Approx(5.f));
    CHECK(Meters(3.f).GetValue()      == Approx(3.f));
}

// ─────────────────────────────────────────────────────────────────────────────
// User-defined literals
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("User-defined literals produce correct values", "[si_units]")
{
    CHECK((5.0_mm).GetValue()  == Approx(5.f));
    CHECK((3.0_cm).GetValue()  == Approx(3.f));
    CHECK((2.0_m).GetValue()   == Approx(2.f));
    CHECK((1.0_km).GetValue()  == Approx(1.f));
    CHECK((10_mm).GetValue()     == Approx(10.f)); // wrap integer literal too
}

// ─────────────────────────────────────────────────────────────────────────────
// ToMeters conversion
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("ToMeters converts each unit correctly", "[si_units]")
{
    CHECK((1000.0_mm).ToMeters() == Approx(1.f));
    CHECK((100.0_cm).ToMeters()  == Approx(1.f));
    CHECK((1.0_m).ToMeters()     == Approx(1.f));
    CHECK((1.0_km).ToMeters()    == Approx(1000.f));
}

// ─────────────────────────────────────────────────────────────────────────────
// Implicit unit conversion
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Implicit conversion between units is exact", "[si_units]")
{
    Meters m = Centimeters(100.f);
    CHECK(m.GetValue() == Approx(1.f));

    Centimeters cm = Meters(1.f);
    CHECK(cm.GetValue() == Approx(100.f));

    Millimeters mm = Centimeters(1.f);
    CHECK(mm.GetValue() == Approx(10.f));

    Kilometers km = Meters(1000.f);
    CHECK(km.GetValue() == Approx(1.f));

    Centimeters cm2 = Kilometers(0.001f);
    CHECK(cm2.GetValue() == Approx(100.f));
}

// ─────────────────────────────────────────────────────────────────────────────
// Addition
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Adding same-unit lengths", "[si_units]")
{
    auto result = 3.0_cm + 2.0_cm;
    CHECK(result.GetValue() == Approx(5.f));
}

TEST_CASE("Adding different-unit lengths uses the most-precise common unit", "[si_units]")
{
    SECTION("cm + m → cm")
    {
        auto result = 50.0_cm + 1.0_m; // 50 cm + 100 cm = 150 cm
        CHECK(result.GetValue() == Approx(150.f));
    }

    SECTION("mm + cm → mm")
    {
        auto result = 5.0_mm + 1.0_cm; // 5 mm + 10 mm = 15 mm
        CHECK(result.GetValue() == Approx(15.f));
    }

    SECTION("m + km → m")
    {
        auto result = 500.0_m + 1.0_km; // 500 m + 1000 m = 1500 m
        CHECK(result.GetValue() == Approx(1500.f));
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Subtraction
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Subtracting different-unit lengths converts to common unit", "[si_units]")
{
    auto result = 1.0_m - 30.0_cm; // 100 cm - 30 cm = 70 cm
    CHECK(result.GetValue() == Approx(70.f));
}

// ─────────────────────────────────────────────────────────────────────────────
// Scalar multiplication and division
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Length * scalar and scalar * Length", "[si_units]")
{
    CHECK((2.0_cm * 3.f).GetValue() == Approx(6.f));
    CHECK((3.f * 2.0_cm).GetValue() == Approx(6.f));
}

TEST_CASE("Length / scalar", "[si_units]")
{
    CHECK((6.0_cm / 2.f).GetValue() == Approx(3.f));
}

// ─────────────────────────────────────────────────────────────────────────────
// Length × Length → SquaredLength
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Multiplying same-unit lengths produces SquaredLength", "[si_units]")
{
    auto area = 3.0_cm * 4.0_cm; // 12 cm²
    CHECK(area.GetValue()        == Approx(12.f));
    CHECK(area.ToSquareMeters()  == Approx(0.0012f)); // 12 × (0.01)² = 0.0012 m²
}

TEST_CASE("Multiplying different-unit lengths converts to common unit first", "[si_units]")
{
    // 1 cm × 1 m → common unit is cm → 1 cm × 100 cm = 100 cm²
    auto area = 1.0_cm * 1.0_m;
    CHECK(area.GetValue()       == Approx(100.f));
    CHECK(area.ToSquareMeters() == Approx(0.01f)); // 100 × (0.01)² = 0.0001… wait:
                                                   // 100 cm² = 100 × 10⁻⁴ m² = 0.01 m² ✓
}

TEST_CASE("SquaredLength ToSquareMeters for each unit", "[si_units]")
{
    CHECK(SquareMillimeters(1'000'000.f).ToSquareMeters() == Approx(1.f));
    CHECK(SquareCentimeters(10'000.f).ToSquareMeters()    == Approx(1.f));
    CHECK(SquareMeters(1.f).ToSquareMeters()              == Approx(1.f));
}

// ─────────────────────────────────────────────────────────────────────────────
// Length / Length → dimensionless float
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Dividing two lengths returns a dimensionless float", "[si_units]")
{
    float r1 = 1.0_m / 1.0_cm;
    CHECK(r1 == Approx(100.f));

    float r2 = 1.0_km / 1.0_m;
    CHECK(r2 == Approx(1000.f));

    float r3 = 1.0_cm / 1.0_mm;
    CHECK(r3 == Approx(10.f));
}

// ─────────────────────────────────────────────────────────────────────────────
// SquaredLength / Length → Length
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Dividing SquaredLength by Length returns a Length", "[si_units]")
{
    SECTION("same unit: 24 cm² / 4 cm = 6 cm")
    {
        auto area = 6.0_cm * 4.0_cm; // 24 cm²
        auto len  = area / 4.0_cm;
        CHECK(len.GetValue() == Approx(6.f));
    }

    SECTION("different units: 100 cm² / 1 m = 1 cm")
    {
        auto area = 1.0_cm * 1.0_m; // 100 cm²
        auto len  = area / 1.0_m;   // 100 cm² / 100 cm = 1 cm
        CHECK(len.GetValue() == Approx(1.f));
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Comparison operators
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Length comparison operators work across units", "[si_units]")
{
    CHECK(100.0_cm == 1.0_m);
    CHECK(1000.0_m == 1.0_km);
    CHECK(10.0_mm  == 1.0_cm);

    CHECK(1.0_m  > 99.0_cm);
    CHECK(99.0_cm < 1.0_m);
    CHECK(1.0_m  >= 100.0_cm);
    CHECK(1.0_m  <= 100.0_cm);
    CHECK(1.0_km != 999.0_m);
}

// ─────────────────────────────────────────────────────────────────────────────
// Unary negation
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Unary negation of a Length", "[si_units]")
{
    CHECK((-5.0_cm).GetValue()  == Approx(-5.f));
    CHECK((-2.0_m).GetValue()   == Approx(-2.f));
}

// ─────────────────────────────────────────────────────────────────────────────
// Compound assignment operators
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Compound assignment operators", "[si_units]")
{
    Centimeters cm(10.f);

    cm += Centimeters(5.f);
    CHECK(cm.GetValue() == Approx(15.f));

    cm -= Centimeters(3.f);
    CHECK(cm.GetValue() == Approx(12.f));

    cm *= 2.f;
    CHECK(cm.GetValue() == Approx(24.f));

    cm /= 4.f;
    CHECK(cm.GetValue() == Approx(6.f));
}

TEST_CASE("Compound += and -= accept different units", "[si_units]")
{
    Centimeters cm(50.f);
    cm += 1.0_m; // 1 m = 100 cm → total 150 cm
    CHECK(cm.GetValue() == Approx(150.f));

    cm -= 500.0_mm; // 500 mm = 50 cm → total 100 cm
    CHECK(cm.GetValue() == Approx(100.f));
}

// ─────────────────────────────────────────────────────────────────────────────
// Compile-time (constexpr) evaluation
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Length operations are usable at compile time", "[si_units]")
{
    constexpr Meters m       = Centimeters(250.f); // 2.5 m
    constexpr Meters doubled = m * 2.f;
    static_assert(doubled.GetValue() > 4.f, "2 × 2.5 m must be > 4 m");
    CHECK(m.GetValue()       == Approx(2.5f));
    CHECK(doubled.GetValue() == Approx(5.f));
}
