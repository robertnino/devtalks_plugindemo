class DemoPlugin:
	def __init__(self, name):
		self._handledCommands = []
		self._name = name
		pass

	def handle(self, context, data):
		pass

	def handledCommands(self):
		return self._handledCommands

	def addHandledCommand(self, cmd):
		self._handledCommands.append(cmd)

	def help(self):
		print("\tPython # {} help".format(self._name))
		for cmd in self._handledCommands:
			print("\t\t{}".format(cmd))

	def init(self, ctx):
		pass

	def fini(self, ctx):
		pass

if __name__ == '__main__':
	DemoPlugin('plugin')
