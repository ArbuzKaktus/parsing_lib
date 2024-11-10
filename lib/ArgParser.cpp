#include "ArgParser.h"
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>


ArgumentParser::StringArg::StringArg(char short_arg_name, std::string arg_name, std::string description)
    : short_arg_name_(short_arg_name)
    , arg_name_(arg_name)
    , description_(description)
    , is_positional_(false)
    , is_used_(false)
    , min_count_values_(0)
    , default_value_("")
    , value_("")
    , store_value_(nullptr)
    , store_values_(nullptr) 
    {}

ArgumentParser::StringArg &ArgumentParser::StringArg::Positional() {
  is_positional_ = true;
  return *this;
}

ArgumentParser::StringArg& ArgumentParser::StringArg::StoreValues(std::vector<std::string>& store) {
  store_values_ = &store;
  return *this;
}

ArgumentParser::StringArg& ArgumentParser::StringArg::StoreValue(std::string& store) {
  store_value_ = &store;
  return *this;
}

ArgumentParser::StringArg& ArgumentParser::StringArg::MultiValue(size_t min_count_values) {
  min_count_values_ = min_count_values;
  return *this;
}

ArgumentParser::StringArg& ArgumentParser::StringArg::Default(std::string value) {
  default_value_ = value;
  value_ = value;
  is_used_ = true;
  return *this;
}

std::vector<std::string>* ArgumentParser::StringArg::GetStoreValues() { return store_values_; }

size_t ArgumentParser::StringArg::GetMultivalue() {
    return min_count_values_; 
}

std::string ArgumentParser::StringArg::GetValue() {
    return value_; 
}

std::string ArgumentParser::StringArg::GetFullName() {
    return arg_name_; 
}

char ArgumentParser::StringArg::GetShortName() {
    return short_arg_name_; 
}

bool ArgumentParser::StringArg::IsPositional() {
    return is_positional_; 
}

ArgumentParser::IntArg::IntArg(char short_arg_name, std::string arg_name, std::string description)
    : short_arg_name_(short_arg_name)
    , arg_name_(arg_name)
    , description_(description)
    , is_positional_(false)
    , is_used_(false)
    , min_count_values_(0)
    , default_value_(-1)
    , value_(-1)
    , store_value_(nullptr)
    , store_values_(nullptr) 
    {}

ArgumentParser::IntArg& ArgumentParser::IntArg::Positional() {
  is_positional_ = true;
  return *this;
}

ArgumentParser::IntArg& ArgumentParser::IntArg::StoreValues(std::vector<int>& store) {
  store_values_ = &store;
  return *this;
}

ArgumentParser::IntArg& ArgumentParser::IntArg::StoreValue(int& store) {
  store_value_ = &store;
  return *this;
}

ArgumentParser::IntArg& ArgumentParser::IntArg::Default(int value) {
  default_value_ = value;
  value_ = value;
  is_used_ = true;
  return *this;
}

ArgumentParser::IntArg& ArgumentParser::IntArg::MultiValue(size_t min_count_values) {
  min_count_values_ = min_count_values;
  return *this;
}

std::vector<int>* ArgumentParser::IntArg::GetStoreValues() {
    return store_values_;
}

size_t ArgumentParser::IntArg::GetMultivalue() {
    return min_count_values_;
}

int ArgumentParser::IntArg::GetValue() {
    return value_;
}

std::string ArgumentParser::IntArg::GetFullName() {
    return arg_name_;
}

char ArgumentParser::IntArg::GetShortName() {
    return short_arg_name_;
}

bool ArgumentParser::IntArg::IsPositional() {
    return is_positional_;
}


ArgumentParser::FlagArg::FlagArg(char short_arg_name, std::string arg_name, std::string description)
    : short_arg_name_(short_arg_name)
    , arg_name_(arg_name)
    , description_(description)
    , default_value_(false)
    , store_value_(nullptr)
    , value_(false) 
    {}

ArgumentParser::FlagArg& ArgumentParser::FlagArg::StoreValue(bool& store) {
  store_value_ = &store;
  return *this;
}

ArgumentParser::FlagArg &ArgumentParser::FlagArg::Default(bool value) {
  default_value_ = value;
  value_ = value;
  return *this;
}

bool ArgumentParser::FlagArg::GetValue() {
    return value_;
}

std::string ArgumentParser::FlagArg::GetFullName() {
    return arg_name_;
}

char ArgumentParser::FlagArg::GetShortName() {
    return short_arg_name_;
}


ArgumentParser::HelpArg::HelpArg(char short_arg_name, std::string arg_name, std::string description)
    : short_arg_name_(short_arg_name)
    , arg_name_(arg_name)
    , description_(description)
    , default_value_(false)
    , store_value_(nullptr)
    , value_(false) 
    {}

ArgumentParser::HelpArg& ArgumentParser::HelpArg::StoreValue(bool& store) {
  store_value_ = &store;
  return *this;
}

ArgumentParser::HelpArg& ArgumentParser::HelpArg::Default(bool value) {
  default_value_ = value;
  value_ = value;
  return *this;
}

bool ArgumentParser::HelpArg::GetValue() {
    return value_;
}

std::string ArgumentParser::HelpArg::GetFullName() {
    return arg_name_;
}

char ArgumentParser::HelpArg::GetShortName() {
    return short_arg_name_;
}

std::string ArgumentParser::HelpArg::GetDescription(){
    return description_;
}

ArgumentParser::ArgParser::ArgParser(std::string parser_name)
    : parser_name_(parser_name)
    , help_arg_(nullptr) 
    {}

ArgumentParser::ArgParser::~ArgParser() {

  for (IntArg *elem : int_args_) {
    delete elem;
  }
  for (StringArg *elem : string_args_) {
    delete elem;
  }
  for (FlagArg *elem : flag_args_) {
    delete elem;
  }

  if (help_arg_) {
    delete help_arg_;
  }
}

ArgumentParser::ArgParser::ArgParser(const ArgParser& other)
    : parser_name_(other.parser_name_) {

  for (IntArg* elem : other.int_args_) {
    IntArg* arg = new IntArg(*elem);
    int_args_.push_back(arg);
  }
  for (StringArg* elem : other.string_args_) {
    StringArg* arg = new StringArg(*elem);
    string_args_.push_back(arg);
  }
  for (FlagArg* elem : other.flag_args_) {
    FlagArg* arg = new FlagArg(*elem);
    flag_args_.push_back(arg);
  }
  if (other.help_arg_) {
    HelpArg* arg = new HelpArg(*other.help_arg_);
    help_arg_ = arg;
  }
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::operator=(const ArgParser& other) {
  if (this != &other) {
    for (int i = int_args_.size() - 1; i != -1; --i) {
      IntArg* temp = int_args_[i];
      int_args_.pop_back();
      delete temp;
    }
    for (int i = string_args_.size() - 1; i != -1; --i) {
      StringArg* temp = string_args_[i];
      string_args_.pop_back();
      delete temp;
    }
    for (int i = flag_args_.size() - 1; i != -1; --i) {
      FlagArg* temp = flag_args_[i];
      flag_args_.pop_back();
      delete temp;
    }
    if (help_arg_) {
      delete help_arg_;
      if (other.help_arg_) {
        HelpArg* arg = new HelpArg(*other.help_arg_);
        help_arg_ = arg;
      } else {
        help_arg_ = nullptr;
      }
    }

    for (IntArg* elem : other.int_args_) {
      IntArg* arg = new IntArg(*elem);
      int_args_.push_back(arg);
    }
    for (StringArg* elem : other.string_args_) {
      StringArg* arg = new StringArg(*elem);
      string_args_.push_back(arg);
    }
    for (FlagArg* elem : other.flag_args_) {
      FlagArg* arg = new FlagArg(*elem);
      flag_args_.push_back(arg);
    }
  }
  return *this;
}

ArgumentParser::IntArg& ArgumentParser::ArgParser::AddIntArgument(
    char short_arg_name, std::string arg_name, std::string description) {

  IntArg* arg = new IntArg(short_arg_name, arg_name, description);
  int_args_.push_back(arg);
  return *arg;
}

ArgumentParser::IntArg& ArgumentParser::ArgParser::AddIntArgument(std::string arg_name,
                                          std::string description) {

  IntArg *arg = new IntArg(arg_name[0], arg_name, description);
  int_args_.push_back(arg);
  return *arg;
}

ArgumentParser::StringArg& ArgumentParser::ArgParser::AddStringArgument(
    char short_arg_name, std::string arg_name, std::string description) {

  StringArg *string_arg = new StringArg(short_arg_name, arg_name, description);
  string_args_.push_back(string_arg);
  return *string_arg;
}

ArgumentParser::StringArg&
ArgumentParser::ArgParser::AddStringArgument(std::string arg_name, std::string description) {
  StringArg *string_arg = new StringArg(arg_name[0], arg_name, description);
  string_args_.push_back(string_arg);
  return *string_arg;
}

ArgumentParser::FlagArg&
ArgumentParser::ArgParser::AddFlag(char short_arg_name, std::string arg_name, std::string description) {
  FlagArg *flag_arg = new FlagArg(short_arg_name, arg_name, description);
  flag_args_.push_back(flag_arg);
  return *flag_arg;
}

ArgumentParser::FlagArg&
ArgumentParser::ArgParser::AddFlag(std::string arg_name, std::string description) {

  FlagArg *flag_arg = new FlagArg(arg_name[0], arg_name, description);
  flag_args_.push_back(flag_arg);
  return *flag_arg;
}

ArgumentParser::HelpArg&
ArgumentParser::ArgParser::AddHelp(char short_arg_name, std::string arg_name, std::string description) {
  HelpArg* help_arg = new HelpArg(short_arg_name, arg_name, description);
  help_arg_ = help_arg;
  return *help_arg;
}

ArgumentParser::HelpArg&
ArgumentParser::ArgParser::AddHelp(std::string arg_name, std::string description) {
  HelpArg* help_arg = new HelpArg(arg_name[0], arg_name, description);
  help_arg_ = help_arg;
  return *help_arg;
}

bool ArgumentParser::ArgParser::Help() {
    return help_arg_->GetValue();
}

int ArgumentParser::ArgParser::GetIntValue(std::string arg_name) {
  size_t i = 0;
  while (i != int_args_.size() && int_args_[i]->GetFullName() != arg_name) {
    ++i;
  }

  if (i == int_args_.size()) {
    std::cerr << "No such argument in the list." << std::endl;
    exit(EXIT_FAILURE);
  }

  return int_args_[i]->GetValue();
}

int ArgumentParser::ArgParser::GetIntValue(std::string arg_name, size_t index) {
  size_t i = 0;
  while (i != int_args_.size() && int_args_[i]->GetFullName() != arg_name) {
    ++i;
  }

  if (i == int_args_.size()) {
    std::cerr << "No such argument in the list." << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<int>* arr = int_args_[i]->GetStoreValues();
  if (arr[0].size() <= index) {
    std::cerr << "Segmentation fault." << std::endl;
    exit(EXIT_FAILURE);
  }

  return arr[0][index];
}

std::string ArgumentParser::ArgParser::GetStringValue(std::string arg_name) {
  size_t i = 0;
  while (i != string_args_.size() &&
         string_args_[i]->GetFullName() != arg_name) {
    ++i;
  }

  if (i == string_args_.size()) {
    std::cerr << "No such argument in the list." << std::endl;
    exit(EXIT_FAILURE);
  }

  return string_args_[i]->GetValue();
}

std::string ArgumentParser::ArgParser::GetStringValue(std::string arg_name, size_t index) {
  size_t i = 0;
  while (i != string_args_.size() &&
         string_args_[i]->GetFullName() != arg_name) {
    ++i;
  }

  if (i == string_args_.size()) {
    std::cerr << "No such argument in the list." << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<std::string> *arr = string_args_[i]->GetStoreValues();
  if (arr[0].size() <= index) {
    std::cerr << "Segmentation fault." << std::endl;
    exit(EXIT_FAILURE);
  }

  return arr[0][index];
}

bool ArgumentParser::ArgParser::GetFlag(std::string arg_name) {
  size_t i = 0;
  while (i != flag_args_.size() && flag_args_[i]->GetFullName() != arg_name) {
    ++i;
  }

  if (i == flag_args_.size()) {
    std::cerr << "No such argument in the list." << std::endl;
    exit(EXIT_FAILURE);
  }

  return flag_args_[i]->GetValue();
}

std::string ArgumentParser::StringArg::MakeHelpForArg() {
  std::string help = "";
  help += '-';
  help += short_arg_name_;
  help += "\t--" + arg_name_ + "=<string>\t" + description_ + '\t';
  is_positional_ ? help += "[positional]" : help;
  default_value_ != "" ? help += "[default = " + default_value_ + ']' : help;
  min_count_values_ != 0 ? help += "[repeated, min arg = " 
                                    + std::to_string(min_count_values_) 
                                    + ']' : help;
  help += '\n';

  return help;
}

std::string ArgumentParser::IntArg::MakeHelpForArg() {
  std::string help = "";
  help += '-';
  help += short_arg_name_;
  help += "\t--" + arg_name_ + "=<string>\t" + description_ + '\t';
  is_positional_ ? help += "[positional]" : help;
  default_value_ != -1 ? help += "[default = " 
                                + std::to_string(default_value_) + ']' : help;
  min_count_values_ != 0 ? help += "[repeated, min arg = " 
                                + std::to_string(min_count_values_) + ']' : help;
  help += '\n';

  return help;
}

std::string ArgumentParser::FlagArg::MakeHelpForArg() {
  std::string help = "";
  help += '-';
  help += short_arg_name_;
  help += "\t--" + arg_name_ + "\t" + description_ + '\t';
  default_value_ ? help += "[default = true]" : help += "[default = false]";
  help += '\n';

  return help;
}

std::string ArgumentParser::HelpArg::MakeHelpForArg() {
  std::string help = "";
  help += '-';
  help += short_arg_name_;
  help += "\t--" + arg_name_ + "\t";
  size_t i = 0;
  while (description_[i] != '\0') {
    help += description_[i];
    ++i;
  }
  help += '\t';
  default_value_ ? help += "[default = true]" : help += "[default = false]";
  help += '\n';

  return help;
}

std::string ArgumentParser::ArgParser::HelpDescription() {
  std::string output = "";
  output += parser_name_ + "\n";
  output += help_arg_->GetDescription() + "\n\n";

  for (StringArg *elem : string_args_) {
    output += elem->MakeHelpForArg();
  }

  for (IntArg *elem : int_args_) {
    output += elem->MakeHelpForArg();
  }

  for (FlagArg *elem : flag_args_) {
    output += elem->MakeHelpForArg();
  }

  output += help_arg_->MakeHelpForArg();

  return output;
}

std::string GetArgWithoutEq(std::string arg) {
  std::string out = "";

  for (char elem : arg) {
    if (elem == '=') {
      break;
    }
    out += elem;
  }

  return out;
}

ArgumentParser::IntArg**
ArgumentParser::ArgParser::FindIntArg(std::string argument) {
  std::string short_arg;
  std::string arg = GetArgWithoutEq(argument);

  for (IntArg *elem : int_args_) {
    short_arg = elem->GetShortName();

    if ("--" + elem->GetFullName() == arg || '-' + short_arg == arg) {
      IntArg **elem_ptr = &elem;
      return elem_ptr;
    }
  }

  return nullptr;
}

ArgumentParser::StringArg**
ArgumentParser::ArgParser::FindStringArg(std::string argument) {
  std::string short_arg;
  std::string arg = GetArgWithoutEq(argument);
  for (StringArg *elem : string_args_) {
    short_arg = elem->GetShortName();

    if ("--" + elem->GetFullName() == arg || '-' + short_arg == arg) {
      StringArg **elem_ptr = &elem;
      return elem_ptr;
    }
  }

  return nullptr;
}

ArgumentParser::FlagArg**
ArgumentParser::ArgParser::FindFlagArg(std::string arg) {
  for (FlagArg *elem : flag_args_) {

    if ("--" + elem->GetFullName() == arg) {
      FlagArg **elem_ptr = &elem;
      return elem_ptr;
    }
  }

  return nullptr;
}

ArgumentParser::FlagArg**
ArgumentParser::ArgParser::FindShortFlagArg(char arg) {
  for (FlagArg *elem : flag_args_) {

    if (elem->GetShortName() == arg) {
      FlagArg **elem_ptr = &elem;
      return elem_ptr;
    }
  }

  return nullptr;
}

ArgumentParser::HelpArg*
ArgumentParser::ArgParser::FindHelpArg(std::string arg) {
  std::string short_arg;
  short_arg = help_arg_->GetShortName();
  if ("--" + help_arg_->GetFullName() == arg || '-' + short_arg == arg) {
    return help_arg_;
  }
  return nullptr;
}

bool IsArg(std::string str) {
    return str[0] == '-';
}

bool IsShortArg(std::string str) {
    return IsArg(str) && isalpha(str[1]);
}

bool Isnum(std::string str) {
  for (char elem : str) {

    if (isalpha(elem)) {
      return false;
    }
  }

  return true;
}

int GetIntValueFromArg(std::vector<std::string> &argv, size_t &index) {
  std::string num = "";
  std::string str = argv[index];

  for (size_t i = 0; i < str.size(); ++i) {

    if (str[i] == '=') {

      if (i == str.size()) {
        std::cerr << "too few arguments" << std::endl;
        exit(EXIT_FAILURE);
      }
      ++i;

      while (i < str.size()) {
        num += str[i];
        ++i;
      }

    }
  }
  if (num == "") {

    if (index + 1 >= argv.size()) {
      std::cerr << "too few arguments" << std::endl;
      exit(EXIT_FAILURE);
    }

    ++index;
    num = argv[index];
  }
  if (!Isnum(num)) {
    std::cerr << "argument value must have int data type" << std::endl;
    exit(EXIT_FAILURE);
  }

  return std::stoi(num);
}

std::string GetStringValueFromArg(std::vector<std::string>& argv, size_t& index) {
  std::string value = "";
  std::string str = argv[index];
  for (size_t i = 0; i < str.size(); ++i) {

    if (str[i] == '=') {

      if (i >= str.size()) {
        std::cerr << "too few arguments" << std::endl;
        exit(EXIT_FAILURE);
      }

      if (str[i + 1]) ++i;
      while (i < str.size()) {
        value += str[i];
        ++i;
      }
    }
  }

  if (value == "") {

    if (index + 1 >= argv.size()) {
      std::cerr << "too few arguments" << std::endl;
      exit(EXIT_FAILURE);
    }

    ++index;
    value = argv[index];
    if (value[0] == '-') {
      std::cerr << "too few arguments" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  return value;
}

void ArgumentParser::IntArg::SetArgValue(std::vector<std::string>& argv, size_t& ind) {
  int value = GetIntValueFromArg(argv, ind);
  value_ = value;
  if (min_count_values_ != 0) {

    if (store_values_) {
      store_values_->push_back(value);
    }

  }
  if (store_value_) {
    *store_value_ = value;
  }
}

void ArgumentParser::IntArg::SetPosArgValue(std::vector<std::string>& argv, size_t& ind) {
  int value;
  if (min_count_values_ != 0) {

    if (!store_values_) {
      std::cerr << "Store for Values not set" << std::endl;
      exit(EXIT_FAILURE);
    }
    
    while (ind != argv.size() && Isnum(argv[ind])) {
      value = std::stoi(argv[ind]);
      store_values_->push_back(value);
      value_ = value;

      if (store_value_) {
        *store_value_ = value;
      }

      ++ind;
    }
    if (store_values_->size() < min_count_values_) {
      std::cerr << "Too few Arguments" << std::endl;
      exit(EXIT_FAILURE);
    }
    return;
  }
  value_ = value;
  if (store_value_) {
    *store_value_ = value;
  }
}

void ArgumentParser::StringArg::SetArgValue(std::vector<std::string>& argv, size_t& ind) {
  std::string value = GetStringValueFromArg(argv, ind);
  value_ = value;
  if (min_count_values_ != 0) {

    if (store_values_) {
      store_values_->push_back(value);
    }

  }
  if (store_value_) {
    *store_value_ = value;
  }
}

void ArgumentParser::StringArg::SetPosArgValue(std::vector<std::string>& argv, size_t& ind) {
  std::string value = argv[ind];
  ++ind;
  if (min_count_values_ != 0) {

    if (!store_values_) {
      std::cerr << "Store for Values not set" << std::endl;
      exit(EXIT_FAILURE);
    }

    while (ind != argv.size() && !Isnum(argv[ind]) && argv[ind][0] != '-') {
      store_values_->push_back(value);
      value_ = value;

      if (store_value_) {
        *store_value_ = value;
      }

      ++ind;
    }

    if (store_values_->size() < min_count_values_) {
      std::cerr << "Too few Arguments" << std::endl;
      exit(EXIT_FAILURE);
    }

    return;
  }
  value_ = value;
  if (store_value_) {
    *store_value_ = value;
  }
}

void ArgumentParser::FlagArg::SetArgValue() {
  value_ = true;
  if (store_value_) {
    *store_value_ = value_;
  }
}
void ArgumentParser::HelpArg::SetArgValue() {
  value_ = true;
  if (store_value_) {
    *store_value_ = value_;
  }
}

ArgumentParser::IntArg **
ArgumentParser::ArgParser::FindNextIntArgPosition(size_t &num_prev) {
  size_t count = 0;
  for (IntArg *elem : int_args_) {

    if (elem->IsPositional()) {
      ++count;
    }

    if (count == num_prev) {
      ++num_prev;
      IntArg **int_arg = &elem;
      return int_arg;
    }

  }
  return nullptr;
}

ArgumentParser::StringArg**
ArgumentParser::ArgParser::FindNextStringArgPosition(size_t& num_prev) {
  size_t count = 0;
  for (StringArg* elem : string_args_) {

    if (elem->IsPositional()) {
      ++count;
    }

    if (count == num_prev) {
      ++num_prev;
      StringArg** int_arg = &elem;
      return int_arg;
    }

  }
  return nullptr;
}

bool ArgumentParser::ArgParser::Parse(std::vector<std::string> argv) {
  bool fl_int = int_args_.empty();
  bool fl_str = string_args_.empty();
  bool fl_fl = flag_args_.empty();
  bool fl_hlp = help_arg_ == nullptr;
  IntArg** int_arg = nullptr;
  StringArg** str_arg = nullptr;
  FlagArg** fl_arg = nullptr;
  HelpArg* hlp_arg = nullptr;
  size_t num_prev = 1;
  size_t args_count = argv.size();
  size_t i = 1;

  while (i < args_count) {

    if (IsArg(argv[i])) {

      if (!fl_int) {
        int_arg = FindIntArg(argv[i]);

        if (int_arg) {
          (*int_arg)->SetArgValue(argv, i);
          ++i;
          continue;
        }

      }

      if (!fl_str) {
        str_arg = FindStringArg(argv[i]);

        if (str_arg) {
          (*str_arg)->SetArgValue(argv, i);
          ++i;
          continue;
        }

      }
      if (!fl_hlp) {
        hlp_arg = FindHelpArg(argv[i]);

        if (hlp_arg) {
          (hlp_arg)->SetArgValue();
          ++i;
          continue;
        }

      }

      if (!fl_fl) {

        if (IsShortArg(argv[i])) {

          for (size_t j = 1; j != argv[i].size(); ++j) {
            FlagArg **fl_arg = FindShortFlagArg(argv[i][j]);

            if (fl_arg) {
              (*fl_arg)->SetArgValue();
            } else {
              std::cerr << "Uknown Argument" << std::endl;
              exit(EXIT_FAILURE);
            }

          }
          ++i;

        } else {
          FlagArg **fl_arg = FindFlagArg(argv[i]);

          if (fl_arg) {
            (*fl_arg)->SetArgValue();
            ++i;
          } else {
            std::cerr << "Uknown Argument" << std::endl;
            exit(EXIT_FAILURE);
          }

        }
      }
    } else {

      if (Isnum(argv[i])) {
        IntArg **int_arg = FindNextIntArgPosition(num_prev);

        if (int_arg) {
          (*int_arg)->SetPosArgValue(argv, i);
        }

      } else {
        StringArg **str_arg = FindNextStringArgPosition(num_prev);

        if (str_arg) {
          (*str_arg)->SetPosArgValue(argv, i);
        }

      }
    }
  }
  if (help_arg_) {

    if (help_arg_->GetValue()) {
      std::cout << HelpDescription() << std::endl;
      return true;
    }

  }
  for (IntArg *elem : int_args_) {

    if (elem->GetValue() == -1) {
      std::cerr << "Not default argument is not set" << std::endl;
      return false;
    }

    if (!elem->GetStoreValues()) continue;

    if (elem->GetStoreValues()->size() < elem->GetMultivalue()) {
      std::cerr << "too few arguments" << std::endl;
      return false;
    }

  }
  for (StringArg *elem : string_args_) {

    if (elem->GetValue() == "") {
      std::cerr << "Not default argument is not set" << std::endl;
      return false;
    }

    if (!elem->GetStoreValues()) continue;

    if (elem->GetStoreValues()->size() < elem->GetMultivalue()) {
      std::cerr << "too few arguments" << std::endl;
      return false;
    }

  }
  
  return true;
}

bool ArgumentParser::ArgParser::Parse(int argc, char **args) {
  std::vector<std::string> argv;
  std::string arg;
  for (size_t i = 0; i != argc; ++i) {
    arg = args[i];
    argv.push_back(arg);
  }
  return Parse(argv);
}