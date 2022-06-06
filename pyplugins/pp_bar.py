
import DemoPlugin

class Bar(DemoPlugin.DemoPlugin):
	def __init__(self):
		super().__init__('pybar')
		print("Python # Bar::__init__")
		self.addHandledCommand("pybar")
		pass

	def handle(self, context = None, data = None):
		print("Python # Bar::handle")
		pass

	def init(self, context = None):
		print("Python # Bar::init")
		pass

	def fini(self, context = None):
		print("Python # Bar::fini")
		pass

plugin_object = Bar()

if __name__ == '__main__':
	bar = Bar()
	bar.init()
	bar.help()
	bar.handle()
	print(bar.handledCommands())
	bar.fini()
