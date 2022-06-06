#include <Plugin.h>
#include <iostream>
#include <unistd.h>

class WhoAmI : public Plugin
{
public:
	WhoAmI() : Plugin("whoami")
	{
		addHandledCommand("whoami");
	}
	virtual ~WhoAmI()
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
		uid_t uid = ::getuid();
		std::cout << "uid = " << uid << " username " << getlogin() << std::endl;
		return true;
	}
};

extern "C"
{
	WhoAmI plugin_object;
}
