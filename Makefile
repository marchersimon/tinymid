build:
	g++ src/*.cpp -o tinymid -Isrc/include

debug: 
	g++ src/*.cpp -o tinymid -Isrc/include -g
