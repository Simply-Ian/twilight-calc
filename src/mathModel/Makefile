CXX=g++-13
C_FLAGS= --std=c++2a -fdiagnostics-color=always -fPIE -fpic -g
PYFLAGS=$(filter-out -O3, $(shell python3-config --cflags --ldflags --embed))
SRC=expression/compute_RPN.cpp expression/expr_to_RPN.cpp expression/tokenize.cpp
PY_REQ_SRC=mfp/mathFunProvider.cpp model.cpp
OBJ=$(SRC:.cpp=.o) $(PY_REQ_SRC:.cpp=.o)

.PHONY: clean all

%.o: %.cpp
	$(CXX) $(C_FLAGS) $< $(if $(filter $(PY_REQ_SRC),$<),$(PYFLAGS)) -c -o $@

clean:
	rm *.o -r
