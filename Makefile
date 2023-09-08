export BUILD := $(PWD)/build
export LIB := $(BUILD)/lib
export INCLUDE := $(PWD)/include
export CPPFLAGS := -I$(INCLUDE)
export LDFLAGS := -ligraph

.PHONY: all
all: lib

.PHONY: lib
lib: $(BUILD)
	@cd src; $(MAKE) libse2

.PHONY: $(BUILD)
$(BUILD):
	[ -d $(BUILD) ] || mkdir $(BUILD)
	[ -d $(LIB) ] || mkdir $(LIB)

.PHONY: toolbox
toolbox: $(BUILD)
	[ -z matlab-igraph ] || git submodule update --recursive --init
	@cd matlab-igraph; $(MAKE) mxIgraph BUILD=$(BUILD)
	@cd src; $(MAKE) libse2 LDFLAGS="-L$(LIB) $(LDFLAGS)"
	@cd toolbox; $(MAKE) all CPPFLAGS="$(CPPFLAGS) -I$(PWD)/matlab-igraph/mxIgraph/include"

.PHONY: check
check: lib
	@cd tests; $(MAKE) check

.PHONY: clean
clean:
	@cd src; $(MAKE) clean
	@cd matlab-igraph; $(MAKE) clean
	@cd toolbox; $(MAKE) clean
	@cd tests; $(MAKE) clean

.PHONY: clean-dist
clean-dist: clean
	rm -rf build
