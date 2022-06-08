
#include "args.h"

_BeginNamespace(eokas::cli)

String Option::toString() const
{
	return String::format("\t%s\t\t\t\t%s (default:%s)\n", name.cstr(), info.cstr(), value.cstr());
}

Option Command::option(const String& name, const String& info, const StringValue& defaultValue)
{
	Option opt;
	opt.name = name;
	opt.info = info;
	opt.value = defaultValue;

	this->options.insert(std::make_pair(name, opt));

	return opt;
}

Command Command::command(const String& name, const String& info, const int& func)
{
	Command cmd;
	cmd.name = name;
	cmd.info = info;
	cmd.func = func;

	this->subCommands.insert(std::make_pair(name, cmd));

	return cmd;
}

String Command::toString() const
{
	String str = String::format("%s\t\t\t\t%s\n", name.cstr(), info.cstr());
	for(auto& opt : this->options)
	{
		str += opt.toString();
	}
	for(auto& cmd : this->subCommands)
	{
		str += cmd.toString();
	}
	return str;
}

void Command::exec(int argc, char** argv)
{
	std::vector<StringValue> args;
	for (int i = 0; i < argc; i++)
	{
		String arg = argv[i];
		args.push_back(arg);
	}
	this->exec(args);
}

void Command::exec(const std::vector<StringValue>& args)
{
	if(args.size() <= 0)
		return;
	if(args[0] != this->name)
		return;

	// process sub-commands.
	if(args.size() > 1)
	{
		String cmdName = argv[1];
		auto cmdIter = this->subCommands.find(cmd);
		if(cmditer != this->subCommands.end())
		{
			auto& cmd = cmdIter->second;
			cmd.exec(argc - 1, argv + 1);
			return;
		}
	}

	// process options.
	if(args.size() > 1)
	{
		for(auto& opt : this->options)
		{
			auto fragments = opt.name.split(",");
			for(auto frag : fragments)
			{
				auto argIter = std::find(args.begin(), args.end(), frag);
				if(argIter == args.end())
					continue;
				++argIter;
				if(argIter == args.end())
					continue;
				opt.value = *argIter;
			}
		}
	}
	this->func(this);
}

_EndNamespace(eokas::cli)
