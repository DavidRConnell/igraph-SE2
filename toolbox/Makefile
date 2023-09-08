CFLAGS += -Wall -Wextra -Wno-format -Wno-format-security
LDFLAGS += -Wl,-rpath=$(LIB),-z,noexecstack -L$(LIB) -lse2 -lmxIgraph
MEXFLAGS = -R2018a

vpath %.c mex
vpath %.so $(LIB)

# TODO: Change to if statements to detect OS
MEX_EXT = mexa64

.PHONY: all
all: $(outdir) $(outdir)/SpeakEasy2.$(MEX_EXT)

$(outdir)/SpeakEasy2.$(MEX_EXT): SpeakEasy2.c libmxIgraph.so libse2.so
	mex $(CPPFLAGS) "LDFLAGS=$$LDFLAGS $(LDFLAGS)" "CFLAGS=$$CFLAGS $(CFLAGS)" \
	  "COPTIMFLAGS=$$COPTIMFLAGS $(COPTIMFLAGS)" $(MEXFLAGS) $<

.PHONY: clean
clean:
	-rm *.$(MEX_EXT)
