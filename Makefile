run:
	mkdir bin
	clang++ -std=c++11 -stdlib=libc++ main.cpp -I./include -framework OpenGl -lglfw -o bin/main.app
	./bin/main.app