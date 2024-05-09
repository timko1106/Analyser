TARGET = analyser
#CXXFLAGS="...":
#-fsanitize=address -g - for segfaults
#-O0, -O1, -O2

all: ${TARGET}
clean:
	rm -rf ${TARGET} *.o $(CXXFLAGS)
vars.o: Vars.cpp Vars.hpp
	g++ -o vars.o -c Vars.cpp $(CXXFLAGS)
streams.o: vars.o StreamBase.hpp Bitstream.hpp BitstreamInput.cpp BitstreamOutput.cpp FileStream.hpp Streams.cpp
	g++ -o streams.o -c Streams.cpp $(CXXFLAGS)
base64.o: vars.o streams.o Base64.hpp Base64.cpp
	g++ -o base64.o -c Base64.cpp $(CXXFLAGS)
rle.o: vars.o streams.o RLE.hpp RLE.cpp
	g++ -o rle.o -c RLE.cpp $(CXXFLAGS)
xor.o: vars.o streams.o Xor.hpp Xor.cpp
	g++ -o xor.o -c Xor.cpp $(CXXFLAGS)
hamming.o: vars.o streams.o Hamming.hpp Hamming.cpp
	g++ -o hamming.o -c Hamming.cpp $(CXXFLAGS)
main.o: vars.o streams.o base64.o rle.o xor.o hamming.o Main.cpp
	g++ -o main.o -c Main.cpp $(CXXFLAGS)
${TARGET}: vars.o streams.o rle.o xor.o hamming.o main.o
	g++ -o ${TARGET} vars.o streams.o rle.o xor.o hamming.o main.o $(CXXFLAGS)
