TARGET = analyser
CXX = g++
#CXXFLAGS="...":
#-fsanitize=address -g - for segfaults
#-O0, -O1, -O2
CXXFLAGS = -O3 -pedantic -Wall -Wextra -Werror -Wuninitialized -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -pedantic-errors -Wextra -Wcast-align -Wcast-qual -Wconversion -Werror -Wfloat-equal -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimport  -Winit-self  -Winvalid-pch  -Wmissing-field-initializers -Wmissing-format-attribute  -Wmissing-include-dirs -Wmissing-noreturn -Wpacked  -Wpointer-arith -Wredundant-decls -Wshadow -Wstack-protector -Wswitch-default -Wswitch-enum -Wunreachable-code -Wvariadic-macros -Wwrite-strings -fno-rtti
#-fopt-info-inline-optimized-missed

all: ${TARGET}
clean:
	rm -rf ${TARGET} *.o
vars.o: vars.cpp vars.hpp
	$(CXX) -o vars.o -c vars.cpp $(CXXFLAGS) -fPIC
streams.o: vars.o stream_base.hpp bitstream.hpp bitstream_input.cpp bitstream_output.cpp filestream.hpp filestream.cpp streams.cpp stringstream.cpp
	$(CXX) -o streams.o -c streams.cpp $(CXXFLAGS) -fPIC
random.o: vars.o random_source.hpp random_source.cpp
	$(CXX) -o random.o -c random_source.cpp $(CXXFLAGS) -fPIC
long_math.o: vars.o random.o long_math.hpp long_math.cpp
	$(CXX) -o long_math.o -c long_math.cpp $(CXXFLAGS) -fPIC
crypt_base.o: vars.o streams.o long_math.o crypt_base.cpp crypt_base.hpp 
	$(CXX) -o crypt_base.o -c crypt_base.cpp $(CXXFLAGS) -fPIC
rle.o: vars.o streams.o crypt_base.o rle.hpp rle.cpp
	$(CXX) -o rle.o -c rle.cpp $(CXXFLAGS) -fPIC
base64.o: vars.o streams.o crypt_base.o base64.hpp base64.cpp
	$(CXX) -o base64.o -c base64.cpp $(CXXFLAGS) -fPIC
xor.o: vars.o streams.o crypt_base.o xor.hpp xor.cpp
	$(CXX) -o xor.o -c xor.cpp $(CXXFLAGS) -fPIC
hamming.o: vars.o streams.o crypt_base.o hamming.hpp hamming.cpp
	$(CXX) -o hamming.o -c hamming.cpp $(CXXFLAGS) -fPIC
aes.o: vars.o streams.o crypt_base.o aes.hpp aes.cpp
	$(CXX) -o aes.o -c aes.cpp $(CXXFLAGS) -fPIC
rsa.o: vars.o long_math.o crypt_base.o rsa.hpp rsa.cpp
	$(CXX) -o rsa.o -c rsa.cpp $(CXXFLAGS) -fPIC
el_gamal.o: vars.o long_math.o crypt_base.o el_gamal.hpp el_gamal.cpp
	$(CXX) -o el_gamal.o -c el_gamal.cpp $(CXXFLAGS) -fPIC
diffie_hellman.o: vars.o long_math.o crypt_base.o diffie_hellman.hpp diffie_hellman.cpp
	$(CXX) -o diffie_hellman.o -c diffie_hellman.cpp $(CXXFLAGS) -fPIC
tests.o: vars.o random.o streams.o crypt_base.o base64.o rle.o xor.o hamming.o aes.o long_math.o rsa.o el_gamal.o diffie_hellman.o tests.hpp tests.cpp
	$(CXX) -o tests.o -c tests.cpp $(CXXFLAGS) -fPIC
main.o: vars.o random.o streams.o crypt_base.o base64.o rle.o xor.o hamming.o aes.o long_math.o rsa.o el_gamal.o diffie_hellman.o tests.o main.cpp
	$(CXX) -o main.o -c main.cpp $(CXXFLAGS) -fPIC
${TARGET}: main.o
	$(CXX) -o ${TARGET} vars.o random.o streams.o long_math.o crypt_base.o xor.o base64.o hamming.o rle.o aes.o rsa.o el_gamal.o diffie_hellman.o tests.o main.o -lgmp -lgmpxx $(CXXFLAGS)
