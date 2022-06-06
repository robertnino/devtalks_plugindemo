#include <Plugin.h>
#include <iostream>

#include <Python.h>
#include <list>
#include <map>

#include <errno.h>

#include <sys/types.h>
#include <dirent.h>
#include <Data.h>

class Python : public Plugin
{
public:
	Python() : Plugin("python")
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		Py_SetPath(PYTHON_RUNTIME_PATHS);
		Py_Initialize();
	}
	virtual ~Python()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		// Destroy the Python interpreter.
		Py_Finalize();
	}

	void help() const override
	{
		std::cout << "Plugin " << name() << "\n";

		PyObject* pHelpStr = PyUnicode_FromString("help");
		for (auto* script: m_scripts)
		{
			PyObject* pResult = PyObject_CallMethodObjArgs(script, pHelpStr, nullptr);
			if (!pResult)
			{
				std::cerr << "ERROR: Calling the python plugin help failed" << std::endl;
				continue;
			}
			Py_DECREF(pResult);
		}
		Py_DECREF(pHelpStr);
		std::cout << std::endl;
	}

	bool init(Context& context) override
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		// Convert the file name to a Python string.
		const std::string pluginPath {INSTALL_PREFIX "/pyplugins"};
		DIR* pluginDir = opendir(pluginPath.c_str());
		int err = errno;
		if (err)
		{
			std::cerr << "Failed to open plugin directory. Aborting..." << std::endl;
			return false;
		}

		while (struct dirent* entry = readdir(pluginDir))
		{
			std::string pluginName = entry->d_name;
			if (!pluginName.starts_with("pp_") || !pluginName.ends_with(".py"))
			{
				continue;
			}
			pluginName = pluginName.substr(0, pluginName.size() - 3);
			std::cout << "Got file " << entry->d_name << " with plugin name " << pluginName << std::endl;
			PyObject* moduleName = PyUnicode_FromString(pluginName.c_str());
			PyObject *pModule = PyImport_Import(moduleName);
			Py_DECREF(moduleName);
			if (!pModule)
			{
				std::cerr << "ERROR: Failed to import " << pluginName << ". Not loading..." << std::endl;
				continue;
			}
			PyObject *pDict = PyModule_GetDict(pModule);
			if (!pDict)
			{
				std::cerr << "ERROR: Failed to retrieve dico in " << pluginName << ". Not loading..." << std::endl;
				Py_DECREF(pModule);
				continue;
			}

			PyObject *pPluginObject = PyDict_GetItemString(pDict, "plugin_object");
			Py_DECREF(pDict);
			if (!pPluginObject)
			{
				std::cerr << "ERROR: Failed to retrieve plugin_object in " << pluginName << ". Not loading..." << std::endl;
				Py_DECREF(pModule);
				return false;
			}

			PyObject* pInitStr = PyUnicode_FromString("init");
			PyObject* pResult = PyObject_CallMethodObjArgs(pPluginObject, pInitStr, nullptr);
			Py_DECREF(pInitStr);
			if (!pResult)
			{
				std::cerr << "ERROR: Calling the python plugin " << pluginName << "::init failed. Not loading..." << std::endl;
				Py_DECREF(pModule);
				continue;
			}
			Py_DECREF(pResult);
			m_scripts.push_back(pPluginObject);
		}
		closedir(pluginDir);

		// retrieving the handled commands
		for (auto* script: m_scripts)
		{
			PyObject* pCommandsStr = PyUnicode_FromString("handledCommands");
			PyObject* pResult = PyObject_CallMethodObjArgs(script, pCommandsStr, nullptr);
			Py_DECREF(pCommandsStr);
			if (!pResult)
			{
				std::cerr << "ERROR: Calling the python plugin::handledComands failed." << std::endl;
				continue;
			}
			if (!PyList_Check(pResult))
			{
				std::cerr << "ERROR: Result is not a list";
				Py_DECREF(pResult);
				continue;
			}

			Py_ssize_t len = PyList_Size(pResult);
			PyObject* itemAt {nullptr};
			for (auto i = 0; i < len; ++i)
			{
				itemAt = PyList_GetItem(pResult, i);
				if (!itemAt)
				{
					std::cerr << "ERROR: Failed to get item at " << i << std::endl;
				}
				const char* cmd = PyUnicode_AsUTF8(itemAt);
				addHandledCommand(cmd);
				m_handledCommandsByScript[cmd] = script;
			}
			Py_DECREF(pResult);
		}
		return true;
	}

	bool fini(Context& context) override
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		// Call the function with the arguments.
		for (auto* script: m_scripts)
		{
			PyObject* pFiniStr = PyUnicode_FromString("fini");
			PyObject* pResult = PyObject_CallMethodObjArgs(script, pFiniStr, nullptr);
			Py_DECREF(pFiniStr);
			if (!pResult)
			{
				std::cerr << "ERROR: Calling the python plugin fini failed" << std::endl;
				continue;
			}
			Py_DECREF(pResult);
			Py_DECREF(script);
		}
		m_scripts.clear();
		return true;
	}

	bool handle(Context& context, const Data& data) override
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;

		bool ret = true;

		auto it = m_handledCommandsByScript.find(data.command);
		if (it != m_handledCommandsByScript.end())
		{
			PyObject* pydata = dataToPyObject(data);
			PyObject* pyctx = PyDict_New();

			PyObject* pResult = PyObject_CallMethodObjArgs(it->second, PyUnicode_FromString("handle"), pyctx, pydata, nullptr);
			// Print a message if calling the method failed.
			if (!pResult)
			{
				std::cerr << "ERROR: Calling the python plugin handle failed" << std::endl;
				ret = false;
			}
			Py_DECREF(pResult);
			Py_DECREF(pydata);
			Py_DECREF(pyctx);
		}
		return ret;
	}
protected:
	PyObject* dataToPyObject(const Data& data) const
	{
		PyObject* pydata = PyDict_New();
		PyObject* cmd = PyUnicode_FromString(data.command.c_str());
		PyDict_SetItem(pydata, PyUnicode_FromString("command"), cmd);
		Py_DECREF(cmd);

		PyObject* args = PyList_New(0);
		for (auto& it: data.params)
		{
			PyObject* arg = PyUnicode_FromString(it.c_str());
			PyList_Append(args, arg);
			// Py_DECREF(arg);
		}
		PyDict_SetItem(pydata, PyUnicode_FromString("params"), args);
		Py_DECREF(args);

		return pydata;
	}

	PyObject* contextToPyObject(const Context& ctx)
	{
		return nullptr;
	}

private:
	std::list<PyObject*> m_scripts;
	std::map<std::string, PyObject*> m_handledCommandsByScript;
};

extern "C"
{
	Python plugin_object;
}
