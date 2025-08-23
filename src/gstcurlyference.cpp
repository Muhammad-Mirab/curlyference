#include "gstcurlyference.h"
#include <gst/video/video.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

// C linkage for GStreamer plugin functions
extern "C" {

GST_DEBUG_CATEGORY_STATIC (gst_curlyference_debug);
#define GST_CAT_DEFAULT gst_curlyference_debug

G_DEFINE_TYPE (GstCurlyference, gst_curlyference, GST_TYPE_VIDEO_FILTER);

static GstFlowReturn
gst_curlyference_transform_frame (GstVideoFilter * filter,
                                  GstVideoFrame * inframe,
                                  GstVideoFrame * outframe)
{
  GstCurlyference *self = GST_CURLYFERENCE (filter);
  GstVideoInfo *info = &filter->in_info;
  
  // Get frame dimensions and format
  gint width = GST_VIDEO_INFO_WIDTH (info);
  gint height = GST_VIDEO_INFO_HEIGHT (info);
  GstVideoFormat format = GST_VIDEO_INFO_FORMAT (info);
  
  // Get input frame data
  guint8 *input_data = (guint8 *) GST_VIDEO_FRAME_PLANE_DATA (inframe, 0);
  guint8 *output_data = (guint8 *) GST_VIDEO_FRAME_PLANE_DATA (outframe, 0);
  gsize stride = GST_VIDEO_FRAME_PLANE_STRIDE (inframe, 0);
  
  try {
    cv::Mat input_mat, output_mat, processed_mat;
    
    // Create OpenCV Mat based on video format
    switch (format) {
      case GST_VIDEO_FORMAT_RGB:
        input_mat = cv::Mat(height, width, CV_8UC3, input_data, stride);
        break;
      case GST_VIDEO_FORMAT_BGR:
        input_mat = cv::Mat(height, width, CV_8UC3, input_data, stride);
        break;
      case GST_VIDEO_FORMAT_RGBA:
        input_mat = cv::Mat(height, width, CV_8UC4, input_data, stride);
        break;
      case GST_VIDEO_FORMAT_BGRA:
        input_mat = cv::Mat(height, width, CV_8UC4, input_data, stride);
        break;
      case GST_VIDEO_FORMAT_GRAY8:
        input_mat = cv::Mat(height, width, CV_8UC1, input_data, stride);
        break;
      default:
        // Default to RGB if format is unknown
        input_mat = cv::Mat(height, width, CV_8UC3, input_data, stride);
        GST_WARNING_OBJECT (self, "Unsupported format, assuming RGB");
        break;
    }
    
    // Apply OpenCV image processing operations
    // Example 1: Edge detection using Canny
    if (input_mat.channels() == 3) {
      cv::Mat gray;
      cv::cvtColor(input_mat, gray, cv::COLOR_BGR2GRAY);
      cv::Canny(gray, processed_mat, 100, 200);
      // Convert back to color for output
      cv::cvtColor(processed_mat, output_mat, cv::COLOR_GRAY2BGR);
    } else if (input_mat.channels() == 1) {
      cv::Canny(input_mat, processed_mat, 100, 200);
      output_mat = processed_mat;
    } else {
      // For RGBA/BGRA, extract RGB channels first
      cv::Mat rgb_mat;
      cv::cvtColor(input_mat, rgb_mat, cv::COLOR_BGRA2BGR);
      cv::Mat gray;
      cv::cvtColor(rgb_mat, gray, cv::COLOR_BGR2GRAY);
      cv::Canny(gray, processed_mat, 100, 200);
      cv::cvtColor(processed_mat, output_mat, cv::COLOR_GRAY2BGRA);
    }
    
    // Copy processed data to output frame
    if (output_mat.isContinuous() && output_mat.total() * output_mat.elemSize() <= stride * height) {
      memcpy(output_data, output_mat.data, output_mat.total() * output_mat.elemSize());
    } else {
      // Copy line by line if strides don't match
      gsize output_stride = output_mat.step[0];
      gsize copy_size = std::min(stride, output_stride);
      for (int y = 0; y < height; y++) {
        memcpy(output_data + y * stride, 
               output_mat.data + y * output_stride, 
               copy_size);
      }
    }
    
    GST_LOG_OBJECT (self, "Processed frame %dx%d", width, height);
    
  } catch (const cv::Exception& e) {
    GST_ERROR_OBJECT (self, "OpenCV error: %s", e.what());
    // Fallback to passthrough
    gst_video_frame_copy (outframe, inframe);
    return GST_FLOW_ERROR;
  } catch (...) {
    GST_ERROR_OBJECT (self, "Unknown error in OpenCV processing");
    // Fallback to passthrough
    gst_video_frame_copy (outframe, inframe);
    return GST_FLOW_ERROR;
  }

  return GST_FLOW_OK;
}

static gboolean
gst_curlyference_set_info (GstVideoFilter * filter, GstCaps * incaps,
    GstVideoInfo * in_info, GstCaps * outcaps, GstVideoInfo * out_info)
{
  GstCurlyference *self = GST_CURLYFERENCE (filter);
  
  GST_INFO_OBJECT (self, "Setting caps: %" GST_PTR_FORMAT " -> %" GST_PTR_FORMAT,
      incaps, outcaps);
  
  // Ensure input and output formats are compatible
  if (!gst_video_info_is_equal (in_info, out_info)) {
    GST_ERROR_OBJECT (self, "Input and output formats must be the same");
    return FALSE;
  }
  
  return TRUE;
}

static void
gst_curlyference_class_init (GstCurlyferenceClass * klass)
{
  GstElementClass *element_class = GST_ELEMENT_CLASS (klass);
  GstVideoFilterClass *filter_class = GST_VIDEO_FILTER_CLASS (klass);

  gst_element_class_set_static_metadata (element_class,
      "Curlyference",
      "Filter/Effect/Video",
      "AI inference video filter with OpenCV processing",
      "Muhammad Mirab Br <https://mirab.pythonanywhere.com>");

  filter_class->transform_frame = gst_curlyference_transform_frame;
  filter_class->set_info = gst_curlyference_set_info;

  // Support common video formats
  GstCaps *caps = gst_caps_from_string (
      "video/x-raw, "
      "format=(string){RGB,BGR,RGBA,BGRA,GRAY8}, "
      "width=(int)[1,MAX], "
      "height=(int)[1,MAX], "
      "framerate=(fraction)[0/1,MAX]");

  gst_element_class_add_pad_template (element_class,
      gst_pad_template_new ("sink", GST_PAD_SINK, GST_PAD_ALWAYS, caps));
  gst_element_class_add_pad_template (element_class,
      gst_pad_template_new ("src", GST_PAD_SRC, GST_PAD_ALWAYS, caps));
  
  gst_caps_unref (caps);
}

static void
gst_curlyference_init (GstCurlyference * self)
{
  GST_INFO_OBJECT (self, "Initializing Curlyference plugin with OpenCV support");
}

static gboolean
plugin_init (GstPlugin * plugin)
{
  GST_DEBUG_CATEGORY_INIT (gst_curlyference_debug, "curlyference", 0,
      "Curlyference AI inference plugin with OpenCV");

  return gst_element_register (plugin, "curlyference", GST_RANK_NONE,
      GST_TYPE_CURLYFERENCE);
}

} // extern "C"

GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    curlyference,
    "Curlyference AI inference video plugin with OpenCV",
    plugin_init,
    "1.0",
    "LGPL",
    "GStreamer",
    "https://mirab.pythonanywhere.com"
)