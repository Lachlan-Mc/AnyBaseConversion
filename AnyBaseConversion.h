#ifndef ANYBASECONVERSION_H_INCLUDED
#define ANYBASECONVERSION_H_INCLUDED

#include <string>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <codecvt>
#include <locale>
#include <cstdint>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

enum Base
{
	binary,
	octal,
	decimal,
	hexadecimal,
};

//template <unsigned Digits10, class ExponentType = std::int32_t, class Allocator = void> class cpp_dec_float;
typedef boost::multiprecision::number<boost::multiprecision::cpp_dec_float<1000> > cpp_dec_float_200;

class AnyBaseConversion
{
public:
	AnyBaseConversion(std::string &number_s, Base base, bool throw_exception = false)
	{
		input(number_s, base, throw_exception);
	}

	AnyBaseConversion(std::string &number_s, std::string &digits, bool throw_exception = false)
	{
		input(number_s, digits, throw_exception);
	}

	AnyBaseConversion()
	{
		//No action.
	}

	void input(std::string &number_s, Base base, bool throw_exception = false)
	{
		std::wstring number_w = convert_string_to_wstring(number_s);
		std::wstring digits = convert_Base_to_char_set(base);
		defined_input_digits = true;
		if (base == hexadecimal)
		{
			std::wstring number_in_lower_case = number_w;
			std::transform(number_in_lower_case.begin(), number_in_lower_case.end(), number_in_lower_case.begin(), ::tolower);
			load_number(number_in_lower_case, digits);
		}
		else
			load_number(number_w, digits);
		if (throw_exception)
			throw_exception = true;
	}

	void input(std::string &number_s, std::string &digits, bool throw_exception = false)
	{
		std::wstring number_w = convert_string_to_wstring(number_s);
		std::wstring digits_w = convert_string_to_wstring(digits);
		load_number(number_w, digits_w);
		if (throw_exception)
			throw_exception = true;
	}

	std::string output_with_new_base(Base base, bool use_uppercase_hexadecimal = false)
	{
		defined_output_digits = true;
		if (base == hexadecimal && use_uppercase_hexadecimal)
			convert_hexadecimal_output_digits_to_uppercase();
		convert_number_to_new_base(convert_Base_to_char_set(base));
		if (base == hexadecimal)
			output_is_hexadecimal = true;
		return output(use_uppercase_hexadecimal);
	}

	std::string output_with_new_base(std::string digits)
	{
	std::wstring digits_w = convert_string_to_wstring(digits);
	convert_number_to_new_base(digits_w);
	return output();
	}

	void set_float_precision(int precision)
	{
		if (precision < 1 || precision > float_maximum_precision)
			set_error("Error: Illegal float precision. Try again with a float precision (the number of digits after the decimal point) between 1 and " + std::to_string(float_maximum_precision) + ".");
		else
			float_precision = precision;
	}


private:
	std::wstring										input_front;
	std::wstring										input_back;
	std::wstring										input_digits;
	std::wstring										output_front;
	std::wstring 										output_back;
	std::wstring										output_digits;
	std::string											decimal_points { ".," };
	std::string											error { "" };
	bool														throw_exception { false };
	bool														defined_input_digits { false };
	bool														defined_output_digits { false };
	bool														is_float { false };
	bool														output_is_hexadecimal { false };
	wchar_t													decimal_point_used;
	size_t 													float_precision { 10 }; //Return of floats defaults to 10 decimal places, or more if higher number of digits is entered. Can also be set manually.
	size_t													float_maximum_precision { 1000 };
	boost::multiprecision::cpp_int	input_base;
	boost::multiprecision::cpp_int	output_base;


	void load_number(std::wstring &number_s, std::wstring char_set)
	{
		if (defined_input_digits || validate_digits(char_set))
		{
			input_front = number_s;
			input_digits = char_set;
			validate_input();
			break_into_float();
		}
	}

	std::wstring convert_Base_to_char_set(Base base)
	{
		std::string char_set;
		if (base == binary)
			char_set = "01";
		else if (base == octal)
			char_set = "01234567";
		else if (base == decimal)
			char_set = "0123456789";
		else if (base == hexadecimal)
			char_set = "0123456789abcdef";
		return convert_string_to_wstring(char_set);
	}

	std::wstring convert_string_to_wstring(std::string &s)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.from_bytes(s);
	}

	std::string convert_wstring_to_string(std::wstring &s)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.to_bytes(s);
	}

	void convert_number_to_new_base(std::wstring digits)
	{
		if (error == "" && (defined_output_digits || validate_digits(digits)))
		{
			output_digits = digits;
			convert_front();
			if (is_float)
				convert_back();
		}
	}

	bool validate_digits(std::wstring digits)
	{
		bool repeat_digit_found { false };
		size_t i { 0 };
		while (!repeat_digit_found && i < digits.length())
		{
			char digit_character = digits[i];
			if (count_instances_of_char_in_string(digit_character, digits) > 1)
				repeat_digit_found = true;
			i++;
		}
		if (repeat_digit_found)
			set_error("Error: the inputted set of digits contains repeat characters. To be used for base conversion, every character must be unique.");
		return !repeat_digit_found;
	}

	void validate_input()
	{
		bool illegal_character_found { false };
		bool decimal_point_found { false };
		size_t num { 0 };
		std::string test = convert_wstring_to_string(input_front);
		while (!illegal_character_found && num < input_front.length())
		{
			char c = static_cast<char>(input_front[num]);
			if (count_instances_of_char_in_string(input_front[num], input_digits) != 1)
			{
				std::wstring decimal_points_w = convert_string_to_wstring(decimal_points);
				if (count_instances_of_char_in_string(input_front[num], decimal_points_w) == 1)
				{
					if (!decimal_point_found && num != 0 && num != input_front.size())
					{
						decimal_point_used = input_front[num];
						is_float = true;
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
			set_error("Error: Input string does not match formatting requirements. It may have illegal characters, or it may have more than one decimal point or a decimal point in the wrong place.");
	}

	void break_into_float()
	{
		if (is_float)
		{
			size_t decimal_point_location = input_front.find_first_of(decimal_point_used);
			input_back = input_front.substr((decimal_point_location + 1), std::wstring::npos);
			input_front = input_front.substr(0, (decimal_point_location));
			if (input_back.length() > float_precision)
			{
				if (input_back.length() <= float_maximum_precision)
					float_precision = input_back.length();
				else
					float_precision = float_maximum_precision;
			}
		}
	}

	size_t count_instances_of_char_in_string(wchar_t character, std::wstring &str)
	{
		size_t num { 0 };
		for (wchar_t c : str)
			if (c == character)
				num++;
		return num;
	}

	void convert_front()
	{
		if (error == "")
		{
			output_front = convert_base(input_front, input_digits, output_digits);
		}
	}

	void convert_back()
	{
		if (is_float && error == "")
		{
			output_back = convert_base_back(input_back, input_digits, output_digits);
		}
	}

	std::wstring reverse_string(std::wstring &str)
	{
		std::wstring string_out {};
		for (size_t i = str.length(); i > 0; --i)
		{
			string_out += str[i-1];
		}
		return string_out;
	}

	std::wstring convert_base(std::wstring &num_string, std::wstring &digits_from, std::wstring &digits_to)
	{
		boost::multiprecision::cpp_int num = convert_number_wstring_to_integer(num_string, digits_from);
		return convert_integer_to_wstring(num, digits_to);
	}

	std::wstring convert_base_back(std::wstring &num_string, std::wstring &digits_from, std::wstring &digits_to)
	{
		size_t digits_in_back = num_string.length();
		size_t base = digits_from.length();

		boost::multiprecision::cpp_int denominator = pow(base, digits_in_back);
		boost::multiprecision::cpp_int numerator = convert_number_wstring_to_integer(num_string, digits_from);
		cpp_dec_float_200 numerator_f = static_cast<cpp_dec_float_200>(numerator);
		cpp_dec_float_200 denominator_f = static_cast<cpp_dec_float_200>(denominator);
		cpp_dec_float_200 fraction = numerator_f / denominator_f;
		std::string empty_string { "" };
		std::wstring output_string = convert_string_to_wstring(empty_string);
		size_t position_output { 0 };
		size_t position_digits;
		base = digits_to.length();
		bool last_digit_added = false;
		while (position_output <= float_precision && !last_digit_added) //<= so that there is at least one extra digit on the end.
		{
			if (fraction == 0)
				last_digit_added = true;
			fraction = fraction * base;
			cpp_dec_float_200 integer;
			fraction = boost::multiprecision::modf(fraction, &integer);
			position_digits = static_cast<size_t>(integer);
			output_string += digits_to[(position_digits)];
			position_output++;
		}
		bool rounded;
		position_digits = digits_to.find_first_of(output_string.back());
		output_string = output_string.substr(0, (output_string.length() - 1)); //Remove final digit from output.
		size_t midpoint = (base / 2) - 1;
		if (position_digits <= midpoint) //If final digit below mid point (midpoint variable is the next position available at or below the true mid point), no need to do further rounding.
			rounded = true;
		else //If final digit at or above mid point, need to round up.
		{
			position_output = (output_string.length());
			while (!rounded && position_output > 0) //Keep rounding up as long as the final digit is the last in digits_to (ie is the highest possible digit, the equivalent of 9 in decimal).
			{
				position_output--;
				if (output_string[position_output] != digits_to.back())
				{
					position_digits = digits_to.find_first_of(output_string[position_output]);
					position_digits++;
					output_string[position_output] = digits_to[position_digits];
					rounded = true;
				}
				else
					output_string = output_string.substr(0, (output_string.length() - 2)); //If the final digit in output_string is the highest digit, remove it.
			}
			if (position_output == 0) //If the rounding went all the way to the end of the string, e.g. 0.99999999999 in decimal would at this point have been rendered as 0. - with an empty string following the decimal point.
			{
				increment_output_front(); //Increment the number in front of the decimal point by one.
				std::string empty_string { "" };
				output_back = convert_string_to_wstring(empty_string);
				output_back += digits_to.front(); //The number after the decimal point becomes zero, expressed in the base system being used.
			}
		}
		return output_string;
	}

	void increment_output_front()
	{
		boost::multiprecision::cpp_int output_integer = convert_number_wstring_to_integer(output_front, output_digits);
		output_integer++;
		output_front = convert_integer_to_wstring(output_integer, output_digits);
	}

	boost::multiprecision::cpp_int convert_number_wstring_to_integer(std::wstring &num_string, std::wstring &char_set)
	{
		size_t base = char_set.length();
		boost::multiprecision::cpp_int num { 0 };
		std::wstring use_string = reverse_string(num_string);
		size_t value { 0 };
		for (size_t i { 0 }; i < use_string.length(); i++)
		{
			value = char_set.find_first_of(use_string[i]);
			num += (value * pow(base, i));
		}
		return num;
	}

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

	boost::multiprecision::cpp_int pow(size_t base, size_t exp)
	{
			boost::multiprecision::cpp_int result { 1 };
			boost::multiprecision::cpp_int basecu { base };
			for (size_t i = 1; i <= exp; ++i)
			{
				result = result * basecu;
			}

			return result;
	}

	void convert_hexadecimal_output_digits_to_uppercase()
	{
		std::transform(output_front.begin(), output_front.end(), output_front.begin(), ::toupper);
		std::transform(output_back.begin(), output_back.end(), output_back.begin(), ::toupper);
	}

	std::string output(bool hexadecimal_use_uppercase = false)
	{
		if (error != "")
		{
			return error;
		}
		else
			if (hexadecimal_use_uppercase && output_is_hexadecimal)
				convert_hexadecimal_output_digits_to_uppercase();
			if (!is_float)
			{
				std::string output_string = convert_wstring_to_string(output_front);
				return output_string;
			}
			else
			{
				std::string output_string = convert_wstring_to_string(output_front) + static_cast<char>(decimal_point_used) + convert_wstring_to_string(output_back);
				return output_string;
			}
	}

	void set_error(std::string error_message)
	{

		error =  error_message;
		if (throw_exception)
			throw (error_message);
	}

};

#endif // ANYBASECONVERSION_H_INCLUDED
