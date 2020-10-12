#pragma once
#include "OptionCollection.h"

using std::pair;

class OptionParser
{
public:
	using Opt = CommandLineOption;

	OptionParser() = default;
	OptionParser(const char* _actual_arguments[], size_t _actual_arguments_count, const vector<Opt>& _options);
	OptionParser(const vector<string>& _actual_arguments, const vector<Opt>& _options);

	void add_option(const Opt& _option); 
	void add_option(const vector<Opt>& _options);

	void add_actual_argument(const string& _arg); 
	void add_actual_argument(const vector<string>& _args);

	OptionCollection parse() const; 

public:
	const char const_arg_qualifier = '@';

private:
	bool _is_option_name_valid(const string& _option_name) const;
	bool _is_option_argument_valid(const string& _option_name) const;
	void _parse_option_group(const string& _option_group, OptionCollection& _parsed_options) const;
	void _ñheck_existence(const string& _option_name) const;
	void _ñheck_if_already_parsed(const string& _option_name, const OptionCollection& _parsed_options) const;
	void _check_for_an_argument_value(const Opt& _option, const string& _actual_option_name) const;
	void _check_for_missing_option(const Opt& _option, const OptionCollection& _parsed_options) const;
	void _put_an_argument_if_exist(Opt& _option, size_t& _arg_index) const;
	string _remove_arg_qualifier(const string& _arg) const;

private:
	vector<string> actual_arguments_{};
	OptionCollection all_options_{}; 
};

