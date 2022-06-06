
import DemoPlugin

class Poo(DemoPlugin.DemoPlugin):
	def __init__(self):
		super().__init__('pypoo')
		print("Python # Poo::__init__")
		self.addHandledCommand("pypoo")
		self.addHandledCommand("pypa")
		self.addHandledCommand("pypy")
		pass

	def handle(self, context = None, data = None):
		print("\tPython # Poo::handle")
		print("\tPython # Poo::handle received command {} with params {}".format(data['command'], data['params']))
		pass

	def init(self, context = None):
		print("Python # Poo::init")
		pass

	def fini(self, context = None):
		print("Python # Poo::fini")
		pass

plugin_object = Poo()
