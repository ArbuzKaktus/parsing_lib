#pragma once
#include "ArgParser.h"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace ArgumentParser {

class StringArg{
    public:
    StringArg(char short_arg_name = ' ', std::string arg_name = "", std::string description = "");
    StringArg& Positional();
    StringArg& MultiValue(size_t min_count_values = 1);
    StringArg& StoreValue(std::string& store);
    StringArg& StoreValues(std::vector<std::string>& store);
    StringArg& Default(std::string value);
    std::string MakeHelpForArg();
    void SetArgValue(std::vector<std::string>& argv, size_t& ind);
    void SetPosArgValue(std::vector<std::string>& argv, size_t& ind);
    std::string GetValue();
    std::string GetFullName();
    char GetShortName();
    std::vector<std::string>* GetStoreValues();
    size_t GetMultivalue();
    bool IsPositional();

    private:
    size_t min_count_values_;
    std::string arg_name_;
    char short_arg_name_;
    std::string description_;
    std::string default_value_;
    std::string value_;
    std::string* store_value_;
    std::vector<std::string>* store_values_;
    bool is_positional_;
    bool is_used_;
};

class IntArg{
    public:

    IntArg(char short_arg_name = ' ', std::string arg_name = "", std::string description = "");
    IntArg& Positional();
    IntArg& MultiValue(size_t min_count_values = 1);
    IntArg& StoreValue(int& store);
    IntArg& StoreValues(std::vector<int>& store);
    IntArg& Default(int value);
    size_t GetMultivalue();
    void SetArgValue(std::vector<std::string>& argv, size_t& ind);
    void SetPosArgValue(std::vector<std::string>& argv, size_t& ind);
    std::string MakeHelpForArg();
    int GetValue();
    std::string GetFullName();
    char GetShortName();
    bool IsPositional();
    std::vector<int>* GetStoreValues();

    private:

    int* store_value_;
    std::vector<int>* store_values_;
    std::string arg_name_;
    char short_arg_name_;
    std::string description_;
    size_t min_count_values_;
    int default_value_;
    int value_;
    bool is_positional_;
    bool is_used_;
};

class FlagArg{
    public:

    FlagArg(char short_arg_name = ' ', std::string arg_name = "", std::string description = "");
    FlagArg& StoreValue(bool& store);
    FlagArg& Default(bool value);
    std::string MakeHelpForArg();
    void SetArgValue();
    bool GetValue();
    std::string GetFullName();
    char GetShortName();

    private:

    bool* store_value_;
    std::string arg_name_;
    char short_arg_name_;
    std::string description_;
    bool default_value_;
    bool value_;
};

class HelpArg{
    public:

    HelpArg(const char short_arg_name = ' ', const std::string arg_name = "", const std::string description = "");
    HelpArg& StoreValue(bool& store);
    HelpArg& Default(bool value);
    std::string MakeHelpForArg();
    void SetArgValue();
    bool GetValue();
    std::string GetFullName();
    std::string GetDescription();
    char GetShortName();

    private:

    bool* store_value_;
    std::string arg_name_;
    char short_arg_name_;
    std::string description_;
    bool default_value_;
    bool value_;
};

class ArgParser {
    public: 
    ArgParser(const std::string parser_name = "my_parser");
    ~ArgParser();
    ArgParser(const ArgParser& other);
    ArgParser& operator=(const ArgParser& other);
    IntArg& AddIntArgument(char short_arg_name, std::string arg_name = "", std::string description = "");
    IntArg& AddIntArgument(std::string arg_name, std::string description = "");
    StringArg& AddStringArgument(char short_arg_name, std::string arg_name = "", std::string description = "");
    StringArg& AddStringArgument(std::string arg_name, std::string description = "");
    FlagArg& AddFlag(char short_arg_name, std::string arg_name = "", std::string description = "");
    FlagArg& AddFlag(std::string arg_name, std::string description = "");
    HelpArg& AddHelp(char short_arg_name, std::string arg_name, std::string description = "");
    HelpArg& AddHelp(std::string arg_name, std::string description = "");
    bool Parse(std::vector<std::string>);
    bool Parse(int argc, char** argv);
    bool Help();
    std::string HelpDescription();
    int GetIntValue(std::string arg_name);
    int GetIntValue(std::string arg_name, size_t index);
    std::string GetStringValue(std::string arg_name);
    std::string GetStringValue(std::string arg_name, size_t index);
    bool GetFlag(std::string arg_name);
    size_t GetCountOfNotSetArgs();
    IntArg** FindNextIntArgPosition(size_t& prev_num);
    IntArg** FindIntArg(std::string arg);
    StringArg** FindStringArg(std::string arg);
    StringArg** FindNextStringArgPosition(size_t& prev_num);
    FlagArg** FindShortFlagArg(char arg);
    FlagArg** FindFlagArg(std::string arg);
    HelpArg* FindHelpArg(std::string arg);
    
    private:
    HelpArg* help_arg_;
    std::string parser_name_;
    std::vector<StringArg*> string_args_;
    std::vector<IntArg*> int_args_;
    std::vector<FlagArg*> flag_args_;

};

} // namespace ArgumentParser