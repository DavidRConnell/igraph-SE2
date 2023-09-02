export LIB := $(PWD)/lib
export INCLUDE := $(PWD)/include
export CPPFLAGS := -I$(INCLUDE)

.PHONY: all lib mex check clean

all: lib

lib:
	@cd src; $(MAKE) libse2

toolbox: lib
	@cd toolbox; $(MAKE) all

check: lib
	@cd tests; $(MAKE) check

clean:
	@cd src; $(MAKE) clean
	@cd toolbox; $(MAKE) clean
	@cd tests; $(MAKE) clean
