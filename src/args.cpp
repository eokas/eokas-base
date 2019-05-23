
#include "args.h"

_BeginNamespace(eokas)

Args::Args(int argc, char** argv)
	:mArgs()
{
	for (int i = 0; i < argc; i++)
	{
		String arg = argv[i];
		mArgs.push_back(arg);
	}
}

size_t Args::count() const
{
	return mArgs.size();
}

bool Args::has(const String& arg) const
{
	StringValue val(arg);
	auto iter = std::find(mArgs.begin(), mArgs.end(), val);
	return iter != mArgs.end();
}

const StringValue& Args::get(size_t pos, const StringValue& def) const
{
	if (pos < 0 || pos > mArgs.size())
		return def;
	return mArgs.at(pos);
}

const StringValue& Args::get(const String& arg, const StringValue& def) const
{
	StringValue val(arg);
	auto iter = std::find(mArgs.begin(), mArgs.end(), val);
	if (iter == mArgs.end())
		return def;
	if (++iter == mArgs.end())
		return def;
	return *iter;
}

_EndNamespace(eokas)
