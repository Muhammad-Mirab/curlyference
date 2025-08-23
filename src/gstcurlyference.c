#include "gstcurlyference.h"
#include <gst/video/video.h>

GST_DEBUG_CATEGORY_STATIC (gst_curlyference_debug);
#define GST_CAT_DEFAULT gst_curlyference_debug

G_DEFINE_TYPE (GstCurlyference, gst_curlyference, GST_TYPE_VIDEO_FILTER);

static GstFlowReturn
gst_curlyference_transform_frame (GstVideoFilter * filter,
                                  GstVideoFrame * inframe,
                                  GstVideoFrame * outframe)
{
  // Passthrough: copy input → output
  gst_video_frame_copy (outframe, inframe);

  // You can access raw data here for inference:
  // guint8 *data = GST_VIDEO_FRAME_PLANE_DATA(inframe, 0);
  // gsize stride = GST_VIDEO_FRAME_PLANE_STRIDE(inframe, 0);
  // TODO: Insert AI inference logic.

  return GST_FLOW_OK;
}

static void
gst_curlyference_class_init (GstCurlyferenceClass * klass)
{
  GstElementClass *element_class = GST_ELEMENT_CLASS (klass);
  GstVideoFilterClass *filter_class = GST_VIDEO_FILTER_CLASS (klass);

  gst_element_class_set_static_metadata (element_class,
      "Curlyference",
      "Filter/Effect/Video",
      "AI inference passthrough video filter",
      "Muhammad Mirab Br <https://mirab.pythonanywhere.com>");

  filter_class->transform_frame = gst_curlyference_transform_frame;

  gst_element_class_add_pad_template (element_class,
      gst_pad_template_new ("sink", GST_PAD_SINK, GST_PAD_ALWAYS,
          gst_caps_from_string ("video/x-raw")));
  gst_element_class_add_pad_template (element_class,
      gst_pad_template_new ("src", GST_PAD_SRC, GST_PAD_ALWAYS,
          gst_caps_from_string ("video/x-raw")));
}

static void
gst_curlyference_init (GstCurlyference * self)
{
  // Nothing to init yet
}

static gboolean
plugin_init (GstPlugin * plugin)
{
  GST_DEBUG_CATEGORY_INIT (gst_curlyference_debug, "curlyference", 0,
      "Curlyference AI inference plugin");

  return gst_element_register (plugin, "curlyference", GST_RANK_NONE,
      GST_TYPE_CURLYFERENCE);
}

GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    curlyference,
    "Curlyference AI inference video plugin",
    plugin_init,
    "1.0",
    "LGPL",
    "GStreamer",
    "https://mirab.pythonanywhere.com"
)

