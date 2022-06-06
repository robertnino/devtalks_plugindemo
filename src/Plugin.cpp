#include "Plugin.h"
#include <iostream>

Plugin::Plugin(const std::string& name) :
	m_name(name)
{

}

Plugin::~Plugin() noexcept
{
}


const std::string& Plugin::name() const
{
	return m_name;
}

void Plugin::help() const
{
	std::cout << "Plugin " << name() << "\n";
	for (auto it: m_handledCommands)
	{
		std::cout << "\t" << it << "\t" << "Descriptive text\n";
	}
	std::cout << std::endl;
}

const std::set<std::string>& Plugin::handledCommands() const
{
	return m_handledCommands;
}

bool Plugin::addHandledCommand(const std::string& cmd)
{
	if (m_handledCommands.find(cmd) == m_handledCommands.end())
	{
		m_handledCommands.insert(cmd);
		return true;
	}
	return false;
}
