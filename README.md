<h1 align="center">✨ Curlyference – GStreamer + OpenCV Video Filter ✨</h1>

Curlyference is a custom 🎥 GStreamer video filter plugin that applies OpenCV-based image processing on video streams.  
Currently, it demonstrates edge detection ⚡ (Canny filter) but can be extended to run AI inference 🤖 or any other OpenCV pipeline.

---

🔥 Features
- 🧩 GStreamer plugin (curlyference) built as a GstVideoFilter.
- 🎨 Supports common formats: RGB, BGR, RGBA, BGRA, GRAY8.
- 🖼️ Uses OpenCV for processing.
- 🎯 Default processing:
  - 🌓 Converts input to grayscale.
  - ✂️ Applies Canny edge detection.
  - 🌈 Outputs edges in color (or grayscale depending on input format).
- 🔌 Drop-in replacement for any GStreamer filter element.

---

📦 Dependencies
Make sure you have the following installed:

- 📺 GStreamer 1.26.5 (or compatible)
- 🧠 OpenCV 4.x
- 🛠️ Meson + Ninja (or GNU Make if you wire up your own build system)
- 💻 C++17 capable compiler (e.g., g++ or clang++)

On Arch/Manjaro/Garuda:
sudo pacman -S gstreamer gst-plugins-base gst-plugins-good opencv meson ninja

---

⚙️ Building

Clone and build with Meson/Ninja (recommended):

git clone https://github.com/yourusername/curlyference.git  
cd curlyference  
meson setup build  
ninja -C build  

This will produce a shared object (libgstcurlyference.so) inside build/.

---

▶️ Running

Set the plugin path and launch a pipeline:

GST_PLUGIN_PATH=$PWD/build gst-launch-1.0 v4l2src device=/dev/video0 ! videoconvert ! tee name=t \
  t. ! queue ! curlyference ! videoconvert ! autovideosink \
  t. ! queue ! videoconvert ! autovideosink

🎥 Explanation of pipeline:
- 📷 v4l2src device=/dev/video0 → Capture from your webcam.  
- 🎛️ videoconvert → Ensures format compatibility.  
- 🔀 tee → Splits the stream into two branches.  
- First branch: passes through curlyference (edge detection ⚡) → autovideosink.  
- Second branch: direct passthrough → autovideosink for side-by-side comparison.

🖥️ You should see two windows:
- Processed (edge-detected) video 🕸️
- Raw video 🎬

---

🛠️ Development Notes
- 🐞 Logs can be enabled with:
  GST_DEBUG=curlyference:4 GST_PLUGIN_PATH=$PWD/build gst-launch-1.0 ...
- ⚠️ If you see "Unsupported format, assuming RGB" warnings, the input format may not be handled explicitly — add more format conversions in the code if needed.
- 📝 Modify gst_curlyference_transform_frame to replace the Canny pipeline with your own OpenCV or AI inference.

---

📜 License
LGPL v2.1 (same as GStreamer).  
