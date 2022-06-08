
#include "cli.h"

_BeginNamespace(eokas::cli)

void Option::set(const String& name, const String& info, const StringValue& value)
{
	this->name = name;
	this->info = info;
	this->value = value;
}

String Option::toString() const
{
	return String::format("\t%s\t\t\t\t%s (default:%s)\n", name.cstr(), info.cstr(), (const char*)value);
}

void Command::set(const String& name, const String& info, const Func& func)
{
	this->name = name;
	this->info = info;
	this->func = func;
}

Option Command::option(const String& name, const String& info, const StringValue& defaultValue)
{
	Option opt;
	opt.set(name, info, defaultValue);
	this->options.insert(std::make_pair(name, opt));
	return opt;
}

Command Command::command(const String& name, const String& info, const Func& func)
{
	Command cmd;
	cmd.set(name, info, func);
	this->subCommands.insert(std::make_pair(name, cmd));
	return cmd;
}

String Command::toString() const
{
	String str = String::format("%s\t\t\t\t%s\n", name.cstr(), info.cstr());
	for(auto& opt : this->options)
	{
		str += opt.second.toString();
	}
	for(auto& cmd : this->subCommands)
	{
		str += cmd.second.toString();
	}
	return str;
}

int Command::exec(int argc, char** argv)
{
	if(argc <= 0 || this->name != StringValue(argv[0]))
		return -1;
	
	std::vector<StringValue> args;
	for (int i = 0; i < argc; i++)
	{
		String arg = argv[i];
		args.emplace_back(arg);
	}
	
	if(args[0] != this->name)
		return -2;
	
	// process sub-commands.
	if(args.size() > 1)
	{
		String cmdName = args[1];
		auto cmdIter = this->subCommands.find(cmdName);
		if(cmdIter != this->subCommands.end())
		{
			auto& cmd = cmdIter->second;
			return cmd.exec(argc - 1, argv + 1);
		}
	}
	
	// process options.
	if(args.size() > 1)
	{
		for(auto& opt : this->options)
		{
			// compatible with "-v,--version"
			auto fragments = opt.second.name.split(",");
			for(auto& frag : fragments)
			{
				auto argIter = std::find(args.begin(), args.end(), frag);
				if(argIter == args.end())
					continue;
				
				++argIter;
				
				opt.second.value = argIter == args.end() || argIter->string().startsWith("-")
					? "true"
					: *argIter;
			}
		}
	}
	return this->func(*this);
}

_EndNamespace(eokas::cli)
