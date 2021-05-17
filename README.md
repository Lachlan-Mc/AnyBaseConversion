# AnyBaseConversion
C++ single-header-only library to convert a number expressed in any base to any other base.

**Dependencies**
Requires Boost - https://boost.org Boost 1.76.0 is included in src/boost (inclusion in progress - AnyBaseConversion is still usable if AnyBaseConversion.h is downloaded and Boost is installed).

**Methods:**

  input method takes a string and either a Base (binary, octal, decimal or hexadecimal) or a user-defined character set (e.g. "0123456789ab" could be used for base12 - the user can choose their own encoding, as long as no symbol is repeated in the character set.)

  set_float_precision method takes an integer and sets the precision (maximum number of decimal places following the decimal point) for conversion of floats. Maximum possible precision is 1000.

  output_with_new_base method either takes a Base (with option boolean as second parameter to output uppercase letters if hexadecimal is chosen) or a user-defined character set.


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
