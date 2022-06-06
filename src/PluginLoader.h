#ifndef __PLUGIN_HANDLE__
#define __PLUGIN_HANDLE__

#include <string>
#include <list>

class Plugin;
struct Context;

class PluginLoader
{
	struct PluginHandle
	{
		std::string name;
		void* handle {nullptr};
		Plugin* object {nullptr};
	};
public:
	static bool loadAllPlugins(Context& ctx);
	static bool unloadAllPlugins(Context& ctx);

	static Plugin* load(const std::string& name);
	static bool    unload(Plugin* plugin);
	static bool    unload(const std::string& name);
	static const std::string& pluginPath();
protected:
	static bool    unload(PluginHandle& ph);
private:
	static std::string s_pluginPath;
	static std::list<PluginHandle> s_plugins;
};

#endif // __PLUGIN_HANDLE__
