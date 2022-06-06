#include <Plugin.h>
#include <iostream>

#include <Context.h>
#include <Data.h>
#include <cassert>

class Bar : public Plugin
{
public:
	Bar() : Plugin("bar")
	{
		addHandledCommand("bar");
		addHandledCommand("bai");
	}
	virtual ~Bar()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}

	bool init(Context& context)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return true;
	}

	bool fini(Context& context)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return true;
	}

	bool handle(Context& context, const Data& data)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return true;
	}
};

extern "C"
{
	Bar plugin_object;
}
