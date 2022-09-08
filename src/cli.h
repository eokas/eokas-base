
#ifndef  _EOKAS_BASE_CLI_H_
#define  _EOKAS_BASE_CLI_H_

#include "header.h"
#include "string.h"

_BeginNamespace(eokas::cli)

struct Option
{
	String name = "";
	String info = "";
	StringValue value = StringValue::falseValue;

	String toString() const;
};

struct Command
{
	using Func = std::function<int(const Command& cmd)>;

	String name;
	String info;
	std::map <String, Option> options = {};
	Func func;
	
	std::map <String, Command> subCommands = {};
	
	Command& option(const String& name, const String& info, const StringValue& defaultValue);
	Command& action(const Func& func);
	
	Command& subCommand(const String& name, const String& info);
	
	std::optional<Option> fetchOption(const String& shortName) const;
	std::optional<Command> fetchCommand(const String& shortName) const;
	
	String toString() const;
	
	int exec(int argc, char** argv);
};

_EndNamespace(eokas::cli)

#endif//_EOKAS_BASE_CLI_H_
