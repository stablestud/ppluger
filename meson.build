project('pplugger', 'cpp',
  version : '0.1',
  meson_version : '>=0.46.0',
  default_options :
    ['warning_level=3',
      'cpp_std=c++17',
      'default_library=both'])

srcfiles = files(
  'src/exceptions.cpp',
  'src/memory.cpp',
  'src/modules.cpp',
  'src/process.cpp',
  'src/types.cpp',
  'src/utils.cpp',
  'src/windows.cpp')

inclheaders = files(
  'include/exceptions.hpp', 
  'include/macros.hpp',
  'include/memory.hpp',
  'include/modules.hpp',
  'include/process.hpp',
  'include/pplugger.hpp',
  'include/types.hpp',
  'include/utils.hpp')

library(meson.project_name(), srcfiles, include_directories : 'include', install : true, version : meson.project_version())
install_headers(inclheaders, subdir : meson.project_name())
