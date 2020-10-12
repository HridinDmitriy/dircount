#pragma once
#include "CommandLineOption.h"
#include <ostream>

using std::ostream;

class HelpPrinter
{
public:
	using Brackets = std::pair<std::string, std::string>; 
	using Opt = CommandLineOption;

	static Brackets option_brackets;
	static Brackets argument_brackets;

	HelpPrinter(string _executable_name, const vector<Opt>& _options, ostream& _os);
	HelpPrinter(string _executable_name, const vector<Opt>& _options, string _header, ostream& _os);
	HelpPrinter(string _executable_name, const vector<Opt>& _options, string _header, string _footer, ostream& _os);

	void print() const;

private:
	bool _has_header() const;
	bool _has_footer() const;
	bool _has_options() const;

	void _print_line() const;
	void _print_header() const;
	void _print_usage() const;
	void _print_option(const Opt& _option) const;
	void _print_option_argument(const string& _arg_name) const;
	void _print_description() const;
	void _print_too_long_description(const string& _desc, size_t _field_width) const;
	void _print_footer() const;
	void _set_offset_at_non_space_symbol(const string & _str, size_t& _offset) const;

private:
	const vector<Opt>& options_;
	string executable_name_;
	string header_;
	string footer_;
	ostream& os_;

	const static string _empty_string_;
	const static size_t _description_width_;
	const static size_t _long_name_width_;
	const static size_t _short_name_width_;
};

