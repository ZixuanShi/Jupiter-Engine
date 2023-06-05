## Class Layout Example

Assuming it's a header file with a class called Foo
// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
    /* Forward Declarations */

	/* Short description for what this class does */
	class JPT_API Foo
	{
	public:
        /* using statements */
        /* Internal Structs */

	private:
        /* Private member variables. Obey memory alignment rule */

        // If there's no custom default constructor. Initialize the member with default member initializer
        // If there is a custom default constructor(s). Do not initialize value inside class. Initialize it in the constructor with member initializers list instead.
        int32 m_data = 0;

	public:
		/* Constructors and Destructors. Strictly obey the rule of five */

        /* Essential overridden functions for life cycle. Examples:
                Init();
                Update();
                Clean();
         */

        /* Operator Overloadings */

    public:
        /* Public Utils Functions. Examples:
                OnKeyDown(); 
                OnKeyUp();
                Execute(); 
         */

    public:
        /* Getters and Setters for members. As members order */

	private:
        /* Private helper functions */
	};
}

Non-Trivial Functions should have a comment section above the signature explaining what it does, parameters it takes, and what it returns, etc. Example:
/* Convert from int32 to a char pointer holding the integer's value as string literal 
    @param value:        The value to convert to char*
    @param base:         The base of the value. Default to decimal as 10. Could be binary, oct, hex. 
    @return A char pointer pointing to the memory where we store the converted number's string literal */
JPT_API inline char* itoa(int32 value, int32 base = 10);

Assuming it's a cpp file
// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "Foo.h"    // The corresponding header

/* #include any necessary headers from Jupiter in alphabetical order. Should use double quotes #include "Header.h" */

/* #include any necessary headers from 3rd parties in alphabetical order. Should use angle brackets #include <Header.h> */

namespace jpt
{
    /* Local functions only being used in this Cpp file. Prefix with loc */

    /* Function implementations */
}

## Committing Message Convention
https://www.freecodecamp.org/news/how-to-write-better-git-commit-messages/