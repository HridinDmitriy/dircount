#include "OptionParser.h"

OptionParser::OptionParser(const char * _actual_arguments[], size_t _actual_arguments_count,
	const vector<Opt>& _all_options)
{
	size_t& size = _actual_arguments_count;

	for (size_t i = 0; i < size; ++i)
	{
		add_actual_argument(
			_actual_arguments[i]
		);
	}
	add_option(_all_options);
}

OptionParser::OptionParser(const vector<string>& _actual_arguments, const vector<Opt>& _all_options)
{
	this->actual_arguments_ = _actual_arguments;
	add_option(_all_options); 
}

void OptionParser::add_option(const Opt & _option)
{
	return all_options_.add_option(_option);
}

void OptionParser::add_option(const vector<Opt>& _options)
{
	for (auto& opt : _options) 
	{
		add_option(opt); 
	}
}

void OptionParser::add_actual_argument(const string & _arg)
{
	actual_arguments_.push_back(_arg); 
}

void OptionParser::add_actual_argument(const vector<string> & _args)
{
	for (auto& arg : _args)
	{
		add_actual_argument(arg);
	}
}

OptionCollection OptionParser::parse() const
{
	OptionCollection parsed_options; 

	auto actual_arguments_size = actual_arguments_.size();
	for (size_t i = 0; i < actual_arguments_size; ++i)
	{
		auto arg = actual_arguments_[i];

		if (_is_option_name_valid(arg)) 
		{
			auto option_name = Opt::get_name_without_prefix(arg);

			if (Opt::has_short_prefix(arg) && arg.length() > 2)
			{
				_parse_option_group(option_name, parsed_options);
			}
			else
			{ 
				_ñheck_existence(option_name);
				_ñheck_if_already_parsed(option_name, parsed_options);
				
				auto index = all_options_.index_by(option_name);
				auto option = all_options_[index];

				if (option.has_argument())
				{
					_put_an_argument_if_exist(option, i);
					_check_for_an_argument_value(option, option_name);
				}
				parsed_options.add_option(option);
			}
		}
		else
		{
			throw std::runtime_error("The token '" + arg + "' which can be an option, has invalid format");
		}
	}

	auto all_options_size = all_options_.size();
	for (size_t i = 0; i < all_options_size; ++i)
	{
		_check_for_missing_option(all_options_[i], parsed_options);
	}

	return parsed_options;
}

void OptionParser::_parse_option_group(const string & _option_group, OptionCollection& _parsed_options) const
{
	auto size = _option_group.length();
	for (size_t i = 0; i < size; ++i)
	{
		auto option_name = string(1, _option_group[i]);

		_ñheck_existence(option_name);
		_ñheck_if_already_parsed(option_name, _parsed_options);

		auto index = all_options_.index_by(option_name);
		auto option = all_options_[index];

		if (option.has_argument())
		{
			_check_for_an_argument_value(option, option_name);
		}
		_parsed_options.add_option(option);
	}
}

void OptionParser::_ñheck_existence(const string & _option_name) const
{
	if (!all_options_.has_option(_option_name))
	{
		throw std::runtime_error("The option '" + _option_name + "' is undefined.");
	}
}

void OptionParser::_ñheck_if_already_parsed(const string & _option_name, const OptionCollection & _parsed_options) const
{
	if (_parsed_options.has_option(_option_name))
	{
		throw std::runtime_error("The option '" + _option_name + "' has already been parsed.");
	}
}

void OptionParser::_check_for_an_argument_value(const Opt & _option, const string& _actual_option_name) const
{
	if (!_option.has_argument_value())
	{
		throw std::runtime_error("Missing argument for the option '" + _actual_option_name + "'.");
	}
}

void OptionParser::_check_for_missing_option(const Opt & _option, const OptionCollection & _parsed_options) const
{
	auto option_name = _option.get_long_name(); 
	if (!_option.is_optional() && !_parsed_options.has_option(option_name))
	{
		throw std::runtime_error("Required option '" + option_name + "' not found.");
	}
}

bool OptionParser::_is_option_name_valid(const string & _option_name) const
{
	return (Opt::has_short_prefix(_option_name) && _option_name.length() > 1)
		|| (Opt::has_long_prefix(_option_name) && _option_name.length() > 3);
}

bool OptionParser::_is_option_argument_valid(const string & _option_name) const
{
	return !_is_option_name_valid(_option_name);
}

string OptionParser::_remove_arg_qualifier(const string & _arg) const
{
	if (_arg[0] == const_arg_qualifier)
	{
		return _arg.substr(1);
	}
	return _arg;
}

void OptionParser::_put_an_argument_if_exist(Opt & _option, size_t & _arg_index) const
{
	if ((_arg_index + 1) >= actual_arguments_.size()) return;

	auto option_argument = actual_arguments_[_arg_index + 1];

	if (_is_option_argument_valid(option_argument))
	{
		_option.set_argument_value(_remove_arg_qualifier(option_argument));
		++_arg_index;
	}
}
