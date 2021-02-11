OUTPUT	=	wdf85.so

SOURCE	+=	wdf85.c

# Widely available 'SSE4.2' optimization settings
ARCH	?=	-march=znver2 -mno-avx -mno-avx2 -mno-bmi -mno-bmi2

PHPCONF	?=	php-config
INCDIRS	=	$(shell $(PHPCONF) --includes)
CFLAGS	+=	-Wall -pedantic
CFLAGS	+=	-O3 -fomit-frame-pointer
CFLAGS	+=	-fPIC -DPIC -DHAVE_CONFIG_H -DPHP_ATOM_INC
CFLAGS	+=	$(ARCH)
LDFLAGS	+=	-shared -Wl,--build-id=none -s
LDFLAGS	+=	-Wl,-soname -Wl,$(OUTPUT)

all: $(OUTPUT)

$(OUTPUT): $(SOURCE)
	$(CC) $(CFLAGS) $(INCDIRS) $(LDFLAGS) -o $@ $^

clean:
ifneq (,$(wildcard ./$(OUTPUT)))
	$(RM) $(OUTPUT)
endif

test: $(OUTPUT)
	@php -dextension=./$(OUTPUT) ./test.php

coverage: $(SOURCE)
	$(CC) $(CFLAGS) -fprofile-instr-generate -fcoverage-mapping $(INCDIRS) $(LDFLAGS) -o coverage.so $^
	php -dextension=./coverage.so ./coverage.php
	llvm-profdata-11 merge -sparse default.profraw -o default.profdata
	llvm-cov-11 show ./coverage.so -instr-profile=default.profdata --ignore-filename-regex=php
	$(RM) coverage.so default.profraw default.profdata
	@:

help:
	$(info )
	$(info Environmental variables used and respected:)
	$(info 	PHPCONF     Full executable name for php-config tool if not in PATH)
	$(info 	ARCH        Architecture-specific settings for compiling)
	$(info 	CFLAGS      Compiler-specific flags, defaults in Makefile)
	$(info 	LDFLAGS     Linker-specific flags, defaults in Makefile)
	$(info )
	$(info Build Targets:)
	$(info	all         The default build target - compile $(OUTPUT))
	$(info	test        Compile $(OUTPUT) and run test cases script)
	$(info 	coverage    LLVM-COV based code coverage test)
	$(info	clean       Delete $(OUTPUT) and any build system cruft)
	$(info )
	@:
