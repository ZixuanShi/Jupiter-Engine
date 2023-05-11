#include "JupiterPCH.h"
#include "StringAlgorithms.h"

namespace jpt
{
    size_t strlen(const char* inString)
    {
        if (!inString)
        {
            return 0;
        }

        size_t size = 0;
        while (inString[size] != '\0')
        {
            ++size;
        }

        return size;
    }

    char* itoa(int32 value, int32 base)
    {
        // Prepare data
        bool isNegative = false;	// Whether this value is negative or not
        char* result = nullptr;		// Final result string to return
        size_t index = 0;			// The index I'm using for char array, will be used as length of this value and string as well

        // Process 0
        if (value == 0)
        {
            result = new char[2];
            result[0] = '0';
            result[1] = '\0';
            return result;
        }
        // Process negative
        else if (value < 0)
        {
            isNegative = true;
            value = -value;
            ++index;
        }
        // Get Value's literal length
        int32 valueCopy = value;
        while (valueCopy > 0)
        {
            valueCopy /= base;
            ++index;
        }
        // Allocate Result string, + 1 for the end terminater
        result = new char[index + 1];

        // Append string terminator at the end
        result[index] = '\0';
        --index;

        // Process each digit
        while (value > 0)
        {
            int digit = value % base;

            // For different base, process differently
            if (digit > 9)
            {
                result[index] = static_cast<char>(digit - 10) + 'A';
            }
            else
            {
                result[index] = static_cast<char>(digit + '0');
            }

            // Update index and cut last digit of value
            --index;
            value /= base;
        }

        // If value was negative, add '-' at the beginning
        if (isNegative)
        {
            result[index] = '-';
            --index;
        }

        // Return result
        return result;
    }

    constexpr uint32 StringHash32(const char* const str, const uint32 value) noexcept
    {
        return (str[0] == '\0') ? value : StringHash32(&str[1], (value ^ uint32(str[0])) * 0x1000193);
    }

    constexpr uint64 StringHash64(const char* const str, const uint64 value) noexcept
    {
        return (str[0] == '\0') ? value : StringHash64(&str[1], (value ^ uint64(str[0])) * 0x100000001b3);
    }
}

