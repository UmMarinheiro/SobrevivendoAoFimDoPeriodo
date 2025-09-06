g++ facedetect_simple.cpp `pkg-config --cflags opencv4` `pkg-config --libs --static opencv4` -o facedetect_simple

g++ facedetect_extra.cpp `pkg-config --cflags opencv4` `pkg-config --libs --static opencv4` -o facedetect_extra

g++ laranja.cpp `pkg-config --cflags opencv4` `pkg-config --libs --static opencv4` -o laranja

./laranja
