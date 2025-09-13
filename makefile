make build:
	g++ src/*.cpp `pkg-config --cflags opencv4` `pkg-config --libs --static opencv4` -o build

clean:
	rm build

run: build
	./build

compilerun:
	make
	make run