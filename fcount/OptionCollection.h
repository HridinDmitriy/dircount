#pragma once
#include <map>
#include "CommandLineOption.h"

using std::map;

class OptionCollection
{
public:
	using Opt = CommandLineOption;

	Opt& operator[](size_t _index);
	const Opt& operator[](size_t _index) const;

	void add_option(const Opt& _option);
	void add_option(const vector<Opt>& _options);

	size_t index_by(const string& _option_name) const;
	bool has_option(const string& _option_name) const;

	size_t end() const;
	void clear();
	size_t size() const;

private:
	void _add_indices(const Opt& _option, size_t _index);

private:
	map<string, size_t> indices_by_short_name_{};
	map<string, size_t> indices_by_long_name_{};
	vector<Opt> options_{};

	static const size_t const_max_value;
};

