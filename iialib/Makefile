

WCLINE = g++ -c -g3 -I./include -DWINDOWS -DIIADYNAMIC

obj/win32/Socket.o: include/socket/Socket.hpp include/socket/SockAddr.hpp include/socket/SockAddrIn.hpp include/socket/SockAddrIn6.hpp src/socket/Socket.cpp
	$(WCLINE) src/socket/Socket.cpp -o .obj/win32/Socket.o

obj/win32/HostEnt.o: include/socket/HostEnt.hpp include/socket/SockAddrIn.hpp include/socket/SockAddrIn6.hpp src/socket/HostEnt.cpp
	$(WCLINE) src/socket/HostEnt.cpp -o .obj/win32/HostEnt.o

obj/win32/SockAddr.o: include/socket/SockAddr.hpp src/socket/SockAddr.cpp
	$(WCLINE) src/socket/SockAddr.cpp -o .obj/win32/SockAddr.o

obj/win32/SockAddrIn.o: include/socket/SockAddrIn.hpp src/socket/SockAddrIn.cpp include/socket/SockAddr.hpp
	$(WCLINE) src/socket/SockAddrIn.cpp -o .obj/win32/SockAddrIn.o

obj/win32/SockAddrIn6.o: include/socket/SockAddrIn6.hpp src/socket/SockAddrIn6.cpp include/socket/SockAddr.hpp
	$(WCLINE) src/socket/SockAddrIn6.cpp -o .obj/win32/SockAddrIn6.o

obj/win32/Thread.o: include/threads/Thread.hpp src/threads/Thread.cpp
	$(WCLINE) src/threads/Thread.cpp -o .obj/win32/Thread.o

obj/win32/Mutex.o: include/threads/Mutex.hpp src/threads/Mutex.cpp
	$(WCLINE) src/threads/Mutex.cpp -o .obj/win32/Mutex.o

obj/win32/Exception.o: include/exceptions/Exception.hpp src/exceptions/Exception.cpp
	$(WCLINE) src/exceptions/Exception.cpp -o .obj/win32/Exception.o

libiia.a: iia.dll

iia.dll: .obj/win32/Socket.o .obj/win32/SockAddr.o .obj/win32/SockAddrIn.o .obj/win32/SockAddrIn6.o .obj/win32/HostEnt.o .obj/win32/Mutex.o .obj/win32/Thread.o .obj/win32/Exception.o
	g++ -g3 -shared -Wl,--out-implib=libiia.dll -Lc:/compilers/minw34/pthreads/ .obj/win32/Exception.o .obj/win32/Socket.o .obj/win32/SockAddr.o .obj/win32/SockAddrIn.o .obj/win32/SockAddrIn6.o .obj/win32/HostEnt.o .obj/win32/Mutex.o .obj/win32/Thread.o -o iia.dll -lpthreadgc1 -lwsock32

all: iia.dll
