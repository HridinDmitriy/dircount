#include "CommandLineOption.h"

// static variable initialization
const vector<string> CommandLineOption::prefixes{ "--", "-" };


CommandLineOption::CommandLineOption(string _long_name)
	: CommandLineOption(_long_name, "")
{ }

CommandLineOption::CommandLineOption(string _long_name, string _short_name)
	: CommandLineOption(_long_name, _short_name, "")
{ }

CommandLineOption::CommandLineOption(string _long_name, string _short_name, string _description)
	: CommandLineOption(_long_name, _short_name, _description, false)
{ }

CommandLineOption::CommandLineOption(string _long_name, string _short_name, string _description, bool _is_required)
	: CommandLineOption(_long_name, _short_name, _description, _is_required, "")
{ }

CommandLineOption::CommandLineOption(string _long_name, string _short_name, string _description, bool _is_required,
	string _argument_name, string _argument_value) 
{
	this->long_name_ = _long_name;
	this->short_name_ = (_long_name != _short_name)? _short_name : "";
	this->description_ = _description;
	this->is_required_ = _is_required;
	this->argument_name_ = _argument_name; 
	this->argument_value_ = _argument_value;
	this->has_argument_ = (this->argument_name_ != "") ? true : false;
	this->has_argument_value_ = (this->argument_value_ != "") ? true : false;
}

string CommandLineOption::get_short_name() const noexcept
{
	return short_name_;
}

void CommandLineOption::set_short_name(const string & _short_name) noexcept
{
	this->short_name_ = _short_name;
}

string CommandLineOption::get_long_name() const noexcept
{
	return long_name_;
}

void CommandLineOption::set_long_name(const string & _long_name) noexcept
{
	this->long_name_ = _long_name;
}

string CommandLineOption::get_description() const noexcept
{
	return description_;
}

void CommandLineOption::set_description(const string & _description) noexcept
{
	this->description_ = description_;
}

string CommandLineOption::get_argument_name() const noexcept
{
	return argument_name_;
}

void CommandLineOption::set_argument_name(const string & _argument_name) noexcept
{
	this->argument_name_ = _argument_name;
	this->has_argument_ = (this->argument_name_ != "") ? true : false;
}

string CommandLineOption::get_argument_value() const noexcept
{
	return argument_value_;
}

void CommandLineOption::set_argument_value(const string & _argument_value) noexcept
{
	this->argument_value_ = _argument_value;
	this->has_argument_value_ = (this->argument_value_ != "") ? true : false;
}

bool CommandLineOption::has_short_name() const noexcept
{
	return (short_name_ != "") && (short_name_ != long_name_);
}

bool CommandLineOption::has_description() const noexcept
{
	return (description_ != "");
}

bool CommandLineOption::has_argument() const noexcept
{
	return has_argument_;
}

bool CommandLineOption::has_argument_value() const noexcept
{
	return has_argument_value_;
}

bool CommandLineOption::is_optional() const noexcept
{
	return !is_required_;
}

string CommandLineOption::get_name_without_prefix(const string& _option_name)
{
	if (_option_name != "")
	{
		for (auto& prefix : Opt::prefixes)
		{
			size_t prefix_length = prefix.length();
			if (_option_name.substr(0, prefix_length) == prefix)
			{
				return _option_name.substr(prefix_length);
			}
		}
	}
	return _option_name;
}

string CommandLineOption::get_short_prefixed_name(const string & _option_name)
{
	return _get_prefixed_name(_option_name, Opt::prefixes[1]);
}

string CommandLineOption::get_long_prefixed_name(const string & _option_long_name)
{
	return _get_prefixed_name(_option_long_name, Opt::prefixes[0]);
}

string CommandLineOption::_get_prefixed_name(const string & _option_name, const string & _prefix)
{
	if (_option_name == "")
		return _option_name;

	return _prefix + Opt::get_name_without_prefix(_option_name);
}

bool CommandLineOption::has_short_prefix(const string & _option_name)
{
	return (_option_name.length() - Opt::get_name_without_prefix(_option_name).length() == 1);
}

bool CommandLineOption::has_long_prefix(const string & _option_name)
{
	return (_option_name.length() - Opt::get_name_without_prefix(_option_name).length() == 2);
}


