CXX=g++
CXXFLAGS=-std=c++14 -lstarsh -lgsl -llapacke -llapack -lcblas -lblas -fopenmp -lmpi -lgfortran -lmpi -g
TARGET=stars-h.test

$(TARGET):src/main.cpp
	$(CXX) $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(TARGET)
