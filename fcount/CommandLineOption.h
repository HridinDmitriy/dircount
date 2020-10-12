#pragma once
#include <string>
#include <vector>

using std::string; 
using std::vector;

class CommandLineOption
{
public: 
	using Opt = CommandLineOption;

	CommandLineOption(string _long_name); 
	CommandLineOption(string _long_name, string _short_name);
	CommandLineOption(string _long_name, string _short_name, string _description);
	CommandLineOption(string _long_name, string _short_name, string _description, bool _is_required);
	CommandLineOption(string _long_name, string _short_name, string _description, bool _is_required,
		string _argument_name, string _argument_value = "");

	string get_short_name() const noexcept;
	void set_short_name(const string& _name) noexcept;

	string get_long_name() const noexcept;
	void set_long_name(const string& _longname) noexcept;

	string get_description() const noexcept;
	void set_description(const string& _description) noexcept;

	string get_argument_name() const noexcept;
	void set_argument_name(const string& _argument_name) noexcept;

	string get_argument_value() const noexcept;
	void set_argument_value(const string& _argument_value) noexcept;

	bool has_short_name() const noexcept;
	bool has_description() const noexcept;
	bool is_optional() const noexcept;

	bool has_argument() const noexcept;
	bool has_argument_value() const noexcept;

	static string get_name_without_prefix(const string& _option_name);
	static string get_short_prefixed_name(const string& _option_name); 
	static string get_long_prefixed_name(const string& _option_long_name);

	static bool has_short_prefix(const string& _option_name);
	static bool has_long_prefix(const string& _option_name);

public:
	static const vector<string> prefixes;

private:
	static string _get_prefixed_name(const string& _option_name, const string& _prefix);

private:
	string short_name_;
	string long_name_;
	string description_;
	string argument_name_; 
	string argument_value_;
	bool has_argument_;
	bool has_argument_value_;
	bool is_required_; 
};


