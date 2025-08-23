#ifndef __GST_CURLYFERENCE_H__
#define __GST_CURLYFERENCE_H__

#include <gst/video/gstvideofilter.h>

G_BEGIN_DECLS

#define GST_TYPE_CURLYFERENCE            (gst_curlyference_get_type())
#define GST_CURLYFERENCE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_CURLYFERENCE,GstCurlyference))
#define GST_CURLYFERENCE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_CURLYFERENCE,GstCurlyferenceClass))
#define GST_IS_CURLYFERENCE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_CURLYFERENCE))
#define GST_IS_CURLYFERENCE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_CURLYFERENCE))

typedef struct _GstCurlyference      GstCurlyference;
typedef struct _GstCurlyferenceClass GstCurlyferenceClass;

struct _GstCurlyference {
  GstVideoFilter parent;
};

struct _GstCurlyferenceClass {
  GstVideoFilterClass parent_class;
};

GType gst_curlyference_get_type (void);

G_END_DECLS

#endif /* __GST_CURLYFERENCE_H__ */

