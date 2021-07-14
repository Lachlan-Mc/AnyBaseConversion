#ifndef ANYBASECONVERSION_H_INCLUDED
#define ANYBASECONVERSION_H_INCLUDED

#include <string>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <codecvt>
#include <locale>
#include <cstdint>
#include <cwchar>
#include <locale.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/container/flat_map.hpp>

//User-definable constants.
constexpr size_t        float_precision_default {10}; //Return of floats defaults to this number of decimal places, or more if the user manually requests a higher precision.


namespace AnyBaseConversion
{
	enum Base
	{
		binary,
		ternary,
		quaternary,
		quinary,
		senary,
		septenary,
		octal,
		nonary,
		decimal,
		undecimal,
		duodecimal,
		docenal,
		tridecimal,
		tetradecimal,
		pentadecimal,
		hexadecimal,
		vigesimal,
		tetravigesimal,
		base32,
		base36,
		base58,
		base64,
	};

        struct BaseInformation
        {
                std::string             name; //The name of the base as a string.
                AnyBaseConversion::Base base; //The AnyBaseConversion::Base enum of the base.
                std::string             digits; //String defining the digits in the base. Digits are in order and are all distinct.
                bool                    conversion_to_upper_case; //Bases with definitions that include only lower-case letters can also be input/output as upper-case. E.g. hexadecimal can be equally represented "0123456789abcdef" or "0123456789ABCDEF".
        };

        std::vector<AnyBaseConversion::BaseInformation> base_information_vector {
                {"binary",              AnyBaseConversion::binary,              "01",                                                                   false},
                {"ternary",             AnyBaseConversion::ternary,             "012",                                                                  false},
                {"quaternary",          AnyBaseConversion::quaternary,          "0123",                                                                 false},
                {"quinary",             AnyBaseConversion::quinary,             "01234",                                                                false},
                {"senary",              AnyBaseConversion::senary,              "012345",                                                               false},
                {"septenary",           AnyBaseConversion::septenary,           "0123456",                                                              false},
                {"octal",               AnyBaseConversion::octal,               "01234567",                                                             false},
                {"nonary",              AnyBaseConversion::nonary,              "012345678",                                                            false},
                {"decimal",             AnyBaseConversion::decimal,             "0123456789",                                                           false},
                {"undecimal",           AnyBaseConversion::undecimal,           "0123456789a",                                                          true},
                {"duodecimal",          AnyBaseConversion::duodecimal,          "0123456789ab",                                                         true},
                {"docenal",             AnyBaseConversion::docenal,             "0123456789ab",                                                         true},
                {"tridecimal",          AnyBaseConversion::tridecimal,          "0123456789abc",                                                        true},
                {"tetradecimal",        AnyBaseConversion::tetradecimal,        "0123456789abcd",                                                       true},
                {"pentadecimal",        AnyBaseConversion::pentadecimal,        "0123456789abcde",                                                      true},
                {"hexadecimal",         AnyBaseConversion::hexadecimal,         "0123456789abcdef",                                                     true},
                {"vigesimal",           AnyBaseConversion::vigesimal,           "0123456789abcdefghjk",                                                 true},
                {"tetravigesimal",      AnyBaseConversion::tetravigesimal,      "0123456789abcdefghjkmnpq",                                             true},
                {"base32",              AnyBaseConversion::base32,              "abcdefghijklmnopqrstuvwxyz234567",                                     true},
                {"base36",              AnyBaseConversion::base36,              "0123456789abcdefghijklmnopqrstuvwxyz",                                 true},
                {"base58",              AnyBaseConversion::base58,              "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz",           false},
                {"base64",              AnyBaseConversion::base64,              "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/",     false}
        };

        //Given an AnyBaseConversion::Base input, returns the corresponding digits definition.
        std::string get_char_set_from_base(AnyBaseConversion::Base base)
        {
                bool char_set_found {false};
                std::vector<BaseInformation>::iterator it = base_information_vector.begin();
                std::string char_set;
                while (!char_set_found)
                {
                        BaseInformation current_base_info = *it;
                        if (current_base_info.base == base)
                        {
                                char_set = current_base_info.digits;
                                char_set_found = true;
                        }
                        it++;
                }
                return char_set;
        }

        //Given an AnyBaseConversion::Base input, returns a bool  which indicates whether the base in question has an uppercase option (e.g. hexadecimal can use the digits "0123456789abcdef" or "0123456789ABCDEF".
        bool does_base_allow_use_uppercase(AnyBaseConversion::Base base)
        {
                bool base_allows_uppercase = false;
                bool base_information_found = false;
                std::vector<BaseInformation>::iterator it = base_information_vector.begin();
                while (!base_information_found)
                {
                        BaseInformation current_base_info = *it;
                        if (current_base_info.base == base)
                        {
                                base_information_found = true;
                                base_allows_uppercase = current_base_info.conversion_to_upper_case;
                        }
                        it++;
                }
                return base_allows_uppercase;
        }

        //Utilities

        //Reverses a wstring and returns the reversed wstring.
        std::wstring reverse_wstring(std::wstring &str)
        {
                std::wstring string_out {};
                for (size_t i = str.length(); i > 0; --i)
                {
                        string_out += str[i-1];
                }
                return string_out;
        }

        //Calculates base to the power of exp.
        boost::multiprecision::cpp_int exponent(size_t base, size_t exp)
        {
                        boost::multiprecision::cpp_int result { 1 };
                        boost::multiprecision::cpp_int basecu { base };
                        for (size_t i = 1; i <= exp; ++i)
                        {
                                result = result * basecu;
                        }

                        return result;
        }

        //Converts a wstring to an integer using a given base.
        boost::multiprecision::cpp_int convert_number_wstring_to_integer(std::wstring &num_string, std::wstring &char_set)
        {
                size_t base = char_set.length();
                boost::multiprecision::cpp_int num { 0 };
                std::wstring use_string = reverse_wstring(num_string);
                size_t value { 0 };
                for (size_t i { 0 }; i < use_string.length(); i++)
                {
                        value = char_set.find_first_of(use_string[i]);
                        num += (value * exponent(base, i));
                }
                return num;
        }

        //Converts an integer to a wstring in a given base.
        std::wstring convert_integer_to_wstring(boost::multiprecision::cpp_int num, std::wstring &char_set)
        {
                size_t base = char_set.length();
                std::wstring output_string {};
                boost::multiprecision::cpp_int remainder;
                size_t remainder_st {};
                while (num != 0)
                {
                        remainder = num % base;
                        remainder_st = static_cast<size_t>(boost::multiprecision::number(remainder));
                        num = num / base;
                        output_string = char_set[remainder_st] + output_string;
                }
                return output_string;
        }

        //Converts a string to a wstring.
        std::wstring convert_string_to_wstring(std::string &s)
        {
                std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
                return converter.from_bytes(s);
        }

        //Converts a wstring to a string.
        std::string convert_wstring_to_string(std::wstring &s)
        {
                std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
                return converter.to_bytes(s);
        }

        //Convert a Base enum to its corresponding character set, as a wstring.
        std::wstring convert_base_to_char_set(Base base)
        {
                std::string char_set = get_char_set_from_base(base);
                return convert_string_to_wstring(char_set);
        }

        //Counts the number of times a character appears in a wstring.
        size_t count_instances_of_wchar_t_in_wstring(wchar_t character, std::wstring &str)
        {
                size_t num { 0 };
                for (wchar_t c : str)
                        if (c == character)
                                num++;
                return num;
        }

        //ConversionReturn is the preferred form of return of the BaseConversion object.
        struct ConversionReturn
        {
                bool                            errors_encountered; //Whether errors were encountered in the conversion.
                bool                            warnings_encountered; //Whether warnings were encountered in the conversion.
                std::vector<std::string>        errors; //Vector of errors encountered.
                std::vector<std::string>        warnings; //Vector of warnings encountered.
                std::wstring                    before_point; //The part of the output before the decimal point.
                std::wstring                    after_point; //The part of the output after the decimal point, if output is a float.
                wchar_t                         decimal_point_used; //The character used to indicate a decimal point, if output is a float.
                std::wstring                    output; //The whole of the output, converted.
                std::wstring                    input_digits; //The digit set which defines the input.
                std::wstring                    output_digits; //The digit set which defines the output.
                unsigned int                    input_base; //The number of digits in the input base.
                unsigned int                    output_base; //The number of digits in the output base.
                bool                            output_base_allows_use_uppercase; //Whether the output base allows the use of conversion of letter digits to uppercase.
                bool                            output_uppercase_used {false}; //Whether a conversion of letter digits to uppercase was in fact made.
                bool                            is_float; //Whether the input and output are floats. False = input and output are integers.
                unsigned int                    max_precision; //The maximum precision after the floating point of the selected output base, if a float.
                unsigned int                    precision; //The actual precision used in the output, if a float. May be more than the number of digits after the floating point if further digits were not needed.
                bool                            rounded; //Whether the output, if a float, had to be rounded to fit within the precision.
        };

        //Validation return carries information about whether a character set has been validated, and any errors which were encountered.
        struct ValidationReturn
        {
                bool valid; //Whether the inputted item has been validated.
                std::vector<std::string> error_messages; //Any error messages created during the validation.
        };

        //Defines a special float type with the largest precision possible.
	typedef boost::multiprecision::number<boost::multiprecision::cpp_dec_float<1800> > cpp_dec_float_huge;

	class BaseConversion
	{
	public:
		//Construct with a std::wstring encoded number, and an AnyBaseConversion defined Base.
		BaseConversion(std::wstring &number_s, Base base, bool throw_exception = false)
		{
			input(number_s, base, throw_exception);
		}

                //Construct with a std::wstring encoded number and a user-defined std::wstring definition of the numbering.
		BaseConversion(std::wstring &number_s, std::wstring &digits, bool throw_exception = false)
		{
			input(number_s, digits, throw_exception);
		}

                //Construct with a std::string encoded number, and an AnyBaseConversion defined Base.
		BaseConversion(std::string &number_s, Base base, bool throw_exception = false)
		{
			std::wstring number_w = convert_string_to_wstring(number_s);
			input(number_w, base, throw_exception);
		}

                //Construct with a std::string encoded number and a user-defined std::string definition of the numbering.
		BaseConversion(std::string &number_s, std::string &digits, bool throw_exception = false)
		{
			std::wstring number_w = convert_string_to_wstring(number_s);
			std::wstring digits_w = convert_string_to_wstring(digits);
			input(number_w, digits_w, throw_exception);
		}

                //Construct an empty object.
		BaseConversion()
		{
			//No action.
		}

                //Input a std::wstring number and an AnyBaseConversion::Base to an empty BaseConversion, or overwrite the existing user input if not empty.
		void input(std::wstring &number_w, Base base, bool throw_exception = false)
		{
			if (throw_exception)
				throw_exception = true;
                        std::wstring digits = convert_base_to_char_set(base);
			defined_input_digits = true;
			if (does_base_allow_use_uppercase(base))
			{
				std::wstring number_in_lower_case = number_w;
				std::transform(number_in_lower_case.begin(), number_in_lower_case.end(), number_in_lower_case.begin(), ::tolower);
				load_number(number_in_lower_case, digits);
			}
			else
				load_number(number_w, digits);
		}

		//Input a std::wstring number and a user-defined std::wstring numbering definition to an empty BaseConversion, or overwrite the existing user input if not empty.
		void input(std::wstring &number_w, std::wstring &digits_w, bool throw_exception = false)
		{
			if (throw_exception)
				use_exceptions = true;
                        load_number(number_w, digits_w);
		}

                //Input a std::string number and an AnyBaseConversion::Base to an empty BaseConversion, or overwrite the existing user input if not empty.
                void input(std::string &number_s, Base base, bool throw_exception = false)
                {
                        std::wstring number_w = convert_string_to_wstring(number_s);
			input(number_w, base, throw_exception);
                }

                //Input a std::string number and a user-defined std::string numbering definition to an empty BaseConversion, or overwrite the existing user input if not empty.
		void input(std::string &number_s, std::string &digits, bool throw_exception = false)
		{
		        std::wstring number_w = convert_string_to_wstring(number_s);
			std::wstring digits_w = convert_string_to_wstring(digits);
			input(number_w, digits_w, throw_exception);
		}

		//Set the precision (maximum number of digits after the decimal point) of the output float (if it is a float).
		void set_float_precision(int precision_given)
		{
			if (precision_given < 1)
                                set_warning("Warning: Illegal float precision. Try again with a float precision of at least 1.");
			precision_requested = precision_given;
                        float_precision_set = true;
		}

                //Output the number as a string in a defined base. If an error occurs, will output an empty string: "".
                //Allows some defined bases (e.g. hexadecimal) to convert digits which are letters to uppercase. Only available for those bases which already use lowercase letters but no uppercase letters as digits.
		std::string output_string(Base base, bool use_uppercase = false)
		{
			ConversionReturn conversion_return = generate_conversion_return(base, use_uppercase);
			return convert_wstring_to_string(conversion_return.output);
		}

		//Output the number as a string in a user-defined base. If an error occurs, will output an empty string: "".
		std::string output_string(std::string digits)
		{
                        std::wstring digits_w = convert_string_to_wstring(digits);
                        ConversionReturn conversion_return = generate_conversion_return(digits_w);
                        return convert_wstring_to_string(conversion_return.output);
		}

		//Output the number as a wstring in a defined base. If an error occurs, will output an empty string: "".
		std::wstring output_wstring(Base base, bool use_uppercase = false)
		{
			ConversionReturn conversion_return = generate_conversion_return(base, use_uppercase);
			return conversion_return.output;
		}

                //Output the number as a wstring in a user-defined base. If an error occurs, will output an empty string: "".
		std::wstring output_wstring(std::wstring digits_w)
		{
                        ConversionReturn conversion_return = generate_conversion_return(digits_w);
                        return conversion_return.output;
		}

		//It is recommended that you always get the return as a ConversionReturn. This will allow you to know if any errors or warnings occurred.

		//Output the number as a ConversionReturn in a defined base.
		AnyBaseConversion::ConversionReturn output_conversion_return(Base base, bool use_uppercase = false)
		{
		        ConversionReturn conversion_return = generate_conversion_return(base, use_uppercase);
		        return conversion_return;
		}

                //Output the number as a ConversionReturn in a user-defined wstring base.
		AnyBaseConversion::ConversionReturn output_conversion_return(std::wstring digits_w)
		{
		        ConversionReturn conversion_return = generate_conversion_return(digits_w);
		        return conversion_return;
		}

                //Output the number as a ConversionReturn in a user-defined string base.
		AnyBaseConversion::ConversionReturn output_conversion_return(std::string digits)
		{
		        ConversionReturn conversion_return = generate_conversion_return(convert_string_to_wstring(digits));
		        return conversion_return;
		}

        private:
		std::wstring			        input_front;
		std::wstring			        input_back;
		std::wstring			        input_digits;
		std::string			        decimal_points {".,"};
                bool                                    use_exceptions {false};
                bool                                    input_errors_encountered {false};
		bool                                    input_warnings_encountered {false};
		std::vector<std::string>                input_errors;
		std::vector<std::string>                input_warnings;
		bool                                    defined_input_digits {false};
		Base                                    base_used;
		wchar_t                                 input_decimal_point_used;
		bool                                    input_is_float {false};
		unsigned int                            precision_requested;
		bool                                    float_precision_set {false};


		void load_number(std::wstring &number_s, std::wstring char_set)
		{
			ValidationReturn validation_return = validate_digits(char_set);
			if (defined_input_digits || validation_return.valid)
			{
				input_front = number_s;
				input_digits = char_set;
				validate_input();
				break_into_float();
			}
			else if (!validation_return.valid)
                                set_errors(validation_return.error_messages);
		}


		ValidationReturn validate_digits(std::wstring digits) //For this function and the next one, need to return a bool and a string, with the error if there is one. Maybe return a vector of errors, all of which can be copied.
		{
			bool repeat_digit_found {false};
			size_t i {0};
			while (!repeat_digit_found && i < digits.length())
			{
				wchar_t digit_character = digits[i];
				if (count_instances_of_wchar_t_in_wstring(digit_character, digits) > 1)
					repeat_digit_found = true;
				i++;
			}
			ValidationReturn validation_return;
			if (repeat_digit_found)
                        {
                                validation_return.valid = false;
                                validation_return.error_messages.push_back("The inputted set of digits contains repeat characters. To be used for base conversion, every character must be unique.");
                        }
                        else
                                validation_return.valid = true;
                        return validation_return;
		}

		void validate_input()
		{
			bool illegal_character_found {false};
			bool decimal_point_found {false};
			size_t num {0};
			std::string test = convert_wstring_to_string(input_front);
			while (!illegal_character_found && num < input_front.length())
			{
				wchar_t wct = static_cast<char>(input_front[num]);
				if (count_instances_of_wchar_t_in_wstring(wct, input_digits) != 1)
				{
					std::wstring decimal_points_w = convert_string_to_wstring(decimal_points);
					if (count_instances_of_wchar_t_in_wstring(wct, decimal_points_w) == 1)
					{
						if (!decimal_point_found && num != 0 && num != input_front.size())
						{
							input_decimal_point_used = input_front[num];
							decimal_point_found = true;
							input_is_float = true;
						}
						else
							illegal_character_found = true;
					}
					else
						illegal_character_found = true;
				}
				num++;
			}
			if (illegal_character_found)
				set_error("Input string does not match formatting requirements. It may have illegal characters, or it may have more than one decimal point or a decimal point in the wrong place.");
		}

		void set_default_float_precision()
		{
		        if (!float_precision_set)
                        {
                                precision_requested = float_precision_default;
                                float_precision_set = true;
                        }
		}

		void break_into_float()
		{
			if (input_is_float)
			{
				set_default_float_precision();
				size_t decimal_point_location = input_front.find_first_of(input_decimal_point_used);
				input_back = input_front.substr((decimal_point_location + 1), std::wstring::npos);
				input_front = input_front.substr(0, (decimal_point_location));
			}
		}

		ConversionReturn generate_conversion_return(Base base, bool use_uppercase = false)
		{
			if (!does_base_allow_use_uppercase(base))
				use_uppercase = false;
			return generate_conversion_return(convert_base_to_char_set(base), use_uppercase, true);
		}

		ConversionReturn generate_conversion_return(std::wstring output_digits, bool use_uppercase = false, bool defined_base_requested = false)
		{
		        ConversionReturn conversion_return {};
		        populate_conversion_return (conversion_return);
		        ValidationReturn validation_return = validate_digits(output_digits);
		        if (validation_return.valid)
		        {
		                set_output_info(conversion_return, output_digits);
                                convert_front(conversion_return);
                                if (conversion_return.is_float)
                                                convert_back(conversion_return);
                                if (defined_base_requested && use_uppercase)
                                        convert_output_digits_to_uppercase(conversion_return);
		        }
		        else
                                set_errors(validation_return.error_messages);
                        set_output(conversion_return);
                        return conversion_return;
		}

		void populate_conversion_return(ConversionReturn &conversion_return)
		{
		        conversion_return.errors_encountered = input_errors_encountered;
		        conversion_return.warnings_encountered = input_warnings_encountered;
		        conversion_return.errors = input_errors;
		        conversion_return.warnings = input_warnings;
		        conversion_return.is_float = input_is_float;
		        if (conversion_return.is_float)
                                conversion_return.decimal_point_used = input_decimal_point_used;
                        conversion_return.input_base = input_digits.size();
                        conversion_return.input_digits = input_digits;
                        conversion_return.precision = precision_requested;
		}

		void set_output(ConversionReturn &conversion_return)
		{
                        if (!conversion_return.errors_encountered)
                        {
                                conversion_return.output = conversion_return.before_point;
                                if (conversion_return.is_float)
                                        conversion_return.output += conversion_return.decimal_point_used + conversion_return.after_point;
                        }
                        else
                        {
                                std::string empty_string {""};
                                conversion_return.output = convert_string_to_wstring(empty_string);
                        }
		}

		void set_output_info(ConversionReturn &conversion_return, std::wstring &output_digits)
		{
		        if (!conversion_return.errors_encountered)
                        {
                                conversion_return.output_base = output_digits.size();
                                conversion_return.output_digits = output_digits;
                        }
		}

		void convert_front(ConversionReturn &conversion_return)
		{
			if (!conversion_return.errors_encountered)
                                convert_base_front(conversion_return);
		}

		void convert_back(ConversionReturn & conversion_return)
		{
			if (conversion_return.is_float && !conversion_return.errors_encountered)
                                convert_base_back(conversion_return);
		}

		void trim_zeroes_from_integer(ConversionReturn &conversion_return)
		{
		        wchar_t zero_char = conversion_return.output_digits[0];
		        if (conversion_return.before_point.size() > 1)
                        {
                                bool only_zeroes_encountered = true;
                                while (only_zeroes_encountered)
                                {
                                        if (conversion_return.before_point[0] == zero_char)
                                                conversion_return.before_point = conversion_return.before_point.substr(1, std::wstring::npos);
                                        else
                                                only_zeroes_encountered = false;
                                }
                        }
                        if (conversion_return.before_point.size() == 0)
                                conversion_return.before_point += zero_char;
		}

                void convert_base_front(ConversionReturn &conversion_return)
                {
                        boost::multiprecision::cpp_int num = convert_number_wstring_to_integer(input_front, input_digits);
                        conversion_return.before_point = convert_integer_to_wstring(num, conversion_return.output_digits);
                        trim_zeroes_from_integer(conversion_return);
                }

		void convert_base_back(ConversionReturn &conversion_return)
		{
                        //Calculate and correct the result.
                        conversion_return.max_precision = get_max_precision(conversion_return);
                        if (conversion_return.max_precision < conversion_return.precision)
                                conversion_return.precision = conversion_return.max_precision;
                        std::wstring raw_output = calculate_raw_output(conversion_return.precision, conversion_return.output_digits);
                        conversion_return.after_point = round_output(raw_output, conversion_return.output_digits);
                        remove_trailing_zeroes(conversion_return);

                        //Determine whether the output is exact.
                        bool conversion_return_exact = false;
                        if (test_for_exactness(conversion_return.output, conversion_return.output_digits))
                                conversion_return_exact = true;

                        //Update conversion_return.
                        if (!conversion_return_exact)
                                conversion_return.rounded = true;
                        else
                                conversion_return.rounded = false;

		}

		unsigned int get_max_precision(ConversionReturn &conversion_return)
		{
		        boost::multiprecision::cpp_int size_of_base_10 = exponent(10, 1800); //1800 is the maximum precision of the cpp_dec_float_huge type in base 10.
                        boost::multiprecision::cpp_int corrected_float_max_precision = 0;
                        boost::multiprecision::cpp_int size_of_output_base = 1;
                        unsigned int base = conversion_return.output_base;
                        while (size_of_output_base * base <= size_of_base_10)
                        {
                                corrected_float_max_precision++;
                                size_of_output_base *= base;
                        }
                        unsigned int max_precision = static_cast<unsigned int>(corrected_float_max_precision);
                        if (precision_requested > corrected_float_max_precision)
                                set_warning("Float precision at base " + std::to_string(base) + " can be no greater than " + std::to_string(max_precision) + ". Precision has been reset to " + std::to_string(max_precision) + ".", conversion_return);
                        return max_precision;
		}

                std::wstring calculate_raw_output(unsigned int actual_precision, std::wstring digits_to)
                {
                        size_t digits_in_back = input_back.length();
			size_t base = input_digits.length();
			boost::multiprecision::cpp_int denominator = exponent(base, digits_in_back);
			boost::multiprecision::cpp_int numerator = convert_number_wstring_to_integer(input_back, input_digits);
			cpp_dec_float_huge numerator_f = static_cast<cpp_dec_float_huge>(numerator);
			cpp_dec_float_huge denominator_f = static_cast<cpp_dec_float_huge>(denominator);
			cpp_dec_float_huge fraction = numerator_f / denominator_f;
			std::string empty_string {""};
			std::wstring output_string = convert_string_to_wstring(empty_string);
			size_t position_output { 0 };
			size_t position_digits;
			bool last_digit_added = false;
			base = digits_to.size();
                        while (position_output <= actual_precision && !last_digit_added) //<= there is one extra digit on the end, to be removed later.
			{
				if (fraction == 0)
					last_digit_added = true;
				fraction = fraction * base;
				cpp_dec_float_huge integer;
				fraction = boost::multiprecision::modf(fraction, &integer);
				position_digits = static_cast<size_t>(integer);
				output_string += digits_to[(position_digits)];
				position_output++;
			}
			return output_string;
                }

                std::wstring round_output(std::wstring raw_output, std::wstring digits_to)
                {
                        bool rounding_needed = true;
                        bool rounded = false;
                        unsigned int base = digits_to.length();
                        boost::multiprecision::cpp_int midpoint_base = base;
                        if (base % 2 == 1)
                                midpoint_base++;
                        boost::multiprecision::cpp_int midpoint = (midpoint_base / 2);
                        unsigned int position_digits = digits_to.find_first_of(raw_output.back());
                        if (position_digits < midpoint) //If final digit is below mid point (midpoint variable is the next digit available at or below the true mid point), no need to do further rounding, just remove the final digit.
                        {
                                rounding_needed = false;
                                raw_output = raw_output.substr(0, (raw_output.size() - 1));
                                rounded = true;
                        }
                        else if (raw_output.size() > 1)//If final digit at or above mid point, need to round up.
                        {
                                unsigned int position_output = (raw_output.size() - 2);
                                while (rounding_needed && position_output >= 0) //Keep rounding up as long as the final digit is the last in digits_to (ie is the highest possible digit, the equivalent of 9 in decimal). Do not remove the last digit remaining after the decimal point.
                                {
                                        if ((raw_output[position_output + 1] != digits_to.back() || raw_output[position_output] != digits_to.back()) || position_output == 0)
                                        {
                                                position_digits = digits_to.find_first_of(raw_output[position_output]);
                                                position_digits++;
                                                raw_output[(position_output)] = digits_to[position_digits];
                                                rounding_needed = false;
                                                rounded = true;
                                                raw_output = raw_output.substr(0, (raw_output.size() - 1));
                                        }
                                        else
                                                raw_output = raw_output.substr(0, (raw_output.size() - 1)); //If the final digit in raw_output is the highest digit, remove it.

                                        position_output--;
                                }

                        }
                        return raw_output;
                }

                void remove_trailing_zeroes(ConversionReturn &conversion_return)
                {
                        unsigned int position_output = conversion_return.after_point.size();
                        bool finished {false};
                        if (position_output > 1)
                        {
                                while (position_output > 1 && !finished)
                                {
                                        --position_output;
                                        unsigned int character_position = conversion_return.output_digits.find_first_of(conversion_return.after_point.at(position_output));
                                        if (character_position != 0)
                                                finished = true;
                                        else
                                                conversion_return.after_point = conversion_return.after_point.substr(0, position_output);
                                }
                        }
		}

		bool test_for_exactness(std::wstring proposed_output, std::wstring output_digits)
		{
		        bool exact {false};
		        unsigned int input_base = input_digits.size();
		        unsigned int output_base = output_digits.size();
		        boost::multiprecision::cpp_int common_base = input_base * output_base;
		        boost::multiprecision::cpp_int revised_output_numerator = convert_number_wstring_to_integer(proposed_output, output_digits) * input_base;
		        if (revised_output_numerator % output_base == 0)
                                exact = true;
                        return exact;
		}

                void convert_output_digits_to_uppercase(ConversionReturn &conversion_return)
		{
			std::transform(conversion_return.before_point.begin(), conversion_return.before_point.end(), conversion_return.before_point.begin(), ::toupper);
			std::transform(conversion_return.after_point.begin(), conversion_return.after_point.end(), conversion_return.after_point.begin(), ::toupper);
		}

		void set_error_or_warning(std::string &message, std::vector<std::string> &message_vector, bool &encountered)
		{
		        if (!encountered)
                                message_vector.clear();
                        encountered = true;
                        message_vector.push_back(message);
                        if (use_exceptions)
				throw (message);
		}

		void set_warning(std::string warning_message)
		{
                        set_error_or_warning(warning_message, input_warnings, input_warnings_encountered);
		}

		void set_error(std::string error_message)
		{
                        set_error_or_warning(error_message, input_errors, input_errors_encountered);
		}

		void set_warning(std::string warning_message, ConversionReturn &conversion_return)
		{
                        set_error_or_warning(warning_message, conversion_return.warnings, conversion_return.warnings_encountered);
		}

		void set_error(std::string error_message, ConversionReturn &conversion_return)
		{
                        set_error_or_warning(error_message, conversion_return.errors, conversion_return.errors_encountered);
		}

		void set_errors(std::vector<std::string> string_vector)
		{
		        for(std::string s : string_vector)
                                set_error(s);
		}
	};
}


#endif // ANYBASECONVERSION_H_INCLUDED
