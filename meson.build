project('curlyference', 'cpp', 'c',
  version : '1.0',
  license : 'LGPL',
  default_options : [
    'warning_level=1',
    'cpp_std=c++14',
    'c_std=c99'
  ])

# Check C++ compiler
cpp_compiler = meson.get_compiler('cpp')
c_compiler = meson.get_compiler('c')

# Check for C++ standard library
cpp_std_dep = cpp_compiler.find_library('stdc++', required : true)

# Set up GStreamer dependencies
gst_dep = dependency('gstreamer-1.0', version : '>=1.20')
gstvideo_dep = dependency('gstreamer-video-1.0', version : '>=1.20')

# OpenCV dependencies
opencv_dep = dependency('opencv4', required : false)
if not opencv_dep.found()
  opencv_dep = dependency('opencv', version : '>=3.0', required : true)
endif

# Check for required OpenCV modules
opencv_core = dependency('opencv4', modules : ['opencv_core'], required : false)
opencv_imgproc = dependency('opencv4', modules : ['opencv_imgproc'], required : false)

if not opencv_core.found()
  opencv_core = dependency('opencv', required : true)
endif

if not opencv_imgproc.found()
  opencv_imgproc = dependency('opencv', required : true)
endif

# Optional OpenCV modules for advanced features
opencv_objdetect = dependency('opencv4', modules : ['opencv_objdetect'], required : false)
opencv_imgcodecs = dependency('opencv4', modules : ['opencv_imgcodecs'], required : false)
opencv_features2d = dependency('opencv4', modules : ['opencv_features2d'], required : false)

# Install path for plugins (default: lib/gstreamer-1.0)
plugins_install_dir = join_paths(get_option('libdir'), 'gstreamer-1.0')

# Compiler flags
plugin_cpp_args = [
  '-DHAVE_CONFIG_H',
  '-DPACKAGE="gst-curlyference"',
  '-DGST_USE_UNSTABLE_API',
  '-Wno-deprecated-declarations',  # Suppress OpenCV deprecation warnings
  '-fPIC'
]

plugin_c_args = [
  '-DHAVE_CONFIG_H',
  '-DPACKAGE="gst-curlyference"',
  '-DGST_USE_UNSTABLE_API',
  '-fPIC'
]

# Config header
cdata = configuration_data()
cdata.set_quoted('PACKAGE_VERSION', meson.project_version())
cdata.set_quoted('PACKAGE', 'gst-curlyference')
cdata.set_quoted('GST_LICENSE', 'LGPL')
cdata.set_quoted('GST_API_VERSION', '1.0')
cdata.set_quoted('GST_PACKAGE_NAME', 'GStreamer CurlyBracket AI inference with OpenCV')
cdata.set_quoted('GST_PACKAGE_ORIGIN', 'https://mirab.pythonanywhere.com')

# OpenCV version detection
opencv_version = opencv_dep.version()
cdata.set_quoted('OPENCV_VERSION', opencv_version)

# Feature flags based on available OpenCV modules
cdata.set('HAVE_OPENCV_OBJDETECT', opencv_objdetect.found())
cdata.set('HAVE_OPENCV_IMGCODECS', opencv_imgcodecs.found())
cdata.set('HAVE_OPENCV_FEATURES2D', opencv_features2d.found())

configure_file(output : 'config.h', configuration : cdata)

# Linker flags for proper C++/C linking
plugin_link_args = []
if host_machine.system() == 'linux'
  plugin_link_args += ['-Wl,--no-undefined']
elif host_machine.system() == 'darwin'
  plugin_link_args += ['-Wl,-undefined,error']
endif

# Include directories
plugin_inc = include_directories('.')

# Sources
curlyference_sources = files(
  'src/gstcurlyference.cpp',
)

curlyference_headers = files(
  'src/gstcurlyference.h',
)

# Collect all dependencies
plugin_deps = [
  gst_dep,
  gstvideo_dep,
  opencv_dep,
  cpp_std_dep,
]

# Add optional dependencies if found
if opencv_objdetect.found()
  plugin_deps += [opencv_objdetect]
endif

if opencv_imgcodecs.found()
  plugin_deps += [opencv_imgcodecs]
endif

if opencv_features2d.found()
  plugin_deps += [opencv_features2d]
endif

# Build plugin
gstcurlyference = shared_library('gstcurlyference',
  curlyference_sources,
  include_directories : plugin_inc,
  cpp_args: plugin_cpp_args,
  c_args: plugin_c_args,
  link_args: plugin_link_args,
  dependencies : plugin_deps,
  install : true,
  install_dir : plugins_install_dir,
  install_rpath : get_option('prefix') / get_option('libdir'),
  gnu_symbol_visibility : 'hidden',  # Hide internal symbols
  soversion : '0',
  version : meson.project_version()
)

# Summary
summary({
  'GStreamer version': gst_dep.version(),
  'OpenCV version': opencv_version,
  'OpenCV objdetect': opencv_objdetect.found() ? 'yes' : 'no',
  'OpenCV imgcodecs': opencv_imgcodecs.found() ? 'yes' : 'no',
  'OpenCV features2d': opencv_features2d.found() ? 'yes' : 'no',
  'Plugin install dir': plugins_install_dir,
}, section: 'Configuration')

# Optional: Install development headers
install_headers(curlyference_headers,
  subdir : 'gstreamer-1.0/gst/curlyference'
)

# Optional: Generate pkgconfig file for development
pkg = import('pkgconfig')
pkg.generate(gstcurlyference,
  description : 'GStreamer CurlyBracket AI inference plugin with OpenCV',
  url : 'https://mirab.pythonanywhere.com'
)