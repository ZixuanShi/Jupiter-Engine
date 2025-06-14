// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Math_Settings;

export namespace jpt
{
    enum class RotationOrder
    {
        XYZ,
        XZY,
        YXZ,    // Default
        YZX,
        ZXY,
        ZYX,
    };

    enum class AngleUnit
    {
        Degrees,
        Radians,
    };

    enum class LengthUnit
    {
        Meters,
        Centimeters,
        Millimeters,
        Inches,
        Feet,
        Yards,
        Miles,
        Kilometers,
    };

    enum class MassUnit
    {
        Kilograms,
        Grams,
        Milligrams,
        Pounds,
    };

    class MathSettings
    {
    public:
        inline static RotationOrder RotationOrder = RotationOrder::YXZ;
        inline static AngleUnit     AngleUnit     = AngleUnit::Degrees;
    };
}