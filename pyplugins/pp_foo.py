
import DemoPlugin

class Foo(DemoPlugin.DemoPlugin):
	def __init__(self):
		super().__init__('pyfoo')
		print("Python # Foo::__init__")
		self.addHandledCommand("pyfoo")
		pass

	def handle(self,context = None, data = None):
		print("Python # Foo::handle")
		pass

	def init(self, context = None):
		print("Python # Foo::init")
		pass

	def fini(self, context = None):
		print("Python # Foo::fini")
		pass

plugin_object = Foo()
