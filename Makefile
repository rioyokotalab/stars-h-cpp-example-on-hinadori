CXX=g++
CXXFLAGS=-std=c++14 -lstarsh -lgsl -llapacke -lcblas -lblas -fopenmp -lmpi -lgfortran
TARGET=stars-h.test

$(TARGET):src/main.cpp
	$(CXX) $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(TARGET)
