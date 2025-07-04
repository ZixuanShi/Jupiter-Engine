// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

#include "Core/Types/Enum.h"

export module UnitTests_Enum;

import UnitTests_Enum_Global;
import jpt.Utilities;

JPT_ENUM_UINT8(EFruit_Local,
    Apple = 5,
    Banana,
    Orange = 9,
    Grape);
template<typename Type>
bool UnitTests_Enum_NonSequence()
{
    // Static data
    JPT_ENSURE(Type::Count() == 4);
    JPT_ENSURE(Type::Min() == Type::Apple);
    JPT_ENSURE(Type::Max() == Type::Grape);

    // Comparing
    Type fruit(Type::Banana);
    JPT_ENSURE(fruit == 6);
    JPT_ENSURE(fruit == "Banana");
    JPT_ENSURE(fruit == Type::Banana);
    JPT_ENSURE(fruit > Type::Apple);
    JPT_ENSURE(fruit < Type::Orange);
    JPT_ENSURE(fruit != Type::Grape);

    // Math operations
    --fruit;
    JPT_ENSURE(fruit == 5);
    JPT_ENSURE(fruit == "Apple");
    JPT_ENSURE(fruit == Type::Apple);

    ++fruit;
    JPT_ENSURE(fruit == 6);
    JPT_ENSURE(fruit == "Banana");
    JPT_ENSURE(fruit == Type::Banana);

    fruit += 3;
    JPT_ENSURE(fruit == 9);
    JPT_ENSURE(fruit == "Orange");
    JPT_ENSURE(fruit == Type::Orange);

    fruit -= 3;
    JPT_ENSURE(fruit == 6);
    JPT_ENSURE(fruit == "Banana");
    JPT_ENSURE(fruit == Type::Banana);

    JPT_ENSURE(fruit + 3 == 9);
    JPT_ENSURE(fruit + 3 == "Orange");
    JPT_ENSURE(fruit + 3 == Type::Orange);

    JPT_ENSURE(fruit - 1 == 5);
    JPT_ENSURE(fruit - 1 == "Apple");
    JPT_ENSURE(fruit - 1 == Type::Apple);

    // Assigning
    fruit = Type::Orange;
    JPT_ENSURE(fruit == 9);
    JPT_ENSURE(fruit == "Orange");
    JPT_ENSURE(fruit == Type::Orange);

    // Iterating
    // for (const auto& pair : fruit)
    for (const auto& [key, value] : fruit) // Structure binding is available too
    {
        if (key == 5)
            JPT_ENSURE(value == "Apple");

        if (key == 6)
            JPT_ENSURE(value == "Banana");

        if (key == 9)
            JPT_ENSURE(value == "Orange");

        if (key == 10)
            JPT_ENSURE(value == "Grape");
    }

    // Assigning with different sized integer
    // size_t
    size_t i = 5;
    fruit = i;
    JPT_ENSURE(fruit == 5);
    JPT_ENSURE(fruit == Type::Apple);
    JPT_ENSURE(fruit == "Apple");

    // int64
    int64 j = 9;
    fruit = j;
    JPT_ENSURE(fruit == 9);
    JPT_ENSURE(fruit == Type::Orange);
    JPT_ENSURE(fruit == "Orange");

    return true;
}

JPT_ENUM_UINT8(EWeapon_Local,
    Sword,
    Spear,
    Hammer,
    Axe);
template<typename Type>
bool UnitTests_Enum_Sequence()
{
    // Static data
    JPT_ENSURE(Type::Count() == 4);
    JPT_ENSURE(Type::Min() == Type::Sword);
    JPT_ENSURE(Type::Max() == Type::Axe);

    // Comparing
    Type weapon(Type::Spear);
    JPT_ENSURE(weapon == 1);
    JPT_ENSURE(weapon == "Spear");
    JPT_ENSURE(weapon == Type::Spear);
    JPT_ENSURE(weapon > Type::Sword);
    JPT_ENSURE(weapon < Type::Hammer);
    JPT_ENSURE(weapon != Type::Axe);

    // Math operations
    --weapon;
    JPT_ENSURE(weapon == 0);
    JPT_ENSURE(weapon == "Sword");
    JPT_ENSURE(weapon == Type::Sword);

    ++weapon;
    JPT_ENSURE(weapon == 1);
    JPT_ENSURE(weapon == "Spear");
    JPT_ENSURE(weapon == Type::Spear);

    // Assigning
    weapon = Type::Axe;
    JPT_ENSURE(weapon == 3);
    JPT_ENSURE(weapon == "Axe");
    JPT_ENSURE(weapon == Type::Axe);

    // for (const auto& pair : fruit)
    // Iterating. This works on sequential enums
    for (auto i = Type::Min(); i < Type::Max(); ++i)
    {
        Type e(i);
        if (e == Type::Sword)
        {
            JPT_ENSURE(e == "Sword");
        }
        if (e == Type::Spear)
        {
            JPT_ENSURE(e == "Spear");
        }
        if (e == Type::Hammer)
        {
            JPT_ENSURE(e == "Hammer");
        }
        if (e == Type::Axe)
        {
            JPT_ENSURE(e == "Axe");
        }
    }

    return true;
}

JPT_ENUM_UINT16(EFlag_Local,
    A = (1 << 0),
    B = (1 << 1),
    C = (1 << 2),
    D = (1 << 3),
    E = (1 << 4),
    F = (1 << 5),
    G = (1 << 6),
    H = (1 << 7),
    I = (1 << 8),
    J = (1 << 9),
    K = (1 << 10),
    L = (1 << 11),
    M = (1 << 12),
    N = (1 << 13),
    O = (1 << 14),
    P = (1 << 15)
);

bool UnitTests_Enum_Flag()
{
    EFlag_Local flag(EFlag_Local::A | EFlag_Local::G | EFlag_Local::P);

    // |
    JPT_ENSURE(!flag.Has(EFlag_Local::B));
    flag |= EFlag_Local::B;

    // &
    JPT_ENSURE(flag.Has(EFlag_Local::A));
    JPT_ENSURE(flag.Has(EFlag_Local::B));
    JPT_ENSURE(flag.Has(EFlag_Local::G));
    JPT_ENSURE(flag.Has(EFlag_Local::P));
    JPT_ENSURE(!flag.Has(EFlag_Local::C));

    flag &= EFlag_Local::B;
    JPT_ENSURE(flag == EFlag_Local::B);
    JPT_ENSURE(flag.Has(EFlag_Local::B));
    JPT_ENSURE(!flag.Has(EFlag_Local::A));
    JPT_ENSURE(!flag.Has(EFlag_Local::G));
    JPT_ENSURE(!flag.Has(EFlag_Local::P));

    // ^
    flag |= EFlag_Local::A;
    flag ^= EFlag_Local::B;
    JPT_ENSURE(flag == EFlag_Local::A);
    JPT_ENSURE(!flag.Has(EFlag_Local::B));

    // ~
    flag = ~flag;
    for (const auto& [key, value] : flag)
    {
        if (key == EFlag_Local::A)
        {
            JPT_ENSURE(!flag.Has(EFlag_Local::Values(key)));
        }
        else
        {
            JPT_ENSURE(flag.Has(EFlag_Local::Values(key)));
        }
    }

    return true;
}

JPT_ENUM_UINT32(EColor_Local,
    Red    = 0xFF000000,
    Green  = 0x00FF0000,
    Blue   = 0x0000FF00,
    Alpha  = 0x000000FF,
    
    //White  = Red | Green | Blue | Alpha,
    //Black  = 0x00000000,
    //Purple = Red | Blue,
    Yellow = 0xFFFF0000,
    //Cyan   = Green | Blue,
);
bool UnitTests_Enum_Hex()
{
    EColor_Local color(EColor_Local::Red);
    color |= EColor_Local::Green;
    
    JPT_ENSURE(color == EColor_Local::Yellow);

    return true;
}

static bool StringToEnum()
{
    EColor_Local color("Green");
    JPT_ENSURE(color == EColor_Local::Green);
    JPT_ENSURE(color == 0x00FF0000);
    JPT_ENSURE(color < EColor_Local::Yellow);

    color = "Red";
    JPT_ENSURE(color == EColor_Local::Red);
    JPT_ENSURE(color == 0xFF000000);
    JPT_ENSURE(color < EColor_Local::Yellow);

    return true;
}

export bool RunUnitTests_Enum()
{
    JPT_ENSURE(UnitTests_Enum_NonSequence<EFruit_Local>());
    JPT_ENSURE(UnitTests_Enum_NonSequence<EFruit_Global>());
    JPT_ENSURE(UnitTests_Enum_NonSequence<GlobalEnumContainer::EFruit_Nested>());

    JPT_ENSURE(UnitTests_Enum_Sequence<EWeapon_Local>());
    JPT_ENSURE(UnitTests_Enum_Sequence<EWeapon_Global>());
    JPT_ENSURE(UnitTests_Enum_Sequence<GlobalEnumContainer::EWeapon_Nested>());

    JPT_ENSURE(UnitTests_Enum_Flag());
    JPT_ENSURE(UnitTests_Enum_Hex());
    JPT_ENSURE(StringToEnum());

    return true;
}
