#ifndef __DATA_H__
#define __DATA_H__

#include <string>
#include <list>

struct Data
{
	std::string command;
	std::list<std::string> params;
};

#endif // __DATA_H__
