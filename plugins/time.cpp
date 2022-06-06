#include <Plugin.h>
#include <iostream>

#include <Context.h>
#include <Data.h>

#include <time.h>
#include <string.h>
#include <chrono>

class Time : public Plugin
{
public:
	Time() : Plugin("time")
	{
		addHandledCommand("time");
		addHandledCommand("date");
	}
	virtual ~Time()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}

	bool init(Context& context)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		tzset();
		context.environment["TZ"] = (daylight == 0 ? tzname[0] : tzname[1]);
		return true;
	}

	bool fini(Context& context)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		context.environment.erase("TZ");
		return true;
	}

	bool handle(Context& context, const Data& data)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;

		struct tm* tmp {nullptr};
		time_t now = time(nullptr);


		constexpr int replace = 1;
		if (!data.params.empty())
		{
			::setenv("TZ", data.params.front().c_str(), replace);
		}
		else
		{
			if (!context.environment["TZ"].empty())
			{
				::setenv("TZ", context.environment["TZ"].c_str(), replace);
			}
		}
		tzset();
		tmp = localtime(&now);

		if (tmp == nullptr)
		{
			std::cerr << "Failed to get time" << std::endl;
			return false;
		}

		if (data.command == "time")
		{
			char stringRepresentation[128] {};
			strftime(stringRepresentation, sizeof(stringRepresentation), "%H:%M:%S %Z", tmp);
			std::cout << stringRepresentation << std::endl;
		}
		else if (data.command == "date")
		{
			char stringRepresentation[128] {};
			strftime(stringRepresentation, sizeof(stringRepresentation), "%d/%m/%Y", tmp);
			std::cout << stringRepresentation << std::endl;
		}
		::unsetenv("TZ");
		return true;
	}
};

extern "C"
{
	Time plugin_object;
}
