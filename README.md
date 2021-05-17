# AnyBaseConversion
C++ single-header-only library to convert a number expressed in any base (using any characters to encode) to any other base (again, using any characters to encode).

**Dependencies**
Requires Boost - https://boost.org Boost 1.76.0 is included in src/boost, but need not be downloaded if you already have Boost installed. AnyBaseConversion uses the multiprecision library, specifically the cpp_int type and a defined cpp_dec_float subtype with precision of 1000 decimal places.

**Methods:**
    **input** method takes a string and either a Base (binary, octal, decimal or hexadecimal) or a user-defined character set (e.g. "0123456789ab" could be used for base12 - the user can choose their own encoding, as long as no symbol is repeated in the character set.)

    **set_float_precision** method takes an integer and sets the precision (maximum number of decimal places following the decimal point) for conversion of floats. Maximum possible precision is 1000. Setting the float precision is not required. If a float is entered, the precision will default to 10.

    **output_with_new_base** method either takes a Base (with option boolean as second parameter to output uppercase letters if hexadecimal is chosen) or a user-defined character set.

**Instructions**
The only public methods are the three described above. All inputs are strings, except for the Base enum (binary, octal, decimal or hexadecimal) which can be used instead of a user-defined character set.

The limits of the object's ability to handle large numbers have not yet been tested, but it should be able to handle arbitrarily large integers, and floats with an arbitrary number of digits before the decimal point and up to one thousand digits after the decimal point.

The object will return a string beginning "Error:" to the output_with_new_base method if it detects an error. Errors can be caused by digits repeated in the data set, or digits not matching the data set requested.

**Warning on floats**
An integer can always be converted to an exact integer in a different base. Not so for floats. For example, 1.1 in base3 (using "012" as digits) is exactly one and one third, which cannot be expressed exactly in decimal. In initial testing, the object returned good results for floats up to 10 digits long, however the final digits is rounded and a float should never be assumed to be an exact conversion.

**Encoding**
To define your own digit symbols, simply enter a string in the user-defined character set which starts with the digit for 0, then 1 and so on. For example, "0123456789abcdef" is the inbuilt character set for hexadecimal, but a user could enter "ABCDEFGHIJKLMNOP" as the encoding for the number going in or the number coming out if they preferred.

**Example:**
```
#include <iostream>
#include "AnyBaseConversion.h"

  int main()
	{
		std::setlocale(LC_ALL, "en_US.utf8");
    std::string number;
    while (number != "exit")
    {
      std::cout << "Enter a number in any base:" << std::endl;
      std::cin >> number;
      if (number != "exit")
      {
        std::cout << "Enter the digits encoding the number (or 2 for binary, 8 for octal, 10 for decimal, 16 for hexadecimal):" << std::endl;
        std::string base;
        std::cin >> base;
        AnyBaseConversion converter {};
        if (base == "2")
          converter.input(number, binary);
        else if (base == "8")
          converter.input(number, octal);
        else if (base == "10")
          converter.input(number, decimal);
        else if (base == "16")
          converter.input(number, hexadecimal);
        else
          converter.input(number, base);
        std::cout << "Enter the digit encoding you want the number returned in (or 2 for binary, 8 for octal, 10 for decimal, 16 for hexadecimal):" << std::endl;
        base = "";
        std::cin >> base;
        if (base == "2")
          std::cout << "The number inputted converts to binary: " << converter.output_with_new_base(binary) << std::endl;
        else if (base == "8")
          std::cout << "The number inputted converts to octal: " << converter.output_with_new_base(octal) << std::endl;
        else if (base == "10")
          std::cout << "The number inputted converts to decimal: " << converter.output_with_new_base(decimal) << std::endl;
        else if (base == "16")
          std::cout << "The number inputted converts to hexadecimal: " << converter.output_with_new_base(hexadecimal) << std::endl;
        else
          std::cout << "The number inputted converts to: " << converter.output_with_new_base(base) << std::endl;
      }
    }
    return 0;
  }
```
