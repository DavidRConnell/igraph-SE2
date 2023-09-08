CC = gcc
CFLAGS = -Wall -Wextra -fPIC
SE2BUILD := $(BUILD)/se2

# TODO: Change to if statements to detect OS
LIB_EXT = so

vpath %.$(LIB_EXT) $(LIB)
vpath %.h $(INCLUDE)
vpath %.o $(SE2BUILD)

sources := $(wildcard ./se2_*.c)
objs := $(subst ./,$(SE2BUILD)/,$(sources:.c=.o))

ifdef SE2_PRINT_PATH
CFLAGS += -DSE2_PRINT_PATH=$(SE2_PRINT_PATH)
endif

.PHONY: all
all: libse2

.PHONY: libse2
libse2: CFLAGS += -fopenmp
libse2: $(SE2BUILD) $(LIB)/libse2.$(LIB_EXT)

$(LIB)/libse2.$(LIB_EXT): $(objs)

%.so:
	$(LINK.c) -shared $^ -o $@
%.a:
	$(AR) rv $@ $^

.PHONY: $(SE2BUILD)
$(SE2BUILD):
	@[ -d $(SE2BUILD) ] || mkdir $(SE2BUILD)

$(SE2BUILD)/se2_core.o: CFLAGS += -fopenmp
$(SE2BUILD)/se2_core.o: se2_random.h se2_seeding.h se2_modes.h se2_reweight_graph.h \
	speak_easy_2.h
$(SE2BUILD)/se2_random.o: se2_random.h
$(SE2BUILD)/se2_seeding.o: se2_seeding.h se2_random.h speak_easy_2.h
$(SE2BUILD)/se2_reweight_graph.o: se2_reweight_graph.h
$(SE2BUILD)/se2_modes.o: se2_modes.h se2_label.h se2_partitions.h speak_easy_2.h
$(SE2BUILD)/se2_label.o: se2_label.h se2_partitions.h
$(SE2BUILD)/se2_partitions.o: se2_partitions.h se2_random.h

$(SE2BUILD)/%.o: %.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

.PHONY: debug
debug: CFLAGS += -g3 -O0
debug: libse2

.PHONY: clean
clean:
	-rm -rf $(SE2BUILD)

.PHONY: clean-dist
clean-dist: dist
	-rm $(LIB)/libse2.${LIB_EXT}
