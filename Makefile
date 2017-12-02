PACKAGE = vl6180_pi
VERSION = 0.0.1

CC      ?= gcc
CFLAGS += -Iinclude -DVERSION=\"$(VERSION)\" -fPIC -g
LDFLAGS  += 
prefix   ?= /usr/local/

headers    = $(wildcard include/*.h)
lib_hdr    = $(wildcard src/*.h)
lib_src    = $(wildcard src/*.c)
lib_libs   = 
test_hdr   = $(wildcard test/*.h)
test_src   = $(wildcard test/*.c)
test_libs  = $(lib_libs)
extra_dist = Makefile README.md
dist_files = $(headers) $(lib_hdr) $(lib_src) $(test_hdr) $(test_src) $(extra_dist)

EXEEXT    =
LIBEXT    = .so
STATEXT   = .a

.PHONY: all check clean install uninstall dist

all: $(PACKAGE)$(LIBEXT) $(PACKAGE)$(STATEXT)

$(PACKAGE)$(LIBEXT): $(patsubst %.c, %.o, $(lib_src))
	$(CC) -shared -fPIC $(CFLAGS) $(LDFLAGS) $^ $(lib_libs) -o lib$@

$(PACKAGE)$(STATEXT): $(patsubst %.c, %.o, $(lib_src))
	ar rcs lib$@ $^

check: test-$(PACKAGE)$(EXEEXT)	
	./test-$(PACKAGE)$(EXEEXT)

test-$(PACKAGE)$(EXEEXT): $(PACKAGE)$(STATEXT) $(patsubst %.c, %.o, $(test_src))
	$(CC) -static $(CFLAGS) $(patsubst %.c, %.o, $(test_src)) $(LDFLAGS) -L. -l$(PACKAGE) $(test_libs) -o $@

clean: 
	rm -f src/*.o src/*.d test/*.o test/*.d lib$(PACKAGE)$(LIBEXT) test-$(PACKAGE)$(EXEEXT) lib$(PACKAGE)$(STATEXT)	

dist:
	mkdir $(PACKAGE)-$(VERSION)
	cp --parents $(dist_files) $(PACKAGE)-$(VERSION)
	tar -czvf $(PACKAGE)-$(VERSION).tar.gz $(PACKAGE)-$(VERSION)
	rm -rf $(PACKAGE)-$(VERSION)

install: $(PACKAGE)$(LIBEXT)
	mkdir -p $(prefix)/include/$(PACKAGE)
	cp $(headers) $(prefix)/include/$(PACKAGE)
	mkdir -p $(prefix)/lib
	cp lib$(PACKAGE)$(LIBEXT) $(prefix)/lib/lib$(PACKAGE)$(LIBEXT)
	cp lib$(PACKAGE)$(STATEXT) $(prefix)/lib/lib$(PACKAGE)$(STATEXT)

uninstall:
	rm -r $(prefix)/include/$(PACKAGE)
	rm $(prefix)/lib/lib$(PACKAGE)$(LIBEXT)
	rm $(prefix)/lib/lib$(PACKAGE)$(STATEXT)

%.o : %.cpp
	$(CC) $(CFLAGS) -MD -c $< -o $(patsubst %.c, %.o, $<)	

ifneq "$(MAKECMDGOALS)" "clean"
deps  = $(patsubst %.c, %.d, $(lib_src))
deps += $(patsubst %.c, %.d, $(test_src))
-include $(deps)
endif
