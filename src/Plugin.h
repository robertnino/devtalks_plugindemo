#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include <string>
#include <set>

struct Context;
struct Data;

class Plugin
{
public:
	Plugin(const std::string& name);
	virtual ~Plugin() noexcept;

	const std::string& name() const;
	const std::set<std::string>& handledCommands() const;

	virtual void help() const;

	virtual bool init(Context& context) = 0;
	virtual bool fini(Context& context) = 0;

	virtual bool handle(Context& context, const Data& data) = 0;

protected:
	bool addHandledCommand(const std::string& cmd);

private:
	const std::string m_name;
	std::set<std::string> m_handledCommands;
};

#endif // __PLUGIN_H__
