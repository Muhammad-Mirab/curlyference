project('curlyference', 'c',
  version : '1.0',
  license : 'LGPL',
  default_options : ['warning_level=1'])

# Set up GStreamer dependencies
gst_dep = dependency('gstreamer-1.0', version : '>=1.20')
gstvideo_dep = dependency('gstreamer-video-1.0', version : '>=1.20')

# Install path for plugins (default: lib/gstreamer-1.0)
plugins_install_dir = join_paths(get_option('libdir'), 'gstreamer-1.0')

# Compiler flags
plugin_c_args = [
  '-DHAVE_CONFIG_H',
  '-DPACKAGE="gst-curlyference"',
]

# Config header
cdata = configuration_data()
cdata.set_quoted('PACKAGE_VERSION', meson.project_version())
cdata.set_quoted('PACKAGE', 'gst-curlyference')
cdata.set_quoted('GST_LICENSE', 'LGPL')
cdata.set_quoted('GST_API_VERSION', '1.0')
cdata.set_quoted('GST_PACKAGE_NAME', 'GStreamer CurlyBracket AI inference')
cdata.set_quoted('GST_PACKAGE_ORIGIN', 'https://mirab.pythonanywhere.com')
configure_file(output : 'config.h', configuration : cdata)

# Sources
curlyference_sources = files(
  'src/gstcurlyference.c',
  'src/gstcurlyference.h',
)

# Build plugin
gstcurlyference = library('gstcurlyference',
  curlyference_sources,
  c_args: plugin_c_args,
  dependencies : [gst_dep, gstvideo_dep],
  install : true,
  install_dir : plugins_install_dir,
)

