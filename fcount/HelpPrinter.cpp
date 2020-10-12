#include "HelpPrinter.h"

HelpPrinter::Brackets HelpPrinter::option_brackets = std::make_pair("[", "]");
HelpPrinter::Brackets HelpPrinter::argument_brackets = std::make_pair("<", ">");

const string HelpPrinter::_empty_string_ = "";
const size_t HelpPrinter::_description_width_ = 55;
const size_t HelpPrinter::_long_name_width_ = 18;
const size_t HelpPrinter::_short_name_width_ = 10;

HelpPrinter::HelpPrinter(string _executable_name, const vector<Opt>& _options, ostream& _os)
	: options_(_options),
	  os_(_os)
{ 
	this->executable_name_ = _executable_name;
	this->header_ = _empty_string_;
	this->footer_ = _empty_string_;
}

HelpPrinter::HelpPrinter(string _executable_name, const vector<Opt>& _options, string _header, ostream & _os)
	: HelpPrinter(_executable_name, _options, _os)
{
	this->header_ = _header;
}


HelpPrinter::HelpPrinter(string _executable_name, const vector<Opt>& _options, string _header, string _footer, ostream & _os)
	: HelpPrinter(_executable_name, _options, _header, _os)
{
	this->footer_ = _footer;
}

bool HelpPrinter::_has_header() const
{
	return this->header_ != _empty_string_;
}

bool HelpPrinter::_has_footer() const
{
	return this->footer_ != _empty_string_;
}

bool HelpPrinter::_has_options() const
{
	return this->options_.size() > 0;
}

void HelpPrinter::print() const
{
	_print_header();

	if (_has_options())
	{
		if (_has_header())
		{
			_print_line();
		}
		_print_usage();
		_print_line();
		_print_description(); 

		if (_has_footer())
		{
			_print_line();
		}
	}
	else if (_has_header() && _has_footer())
	{
		_print_line();
	}
	_print_footer();
}

void HelpPrinter::_print_line() const
{
	os_ << "\n\n";
}

void HelpPrinter::_print_header() const
{
	if (_has_header())
	{
		os_ << this->header_;
	}
}

void HelpPrinter::_print_footer() const
{
	if (_has_footer())
	{
		os_ << this->footer_;
	}
}

void HelpPrinter::_print_usage() const
{
	os_ << "Usage: " << this->executable_name_ << " "; 

	for (size_t i = 0; i < options_.size(); ++i)
	{
		_print_option(options_[i]);
		if (i < options_.size() - 1)
		{
			os_ << " ";
		}
	}
}

void HelpPrinter::_print_option(const Opt& _option) const 
{
	bool is_optional = _option.is_optional();

	if (is_optional)
	{
		os_ << option_brackets.first;
	}
	os_ << Opt::get_long_prefixed_name(_option.get_long_name());

	if (_option.has_argument())
	{
		os_ << " ";
		_print_option_argument(_option.get_argument_name());
	}

	if (is_optional)
	{
		os_ << option_brackets.second;
	}
}

void HelpPrinter::_print_option_argument(const string& _arg_name) const
{
	os_ << argument_brackets.first;
	os_ << _arg_name;
	os_<< argument_brackets.second;
}

void HelpPrinter::_print_description() const
{
	auto old_width = os_.width();

	os_.setf(std::ios::left);
	for (size_t i = 0; i < options_.size(); ++i)
	{
		auto opt = options_[i];

		os_.width(_long_name_width_);
		os_ << Opt::get_long_prefixed_name(opt.get_long_name());

		os_.width(_short_name_width_);
		if (opt.has_short_name())
		{
			os_ << Opt::get_short_prefixed_name(opt.get_short_name());
		}
		else
		{
			os_ << _empty_string_;
		}

		string description = opt.get_description();
		
		if (description.length() > _description_width_)
		{
			_print_too_long_description(description, _description_width_);
		}
		else
		{
			os_.width(_description_width_);
			os_ << description;
		}

		if (i != options_.size() - 1)
		{
			_print_line();
		}
	}
	os_.width(old_width);
}

void HelpPrinter::_print_too_long_description(const string & _desc, size_t _field_width) const
{
	size_t offset = 0;
	
	while (true)
	{
		if (offset != 0)
		{
			os_.width(_long_name_width_); os_ << _empty_string_;
			os_.width(_short_name_width_); os_ << _empty_string_;
		}
		os_.width(_field_width);
		_set_offset_at_non_space_symbol(_desc, offset);
		os_ <<  _desc.substr(offset, _field_width);

		offset += _field_width;

		if (offset < _desc.length()) os_ << "\n";
		else break;
	}
}

void HelpPrinter::_set_offset_at_non_space_symbol(const string & _str, size_t& _offset) const
{
	while (_str[_offset] == ' ' || _str[_offset] == '\t') 
	{
		++_offset;
		if (_offset >= _str.length()) break;
	}
}
