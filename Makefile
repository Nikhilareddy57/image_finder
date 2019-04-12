CC=g++
CFLAGS=-g -Wall
MKLFLAGS=-m64 -I ~/intel/mkl/include -L~/intel/mkl/lib/intel64 -Wl,--no-as-needed ~/intel/mkl/lib/intel64/libmkl_scalapack_lp64.a ~/intel/mkl/lib/intel64/libmkl_intel_lp64.a ~/intel/mkl/lib/intel64/libmkl_gnu_thread.a ~/intel/mkl/lib/intel64/libmkl_core.a ~/intel/mkl/lib/intel64/libmkl_blacs_intelmpi_lp64.a -lgomp -lpthread -lm -ldl
OPENFLAGS=-I /include/ -L/lib/ -lopenblas -lpthread
PFLAGS= -lpthread
RM=/bin/rm -f

all: main
layer1.o: header1.h layer1.cpp
	${CC} ${CFLAGS} -c layer1.cpp
layer2.o: header2.h layer2.cpp
	${CC} ${CFLAGS} -c layer2.cpp
convolution.o: header2.h convolution.cpp
	${CC} ${CFLAGS} -c convolution.cpp
matrix_activation.o: header2.h matrix_activation.cpp
	${CC} ${CFLAGS} -c matrix_activation.cpp 
subsampling.o: header1.h subsampling.cpp
	${CC} ${CFLAGS} -c subsampling.cpp
conversion.o: header1.h conversion.cpp
	${CC} ${CFLAGS} -c conversion.cpp
mklmult.o: header3.h mklmult.cpp
	${CC} ${MKLFLAGS} -c mklmult.cpp
pthread.o: header3.h pthread.cpp
	${CC} ${PFLAGS} -c pthread.cpp 
normal.o: header3.h normal.cpp
	${CC} ${CFLAGS} -c normal.cpp
openmult.o: header3.h openmult.cpp
	${CC} ${CFLAGS} -c openmult.cpp
main: layer1.o layer2.o conversion.o subsampling.o matrix_activation.o convolution.o mklmult.o pthread.o normal.o openmult.o
	${CC} layer1.o layer2.o conversion.o subsampling.o matrix_activation.o convolution.o mklmult.o pthread.o normal.o openmult.o -o hello ${CFLAGS} ${MKLFLAGS} ${OPENFLAGS} ${PFLAGS}
clean:
	${RM} *.o main
