# AnyBaseConversion
C++ single- header- only library to convert a number expressed in any base (using any characters to encode) to any other base (again, using any characters to encode).


**Dependencies**

Requires Boost -  https://boost.org Boost 1.76.0 is included in src/boost, but need not be downloaded if you already have Boost installed. AnyBaseConversion uses the Boost multiprecision library, specifically the cpp_int type and a defined cpp_dec_float subtype with precision of 1800 decimal places at base 10.


**Methods:**

***input*** method takes a string/wstring and either a Base (see below for defined bases) or a user- defined character set (e.g. "0123456789ab" could be used for base12 -  the user can choose their own encoding, as long as no symbol is repeated in the character set.)

***set_float_precision*** method takes an integer and sets the precision (maximum number of decimal places following the decimal point) for conversion of floats. Maximum possible precision is 5979 for a binary output -  see below for the highest possible precision at larger bases. Setting a precision higher than the maximum possible precision will result in the maximum precision being used and a warning being thrown.

Setting the float precision is not required. If no precision is set, it will default to 10.

***output_conversion_return*** is the preferred method for returning the output, because the ConversionReturn struct carries important information such as errors and warnings encountered. It either takes a Base (with option boolean as second parameter to output uppercase letters if hexadecimal is chosen) or a user- defined character set.

***output_string*** and ***output_wstring*** work in the same way as ***output_conversion_return*** but return only a string or a wstring respectively, with the converted number. If an error is encountered, an empty string/wstring is returned.


**Instructions**
The only public methods are the ones described above. All inputs are strings or wstrings, except for the Base enum (see below for pre- defined Bases) which can be used instead of a user- defined character set.

The object is able to convert an arbitrarily high integer to another base, plus a float with a precision up to the maximum listed below.

To use AnyBaseConversion, create a new empty BaseConversion object then use the input method above to enter the number and the base you are converting from. The use the output_conversion_return method above to enter the base you are converting to and return the number converted into that base. The base can be a pre- defined Base or it can be a user- defined string -  see Base encoding below for more information on defining your own base.


**ConversionReturn**
The preferred way to return the converted number is via a ConversionReturn struct. The struct contains much more information than just the resulting output, such as whether errors/warnings were encountered, the content of those errors/warnings, the maximum precision available at the requested output base, the actual precision used, whether the output of a converted float is rounded or exact, etc. See the code for more details.


**Precision**
The integer part of any conversion can be of arbitrary size. It is calculated separately to any post- floating point part of the conversion. Because of this, the precision never refers to significant figures -  the number of digits in the integer is irrelevant. It always refers to the number of places after the floating point. The default is 10 but it can be set to any number. However, there is a maximum number of possible places, beyond which the conversion will become inaccurate. The maximum number of possible places depends on the base of the output, and if a higher precision is requested the object will throw a warning and use the lower number as the precision. The maximum precision using bases up to 100 follows.

When the output base is...     ...the maximum possible precision is...
- 2                               5979
- 3                               3772
- 4                               2989
- 5                               2575
- 6                               2313
- 7                               2129
- 8                               1993
- 9                               1886
- 10                              1800
- 11                              1728
- 12                              1667
- 13                              1615
- 14                              1570
- 15                              1530
- 16                              1494
- 17                              1462
- 18                              1433
- 19                              1407
- 20                              1383
- 21                              1361
- 22                              1340
- 23                              1321
- 24                              1304
- 25                              1287
- 26                              1272
- 27                              1257
- 28                              1243
- 29                              1230
- 30                              1218
- 31                              1206
- 32                              1195
- 33                              1185
- 34                              1175
- 35                              1165
- 36                              1156
- 37                              1147
- 38                              1139
- 39                              1131
- 40                              1123
- 41                              1116
- 42                              1108
- 43                              1101
- 44                              1095
- 45                              1088
- 46                              1082
- 47                              1076
- 48                              1070
- 49                              1064
- 50                              1059
- 51                              1054
- 52                              1048
- 53                              1043
- 54                              1039
- 55                              1034
- 56                              1029
- 57                              1025
- 58                              1020
- 59                              1016
- 60                              1012
- 61                              1008
- 62                              1004
- 63                              1000
- 64                              996
- 65                              992
- 66                              989
- 67                              985
- 68                              982
- 69                              978
- 70                              975
- 71                              972
- 72                              969
- 73                              966
- 74                              962
- 75                              959
- 76                              957
- 77                              954
- 78                              951
- 79                              948
- 80                              945
- 81                              943
- 82                              940
- 83                              937
- 84                              935
- 85                              932
- 86                              930
- 87                              928
- 88                              925
- 89                              923
- 90                              921
- 91                              918
- 92                              916
- 93                              914
- 94                              912
- 95                              910
- 96                              908
- 97                              905
- 98                              903
- 99                              901
- 100                             900


**Pre- defined bases**
Pre- defined bases which can be entered as an enum are:
binary             "01"
ternary            "012"
quaternary         "0123"
quinary            "01234"
senary             "012345"
septenary          "0123456"
octal              "01234567"
nonary             "012345678"
decimal            "0123456789"
undecimal          "0123456789a"
duodecimal*        "0123456789ab"
docenal*           "0123456789ab"
tridecimal         "0123456789abc"
tetradecimal       "0123456789abcd"
pentadecimal       "0123456789abcde"
hexadecimal        "0123456789abcdef"
vigesimal          "0123456789abcdefghjk"
tetravigesimal     "0123456789abcdefghjkmnpq"
base32             "abcdefghijklmnopqrstuvwxyz234567"
base36             "0123456789abcdefghijklmnopqrstuvwxyz"
base58             "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"
base64             "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
*\* Duodecimal and docenal are alternative names for the same base 12 character set.


**Base encoding**

To define your own digit symbols, simply enter a string in the user- defined character set which starts with the digit for 0, then 1 and so on. For example, "0123456789abcdef" is the inbuilt character set for hexadecimal, but a user could enter "ABCDEFGHIJKLMNOP" as the encoding for the number going in or the number coming out if they preferred.

**Example:**
```
#include <iostream>
#include <string>
#include "src/AnyBaseConversion.h"

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
      AnyBaseConversion::BaseConversion converter {};
      converter.set_float_precision(1001);

      bool base_contains_only_decimal_digits = true;
      std::string decimal_digits = AnyBaseConversion::get_char_set_from_base(AnyBaseConversion::decimal);
      for (size_t i = 0; i < base.size(); i++)
                        {
                                if (decimal_digits.find_first_of(base[i]) == std::string::npos)
                                        base_contains_only_decimal_digits = false;
                        }

      bool base_matches_defined_digits = false;
      bool defined_digits_match_found = false;
      std::vector<AnyBaseConversion::BaseInformation>::iterator it = AnyBaseConversion::base_information_vector.begin();
      while (!defined_digits_match_found && it != AnyBaseConversion::base_information_vector.end() && base_contains_only_decimal_digits)
                        {
                                AnyBaseConversion::BaseInformation current_base_info = *it;
                                if (current_base_info.digits.size() == std::stoi(base, nullptr, 10))
                                {
                                        defined_digits_match_found = true;
                                        converter.input(number, current_base_info.base);
                                }
                                it++;
                        }
                        if (!defined_digits_match_found)
        converter.input(number, base);

      for (AnyBaseConversion::BaseInformation base_info : AnyBaseConversion::base_information_vector)
                        {
                                AnyBaseConversion::ConversionReturn abc = converter.output_conversion_return(base_info.base);
                                if (abc.errors.size() > 0)
                                {
                                        for (std::string & error_tmp : abc.errors)
                                                std::cout << "Error:" << error_tmp << std::endl;
                                }
                                if (abc.warnings.size() > 0)
                                {
                                        for (std::string & warning_tmp : abc.warnings)
                                                std::cout << "Warning:" + warning_tmp << std::endl;
                                }
                                std::cout << "In base " + std::to_string(abc.output_base) << " this number is " + AnyBaseConversion::convert_wstring_to_string(abc.output) << std::endl;
                                std::cout << "Precision used in this calculation: " + std::to_string(abc.precision) << std::endl;
                                std::cout << "The number has " << (abc.rounded ? "" : "not ") << "been rounded." << std::endl;
                        }
    }
  }
  return 0;
}
```
