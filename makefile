obj_files_static := tempfiles/qw_ulong_static.o
obj_files_shared := tempfiles/qw_ulong_shared.o
src_files := src/qw_ulong.cpp

default: static shared

static: libqw_bigint.a
shared: libqw_bigint.so

libqw_bigint.a: tempfiles $(obj_files_static)
	ar rcs $@ $(obj_files_static)

tempfiles/qw_ulong_static.o: src/qw_ulong.cpp
	g++ -mbmi2 -c $? -o $@

libqw_bigint.so: tempfiles $(obj_files_shared)
	gcc -shared $(obj_files_shared) -o $@

tempfiles/qw_ulong_shared.o: src/qw_ulong.cpp
	g++ -mbmi2 -fPIC -c $? -o $@

tempfiles:
	mkdir tempfiles -p

fresh: clear default

clear:
	rm -rf tempfiles
	rm -f libqw_bigint.a
	rm -f libqw_bigint.so
	rm -f testing

#below here requires a testing.cpp file not tracked by git
testing: tempfiles tempfiles/testing.o libqw_bigint.a
	g++ tempfiles/testing.o -L. -l:libqw_bigint.a -o testing

tempfiles/testing.o: testing.cpp
	g++ -c $? -o $@

run: testing
	./testing

frun: clear run #fresh run
