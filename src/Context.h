#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <string>
#include <map>
#include <list>
struct Context
{
	bool exit {false};
	std::map<std::string, std::string> environment;
	std::list<Plugin*> plugins;
	std::map<std::string, Plugin*> handledCommandsByPlugin;
};

#endif // __CONTEXT_H__
