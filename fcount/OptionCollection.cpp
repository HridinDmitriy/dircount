#include "OptionCollection.h"

const size_t OptionCollection::const_max_value = 4294967295;

OptionCollection::Opt& OptionCollection::operator[](size_t _index)
{
	return options_[_index];
}

const OptionCollection::Opt & OptionCollection::operator[](size_t _index) const
{
	return options_[_index];
}

void OptionCollection::add_option(const Opt & _option)
{
	auto index = options_.size();
	options_.push_back(_option);
	_add_indices(_option, index);
}

void OptionCollection::add_option(const vector<Opt>& _options)
{
	for (auto& opt : _options)
	{
		add_option(opt); 
	}
}

size_t OptionCollection::index_by(const string & _option_name) const
{
	if (_option_name == "") return const_max_value;

	auto it = indices_by_short_name_.find(_option_name);
	if (it == indices_by_short_name_.cend())
	{
		it = indices_by_long_name_.find(_option_name);
		if (it == indices_by_long_name_.cend())
		{
			return const_max_value;
		}
	}
	return it->second;
}

bool OptionCollection::has_option(const string & _option_name) const
{
	return index_by(_option_name) != const_max_value;
}

void OptionCollection::clear()
{
	this->indices_by_short_name_.clear();
	this->indices_by_long_name_.clear();
	this->options_.clear();
}

size_t OptionCollection::size() const
{
	return options_.size();
}

size_t OptionCollection::end() const
{
	return const_max_value;
}

void OptionCollection::_add_indices(const Opt & _option, size_t _index)
{
	indices_by_long_name_.insert(std::make_pair(_option.get_long_name(), _index));

	if (_option.has_short_name())
	{
		indices_by_short_name_.insert(std::make_pair(_option.get_short_name(), _index));
	}
}
