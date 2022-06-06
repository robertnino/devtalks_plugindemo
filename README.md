A demo app to demonstrate the use of plugins in C++ and Python
==========

### Requirements:

- C++ 20
- Python >= 3.8
- Linux
- GCC/CLang
- CMake >= 3.16

### Compilation
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/path/to/the/install_dir -DCMAKE_C_COMPILER=/path/to/the/c/compiler -DCMAKE_CXX_COMPILER=/path/to/the/c++/compiler
make install

### Running
```
/path/to/the/install_dir/bin/pluginDemo
```

You should be seeing something similar to this:
```
  $ ~/bin/pluginDemo/bin/pluginDemo 
  Got file libfoo.so with plugin name foo
  virtual bool Foo::init(Context &)
  Got file libtime.so with plugin name time
  virtual bool Time::init(Context &)
  Got file libpython.so with plugin name python
  Python::Python()
  virtual bool Python::init(Context &)
  Got file pp_foo.py with plugin name pp_foo
  Python # Foo::__init__
  Python # Foo::init
  Got file pp_bar.py with plugin name pp_bar
  Python # Bar::__init__
  Python # Bar::init
  Got file pp_poo.py with plugin name pp_poo
  Python # Poo::__init__
  Python # Poo::init
  Got file libbar.so with plugin name bar
  virtual bool Bar::init(Context &)
  Got file libwhoami.so with plugin name whoami
  virtual bool WhoAmI::init(Context &)
  cmd > 
```

Type `help` for available commands
