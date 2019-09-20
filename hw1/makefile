CC = gcc
CFLAGS = -g
IFLAGS = -I. -I/usr/lib/gcc/x86_64-linux-gnu/7/include

getpi: main.o pie.o
	$(CC) -o getpi main.o pie.o

clean:
	rm getpi main.o pie.o
 
depend:
	makedepend ${IFLAGS} main.c pie.c 
# DO NOT DELETE THIS LINE - maketd DEPENDS ON IT

main.o: /usr/include/stdio.h /usr/include/bits/libc-header-start.h
main.o: /usr/include/features.h /usr/include/stdc-predef.h
main.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
main.o: /usr/include/bits/long-double.h /usr/include/gnu/stubs.h
main.o: /usr/lib/gcc/x86_64-linux-gnu/7/include/stddef.h
main.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
main.o: /usr/include/bits/types/__FILE.h /usr/include/bits/types/FILE.h
main.o: /usr/include/bits/libio.h /usr/include/bits/_G_config.h
main.o: /usr/include/bits/types/__mbstate_t.h
main.o: /usr/lib/gcc/x86_64-linux-gnu/7/include/stdarg.h
main.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
main.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
main.o: /usr/include/bits/waitstatus.h /usr/include/bits/floatn.h
main.o: /usr/include/bits/floatn-common.h /usr/include/sys/types.h
main.o: /usr/include/bits/types/clock_t.h /usr/include/bits/types/clockid_t.h
main.o: /usr/include/bits/types/time_t.h /usr/include/bits/types/timer_t.h
main.o: /usr/include/bits/stdint-intn.h /usr/include/endian.h
main.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
main.o: /usr/include/bits/byteswap-16.h /usr/include/bits/uintn-identity.h
main.o: /usr/include/sys/select.h /usr/include/bits/select.h
main.o: /usr/include/bits/types/sigset_t.h
main.o: /usr/include/bits/types/__sigset_t.h
main.o: /usr/include/bits/types/struct_timeval.h
main.o: /usr/include/bits/types/struct_timespec.h
main.o: /usr/include/sys/sysmacros.h /usr/include/bits/sysmacros.h
main.o: /usr/include/bits/pthreadtypes.h
main.o: /usr/include/bits/thread-shared-types.h
main.o: /usr/include/bits/pthreadtypes-arch.h /usr/include/alloca.h
main.o: /usr/include/bits/stdlib-float.h
pie.o: /usr/include/stdlib.h /usr/include/bits/libc-header-start.h
pie.o: /usr/include/features.h /usr/include/stdc-predef.h
pie.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
pie.o: /usr/include/bits/long-double.h /usr/include/gnu/stubs.h
pie.o: /usr/lib/gcc/x86_64-linux-gnu/7/include/stddef.h
pie.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
pie.o: /usr/include/bits/floatn.h /usr/include/bits/floatn-common.h
pie.o: /usr/include/sys/types.h /usr/include/bits/types.h
pie.o: /usr/include/bits/typesizes.h /usr/include/bits/types/clock_t.h
pie.o: /usr/include/bits/types/clockid_t.h /usr/include/bits/types/time_t.h
pie.o: /usr/include/bits/types/timer_t.h /usr/include/bits/stdint-intn.h
pie.o: /usr/include/endian.h /usr/include/bits/endian.h
pie.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
pie.o: /usr/include/bits/uintn-identity.h /usr/include/sys/select.h
pie.o: /usr/include/bits/select.h /usr/include/bits/types/sigset_t.h
pie.o: /usr/include/bits/types/__sigset_t.h
pie.o: /usr/include/bits/types/struct_timeval.h
pie.o: /usr/include/bits/types/struct_timespec.h /usr/include/sys/sysmacros.h
pie.o: /usr/include/bits/sysmacros.h /usr/include/bits/pthreadtypes.h
pie.o: /usr/include/bits/thread-shared-types.h
pie.o: /usr/include/bits/pthreadtypes-arch.h /usr/include/alloca.h
pie.o: /usr/include/bits/stdlib-float.h
