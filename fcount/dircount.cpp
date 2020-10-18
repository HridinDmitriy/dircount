#include <iostream>
#include "OptionParser.h"
#include "HelpPrinter.h"
#include <filesystem>
#include <regex>
#include <Windows.h> 

using std::cout;
using std::endl;
using std::regex_match;
using std::basic_regex;
using namespace std::filesystem;
using rdir_it = recursive_directory_iterator;
using dir_it = directory_iterator;

const string _DEFAULT_DIRECTORY_ = "./";
const string _EMPTY_STRING_ = "";
const string _ERROR_PREFIX_ = "Error: ";

template<typename _Iter>
void directory_traversal(const string& _dir, size_t& _counter, bool _empty_too, bool _hidden_too, int _dispay_count, string _mask);

bool is_hidden(const string& path);
const wchar_t* get_wchar_t(const char* ch);

int main(int argc, const char* argv[]) 
{
	setlocale(LC_ALL, "rus");

	vector<CommandLineOption> options
	{
		CommandLineOption("recursive", "r", "count directories recursively"),
		CommandLineOption("help", "h", "print this help message"),
		CommandLineOption("show", "s", "display the first 'n' directories", false, "n", "10"),
		CommandLineOption("no-empty", "", "empty directories are not counted"), 
		CommandLineOption("no-hidden", "", "hidden directories are not counted"), 
		CommandLineOption("pattern", "p", "the mask that the directory name must match", false, "mask"),
		CommandLineOption("directory", "d", "the directory this program should work with", false, "path")
	};

	OptionParser parser(++argv, argc - 1, options);
	string header = "This utility is developed to count subdirectories of a given directiry.";
	string footer = string("The parameter <path> of the directory option is \"./\" by default.\n\n") +
		string("If you want to use an option parameter that starts with '-' or '--', \n") +
		string("prefix it with '@', otherwise it will be threated as an option.");

	HelpPrinter help_printer("dircount", options, header, footer, cout);
	OptionCollection parsed_options;
	size_t directories_count = 0; 

	try
	{
		parsed_options = parser.parse();
	}
	catch (const std::exception& e)
	{
		cout << endl << "Parse error: " << e.what() << endl << endl;
		help_printer.print();
		cout << endl;
		exit(1); 
	}
	
	string work_directory = _DEFAULT_DIRECTORY_;
	auto index = parsed_options.index_by("directory");

	if (index != parsed_options.end())
	{
		work_directory = parsed_options[index].get_argument_value();
	}

	try
	{
		bool empty = !parsed_options.has_option("no-empty");
		bool hidden = !parsed_options.has_option("no-hidden");

		size_t pattern_option_index = parsed_options.index_by("pattern");

		string mask = (pattern_option_index != parsed_options.end()) ?
			parsed_options[pattern_option_index].get_argument_value() : _EMPTY_STRING_;

		size_t display_option_index = parsed_options.index_by("show");

		size_t display_count = (display_option_index != parsed_options.end()) ?
			std::stoi(parsed_options[display_option_index].get_argument_value()) : 0;


		if (parsed_options.has_option("recursive"))  
		{
			directory_traversal<rdir_it>(work_directory, directories_count, empty, hidden, display_count, mask);
		}
		else 
		{
			directory_traversal<dir_it>(work_directory, directories_count, empty, hidden, display_count, mask);
		}

		cout << endl << "The directory " << absolute(work_directory) << " contains " << directories_count << " folders." << endl; 
	}
	catch (const std::exception& e)
	{
		cout << endl << _ERROR_PREFIX_ << e.what() << endl << endl;
		exit(1); 
	}

	if (parsed_options.has_option("help"))
	{
		cout << endl; 
		help_printer.print(); 
		cout << endl;
	}
}


template<typename _Iter>
void directory_traversal(const string & _dir, size_t & _counter, bool _empty_too, bool _hidden_too, int _dispay_count, string _mask)
{
	if (_dispay_count > 0)
	{
		cout << endl << "File names: " << endl << endl;
	}

	basic_regex regex(_mask); 

	for (auto file : _Iter(_dir)) 
	{ 
		bool is_directory = file.is_directory();
		bool is_dir_empty = is_empty(file.path());
		bool has_mask = (_mask != _EMPTY_STRING_) ? true : false; 
		bool matching = regex_match(file.path().filename().string(), regex);
		 
		if (is_directory && (_empty_too || !is_dir_empty)
			&& (_hidden_too || !is_hidden(file.path().string()))
			&& (!has_mask || matching))
		{
			++_counter;
			if (_dispay_count-- > 0)
			{ 
				cout << _counter << ". " << file.path() << '\n';   
			}
		}
	}
}

bool is_hidden(const string& path)
{
	LPCWSTR wch_path = get_wchar_t(path.c_str());
	DWORD attr = GetFileAttributes(wch_path);
	return ((attr & FILE_ATTRIBUTE_HIDDEN) != 0);
}

const wchar_t* get_wchar_t(const char* ch)
{
	const size_t size = strlen(ch) + 1;
	wchar_t* wch = new wchar_t[size];

	size_t out_size;
	mbstowcs_s(&out_size, wch, size, ch, size - 1);

	return wch;
}