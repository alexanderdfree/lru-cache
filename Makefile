CC=gcc
CFLAGS=-Wall -g 
FILES = kv_cache.o kv_cache_test.c .kv_cache_util.o
LDFLAGS=-Wl,--allow-multiple-definition

all: kv_cache_test

kv_cache.o: kv_cache.c
	@echo "Building kv_cache.c"
	@$(CC) $(CFLAGS) -c $< -o $@

kv_cache_test: $(FILES)
	@echo "Building kv_cache_test"
	@$(CC) $(CFLAGS) -Wconversion   $(FILES) -o $@ $(LDFLAGS)

# kv_cache_test: kv_cache_test.o
# 	@echo "T"
# 	@$(CC) $(LDFLAGS) -o $@


clean:
	@echo "Cleaning/resetting build files"
	@$(RM) kv_cache_test kv_cache.o
	@$(RM) tests/*_diff*.txt tests/*_actual*.txt
