plugin_c_args = ['-DHAVE_CONFIG_H']

cdata = configuration_data()
cdata.set_quoted('PACKAGE_VERSION', gst_version)
cdata.set_quoted('PACKAGE', 'gst-curlyference')
cdata.set_quoted('GST_LICENSE', 'LGPL')
cdata.set_quoted('GST_API_VERSION', api_version)
cdata.set_quoted('GST_PACKAGE_NAME', 'GStreamer CurlyBracket AI inference')
cdata.set_quoted('GST_PACKAGE_ORIGIN', 'https://mirab.pythonanywhere.com')
configure_file(output : 'config.h', configuration : cdata)

# Dependencies
gst_dep = dependency('gstreamer-1.0', version : '>=1.20')
gstvideo_dep = dependency('gstreamer-video-1.0', version : '>=1.20')

# Curlyference plugin sources
curlyference_sources = [
  'src/gstcurlyference.c',
  'src/gstcurlyference.h',
]

gstcurlyference = library('gstcurlyference',
  curlyference_sources,
  c_args: plugin_c_args,
  dependencies : [gst_dep, gstvideo_dep],
  install : true,
  install_dir : plugins_install_dir,
)

