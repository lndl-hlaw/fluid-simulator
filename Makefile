
.PHONY: all clean

all:
	@mkdir -p build
	@cd build && cmake .. && make

debug:
	@mkdir -p build
	@cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make
clean:
	@rm -rf build
