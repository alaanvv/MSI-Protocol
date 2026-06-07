gcc src/main.c && ./a.out && rm ./a.out
gcc src/main.c -DDEBUG_MODE=1 -DCACHE_VIEW_MODE=0 && ./a.out > out && rm ./a.out
