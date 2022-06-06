#include "PluginLoader.h"

#include <dlfcn.h>
#include "Plugin.h"

#include <sstream>
#include <iostream>
#include <cassert>

#include <sys/types.h>
#include <dirent.h>

#include "Context.h"

static constexpr size_t PluginPrefixLength {3};
static constexpr size_t PluginSuffixLength {3};

std::list<PluginLoader::PluginHandle> PluginLoader::s_plugins;
std::string PluginLoader::s_pluginPath {INSTALL_PREFIX "/lib"};

bool PluginLoader::loadAllPlugins(Context& ctx)
{
	DIR* pluginDir = opendir(PluginLoader::pluginPath().c_str());
	int err = errno;
	if (err)
	{
		std::cerr << "Failed to open plugin directory. Aborting..." << std::endl;
		return false;
	}

	while (struct dirent* entry = readdir(pluginDir))
	{
		std::string pluginName = entry->d_name;
		if (!pluginName.starts_with("lib") || !pluginName.ends_with(".so"))
		{
			continue;
		}
		pluginName = pluginName.substr(PluginPrefixLength, pluginName.size() - PluginPrefixLength - PluginSuffixLength);
		std::cout << "Got file " << entry->d_name << " with plugin name " << pluginName << std::endl;

		Plugin* plugin = PluginLoader::load(pluginName);
		if (!plugin)
		{
			std::cerr << "Failed to load plugin " << pluginName << std::endl;
			continue;
		}
		if (!plugin->init(ctx))
		{
			std::cerr << "Failed to init plugin " << plugin->name() << std::endl;
			PluginLoader::unload(plugin);
			continue;
		}
		ctx.plugins.push_back(plugin);

		for (auto& cmd: plugin->handledCommands())
		{
			ctx.handledCommandsByPlugin[cmd] = plugin;
		}
	}
	closedir(pluginDir);
	return true;
}

bool PluginLoader::unloadAllPlugins(Context& ctx)
{
	while (Plugin* plugin = ctx.plugins.front())
	{
		std::string pluginName = plugin->name();
		plugin->fini(ctx);
		if (!PluginLoader::unload(plugin))
		{
			std::cerr << "Failed to unload plugin " << pluginName << std::endl;
		}
		else
		{
			std::cout << "Unloaded plugin " << pluginName << std::endl;
		}
		ctx.plugins.pop_front();
	}
	return true;
}


Plugin* PluginLoader::load(const std::string& name)
{
	PluginHandle ph;
	ph.name = name;

	std::stringstream ss;
	ss << s_pluginPath << "/" << "lib" << name << ".so";

	ph.handle = dlopen(ss.str().c_str(), RTLD_LAZY);
	if (!ph.handle)
	{
		std::cerr << "Failed to load plugin " << name << " with error " << dlerror();
		return nullptr;
	}
	dlerror();
	void* plugin_object = dlsym(ph.handle, "plugin_object");
	if (!plugin_object)
	{
		std::cerr << "Failed to identify the symbol 'plugin_object'. Error was " << dlerror() << std::endl;
		dlclose(ph.handle);
		return nullptr;
	}

	ph.object = reinterpret_cast<Plugin*>(plugin_object);
	s_plugins.push_back(ph);
	return ph.object;
}

bool PluginLoader::unload(const std::string& name)
{
	for (auto& it: s_plugins)
	{
		if (it.object->name() == name)
		{
			return unload(it);
		}
	}
	return false;
}

bool PluginLoader::unload(Plugin* plugin)
{
	for (auto& it: s_plugins)
	{
		if (it.object == plugin)
		{
			return unload(it);
		}
	}
	return false;
}

bool PluginLoader::unload(PluginLoader::PluginHandle& ph)
{
	void* handle = ph.handle;
	dlclose(handle);
	return true;
}

const std::string & PluginLoader::pluginPath()
{
	return s_pluginPath;
}
