#include "PluginLoader.h"
#include "Plugin.h"
#include "Context.h"
#include "Data.h"

#include <iostream>
#include <cstdlib>
#include <list>
#include <set>
#include <algorithm>

#include <errno.h>
#include <sstream>

bool parseLine(const std::string& line, Data& data)
{
	std::stringstream ss(line);
	ss >> data.command;

	while (!ss.eof())
	{
		std::string parameter;
		ss >> parameter;
		data.params.push_back(parameter);
	}
	return true;
}

bool handleCoreCommands(Context& ctx, const Data& data)
{
	if (data.command == "exit")
	{
		ctx.exit = true;
	}
	else if (data.command == "env")
	{
		for (auto it: ctx.environment)
		{
			std::cout << it.first << " = " << it.second << std::endl;
		}
	}
	else if (data.command == "set")
	{
		std::string param;
		std::string val;
		auto it = data.params.cbegin();

		if (it != data.params.cend())
		{
			param = *it;
			++it;
		}
		if (it != data.params.cend())
		{
			val = *it;
		}
		ctx.environment[param] = val;
	}
	else if (data.command == "echo")
	{
		auto it = data.params.cbegin();
		while (it != data.params.cend())
		{
			if (it != data.params.cbegin())
			{
				std::cout << " ";
			}
			std::cout << *it;
			++it;
		}
		std::cout << std::endl;
	}
	else if (data.command == "help")
	{
		std::cout << "Core commands" << "\n"
			<< "\texit              \texits the application\n"
			<< "\thelp              \tprints this help\n"
			<< "\tset <name> <value>\tsets an environment variable\n"
			<< "\tenv               \tprints environment variables\n"
			<< "\techo              \tprints arguments\n";

		for (auto* plugin: ctx.plugins)
		{
			plugin->help();
		}
	}
	else
	{
		return false;
	}
	return true;
}

int main(int argc, char** argv)
{
	Context ctx;
	ctx.environment["prompt"] = "cmd > ";

	PluginLoader::loadAllPlugins(ctx);

	while (!ctx.exit)
	{
		Data data;
		std::list<std::string> cmd;
		std::string tag;
		std::cout << ctx.environment["prompt"];
		if (!std::cin.eof())
		{
			std::getline(std::cin, tag);
		}
		else
		{
			break;
		}

		if (!parseLine(tag, data))
		{
			std::cerr << "Failed to parse input" << std::endl;
			continue;
		}

		if (!handleCoreCommands(ctx, data))
		{
			auto it = ctx.handledCommandsByPlugin.find(data.command);
			if (it != ctx.handledCommandsByPlugin.end())
			{
				it->second->handle(ctx, data);
			}
			else
			{
				std::cout << "Unknown command " << data.command << std::endl;
			}
		}
	}

	PluginLoader::unloadAllPlugins(ctx);

	return EXIT_SUCCESS;
}
