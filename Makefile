TARGET = analyser
CXX = g++
#CXXFLAGS="...":
#-fsanitize=address -g - for segfaults
#-O0, -O1, -O2

all: ${TARGET}
clean:
	rm -rf ${TARGET} *.o $(CXXFLAGS)
vars.o: Vars.cpp Vars.hpp
	$(CXX) -o vars.o -c Vars.cpp $(CXXFLAGS)
random.o: vars.o RandomSource.hpp RandomSource.cpp
	$(CXX) -o random.o -c RandomSource.cpp $(CXXFLAGS)
streams.o: vars.o StreamBase.hpp Bitstream.hpp BitstreamInput.cpp BitstreamOutput.cpp FileStream.hpp FileStream.cpp Streams.cpp StringStream.cpp
	$(CXX) -o streams.o -c Streams.cpp $(CXXFLAGS)
base64.o: vars.o streams.o Base64.hpp Base64.cpp
	$(CXX) -o base64.o -c Base64.cpp $(CXXFLAGS)
rle.o: vars.o streams.o RLE.hpp RLE.cpp
	$(CXX) -o rle.o -c RLE.cpp $(CXXFLAGS)
xor.o: vars.o streams.o Xor.hpp Xor.cpp
	$(CXX) -o xor.o -c Xor.cpp $(CXXFLAGS)
hamming.o: vars.o streams.o Hamming.hpp Hamming.cpp
	$(CXX) -o hamming.o -c Hamming.cpp $(CXXFLAGS)
aes.o: vars.o streams.o AES.hpp AES.cpp
	$(CXX) -o aes.o -c AES.cpp $(CXXFLAGS)
long_math.o: vars.o random.o LongMath.hpp LongMath.cpp
	$(CXX) -o long_math.o -c LongMath.cpp $(CXXFLAGS)
rsa.o: vars.o long_math.o RSA.hpp RSA.cpp
	$(CXX) -o rsa.o -c RSA.cpp $(CXXFLAGS)
el_gamal.o: vars.o long_math.o ElGamal.hpp ElGamal.cpp
	$(CXX) -o el_gamal.o -c ElGamal.cpp $(CXXFLAGS)
diffie_hellman.o: vars.o long_math.o DiffieHellman.hpp DiffieHellman.cpp
	$(CXX) -o diffie_hellman.o -c DiffieHellman.cpp $(CXXFLAGS)
main.o: vars.o random.o streams.o base64.o rle.o xor.o hamming.o aes.o long_math.o rsa.o el_gamal.o diffie_hellman.o Main.cpp
	$(CXX) -o main.o -c Main.cpp $(CXXFLAGS)
${TARGET}: main.o
	$(CXX) -o ${TARGET} vars.o random.o streams.o base64.o rle.o xor.o hamming.o aes.o long_math.o rsa.o el_gamal.o diffie_hellman.o main.o -lgmp -lgmpxx $(CXXFLAGS)
