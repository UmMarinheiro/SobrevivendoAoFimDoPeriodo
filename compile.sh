g++ facedetect_simple.cpp `pkg-config --cflags opencv4` `pkg-config --libs --static opencv4` -o facedetect_simple

g++ facedetect_extra.cpp src/transform.cpp `pkg-config --cflags opencv4` `pkg-config --libs --static opencv4` -o facedetect_extra