#include <Plugin.h>
#include <iostream>

class Foo : public Plugin
{
public:
	Foo() : Plugin("foo")
	{
		addHandledCommand("foo");
		addHandledCommand("fui");
	}
	virtual ~Foo()
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
	Foo plugin_object;
}
