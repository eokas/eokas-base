
#ifndef  _EOKAS_BASE_CLI_H_
#define  _EOKAS_BASE_CLI_H_

#include "header.h"
#include "string.h"

_BeginNamespace(eokas::cli)

struct Option
{
	String name = "";
	String info = "";
	StringValue value = StringValue::empty;

	String toString() const;
};

struct Command
{
	using Func = std::function<void(const Command& cmd)>;

	String name = "";
	String info = "";
	Func func = nullptr;
	std::map <String, Option> options = {};
	std::map <String, Command> subCommands = {};

	Option option(const String& name, const String& info, const StringValue& defaultValue);
	Command command(cons String& name, const String& info, const Func& func);
	String toString() const;
	void exec(int argc, char** argv);
	void exec(const std::vector<StringValue>& args);
};

_EndNamespace(eokas::cli)

#endif//_EOKAS_BASE_CLI_H_
