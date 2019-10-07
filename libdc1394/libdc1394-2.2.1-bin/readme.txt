Author of this file: Mark Tsuchida, March 2014

What are these files?
=====================

These are the libdc1394 2.2.1 headers and Windows binaries (DLLs and import
libraries). The DLLs were built using MinGW-w64; they include the required
parts of the MinGW-w64 runtime libraries. The import libraries are for use
with Visual C++.


How do I link to these files?
=============================

Simply point Visual Studio to the headers and import libraries:

C/C++ > General > Additional Include Directories:
Should include the include directory.

Linker > General > Additional Library Directories:
Should include the Win32 or x64 directory.

Linker > Input > Additional Library Dependencies:
Should include libdc1394-22.lib.

At runtime, libdc1394-22.dll depends on 1394camera.dll, provided as part of
the CMU 1394Camera package, version 6.4.6. Different versions may or may not
work. Since 1394camera.dll is installed in a system directory (e.g.
C:\Windows\System32) which is always on the DLL search path, there is no need
to include 1394camera.dll with the application (the user needs to install the
entire 1394Camera package to get the drivers anyway).


How were the libraries built?
=============================

These DLLs were built using MinGW-w64 on Arch Linux. This was easier than
setting up a MinGW-x64 build environment on Windows, at least for me
(MinGW-x64 does not provide a single official binary distribution, and also
has to be manually added to an existing MSYS installation). Note that
MinGW-w64, which supports both the 32-bit and 64-bit targets, is a distinct
project from MinGW.org (the original 32-bit-only MinGW).


Setting up the build tools
--------------------------

First, install the required packages.

  # The native compilers and various tools
  sudo pacman -S base-devel

  # The MinGW-w64 cross-compilers (both x86 and x64)
  sudo pacman -S mingw-w64-gcc

  # The MinGW-w64 version of pkg-config
  mkdir -p ~/aur && cd ~/aur
  curl -O https://aur.archlinux.org/packages/mi/mingw-w64-pkg-config/mingw-w64-pkg-config.tar.gz
  tar xzf mingw-w64-pkg-config.tar.gz
  cd mingw-w64-pkg-config
  makepkg -s
  sudo pacman -U mingw-w64-pkg-config-2-3-any.pkg.tar.xz

  # The MinGW-w64 gendef tool
  mkdir -p ~/aur && cd ~/aur
  curl -O https://aur.archlinux.org/packages/mi/mingw-w64-tools/mingw-w64-tools.tar.gz
  tar xzf mingw-w64-tools.tar.gz
  cd mingw-w64-tools-config
  makepkg -s
  sudo pacman -U mingw-w64-tools-3.0.0-1-x86_64.pkg.tar.xz

Note: The mingw-w64-pkg-config package from the AUR should be installed to
prevent the native pkg-config from finding the native libusb-1.0 (which we of
course do not want to link against). Alternatively, uninstall the native
pkg-config. (In these instructions we are not building libdc1394's libusb
support.)


Preparing the required libraries
--------------------------------

To build libdc1394 for x86, we need the following three files:
  C:\Windows\SysWOW64\setupapi.dll (in C:\Windows\System32 on 32-bit Windows)
  C:\Windows\SysWOW64\ws2_32.dll (in C:\Windows\System32 on 32-bit Windows)
  C:\Program Files (x86)\CMU\1394Camera\lib\1394camera.dll (in C:\Program
  Files\CMU on 32-bit Windows)

Rename the files by prefixing them with 'lib', and place them in, e.g.,
~/libdc1394-deps/lib

(TODO: Is this rename still necessary? It is only to please libtool; the
compiler and linker are perfectly happy with the original names.)


To build libdc1394 for x64, we need the following three files:
  C:\Windows\System32\setupapi.dll
  C:\Windows\System32\ws2_32.dll
  C:\Program Files (x86)\CMU\1394Camera\lib64\x64\1394camera.dll

Rename the files by prefixing them with 'lib', and place them in, e.g.,
~/libdc1394-deps/lib64


Note: The C:\Windows\System32 folder is treated specially by Windows: if you
use a 32-bit program to view or copy from it, it will act on the 32-bit
versions of the files that are actually in the C:\Windows\SysWOW64 folder. It
is worth checking that you got the DLLs for the intended architecture by
examining them with e.g. Dependency Walker.


Finally, for both architectures, we need the CMU 1394Camera headers. Copy the
contents of C:\Program Files (x86)\CMU\1394Camera\include to
~/libdc1394-deps/include.


Building libdc1394
------------------

For the x86 version, we need some preparation:
  cd ~/libdc1394-deps/lib
  gendef libsetupapi.dll
  gendef libws2_32.dll
  /usr/i686-w64-mingw32/bin/dlltool -k --output-lib libsetupapi.a --input-def libsetupapi.def
  /usr/i686-w64-mingw32/bin/dlltool -k --output-lib libws2_32.a --input-def libws2_32.def
This does not appear to be necessary for x64.

Now, build libdc1394 for x86:
  DEPS=~/libdc1394-deps
  DEPS_INCLUDEDIR=${DEPS}/include
  DEPS_LIBDIR=${DEPS}/lib
  DEST=/whereever
  MS_ARCH=Win32
  GCC_ARCH=i686-w64-mingw32

  TEMPDIR=/tmp/build-libdc1394-${GCC_ARCH}
  mkdir -p ${TEMPDIR} && cd ${TEMPDIR}
  tar xzf /path/to/libdc1394-2.2.1.tar.gz
  cd libdc1394-2.2.1

  ./configure --host=${GCC_ARCH} --enable-shared --disable-static \
    --disable-sdltest --disable-examples CPPFLAGS="-I${DEPS_INCLUDEDIR}" \
    LDFLAGS="-L${DEPS_LIBDIR}" CFLAGS='-O2 -static-libgcc'
  # Make sure libusb-1.0 is _not_ detected

  make

  # Now, this created dc1394/.libs/libdc1394-22.dll, but libtool fails to
  # honor the -static-libgcc flag, so we rebuild our own DLL. The following
  # command is the same that libtool executed to link the DLL, with
  # -static-libgcc appended, the flags to output an import library removed,
  # and the output filenames tweaked:
  ${GCC_ARCH}-gcc -shared dc1394/.libs/libdc1394-22.dll.def \
    dc1394/.libs/*.o \
    -Wl,--whole-archive dc1394/windows/.libs/libdc1394-windows.a \
    dc1394/vendor/.libs/libdc1394-vendor.a -Wl,--no-whole-archive \
    -l1394camera -lsetupapi -lws2_32 -L${DEPS_LIBDIR} -Wl,--export-all \
    -o libdc1394-22.dll -Wl,--enable-auto-image-base -static-libgcc

  mkdir -p ${DEST}/${MS_ARCH}
  cp libdc1394-22.dll ${DEST}/${MS_ARCH}/
  cp dc1394/.libs/libdc1394-22.dll.def ${DEST}/${MS_ARCH}/

To build for x64, set the following instead and repeat the rest of the
commands:
  DEPS=~/libdc1394-deps
  DEPS_INCLUDEDIR=${DEPS}/include
  DEPS_LIBDIR=${DEPS}/lib64
  DEST=/whereever
  MS_ARCH=x64
  GCC_ARCH=x86_64-w64-mingw32

Finally, copy the libdc1394 headers (all *.h files in dc1394/, preserving the
directory hierarchy) to ${DEST}/include/dc1394.


Creating import libraries for Visual C++
----------------------------------------

Now, go back to Windows. Open the Visual Studio Command Prompt, and go to the
directory copied from ${DEST} above. It should contain the directories Win32,
x64, and include.

  cd Win32
  lib /def:libdc1394-22.dll.def /machine:x86 /name:libdc1394-22.dll /out:libdc1394-22.lib
  cd ..\x64
  lib /def:libdc1394-22.dll.def /machine:x64 /name:libdc1394-22.dll /out:libdc1394-22.lib

(Alternatively, rename the .dll.def files to just .def, and omit the /name:
and /out: options.)

It is also a good idea to check that libdc1394-22.dll depends on
1394camera.dll, setupapi.dll, ws2_32.dll, msvcrt.dll, kernel32.dll, but no
others:
  dumpbin /dependents libdc1394-22.dll

Deviating from the above procedure can result in incorrect dependencies on
"lib1394camera.dll" instead of 1394camera.dll, or on libgcc_s_sjlj-1.dll. Also
check your DLL that is linked to libdc1394-22.lib to make sure that it depends
on libdc1394-22.dll, rather than "libdc1394-22.dll.dll".


Why not build static libraries?
===============================

Building a static libdc1394.a would mean that libdc1394, as well as parts of
the MinGW-w64 runtime libraries, would reside in the same (device adapter) DLL
generated by VC++.

Statically linking VC++-compiled code (i.e. the IIDC device adapter) with
MinGW-compiled code (i.e. libdc1394.a) is dangerous. For example, if
libmingw32.a is included in the linker input, the x64 version of the IIDC
device adapter will crash on load, because Microsoft's CRT init code (which is
compiled into the device adapter DLL even though we are linking to the DLL
CRT) calls atexit(), but libmingw32.a provides its own atexit(). The call gets
linked to libmingw32.a's atexit() and crashes at run time. Fortunately,
libmingw32.a is not (currently) needed by libdc1394 or mmgr_dal_IIDC, but it
is important to note that the whole arrangement is a little fragile and may
break in the future when any of the code (MS CRT, MinGW runtime, libdc1394, or
mmgr_dal_IIDC) changes. The safe method will be to build libdc1394 as a DLL
(dc1394.dll) using MinGW only, and link to the DLL from mmgr_dal_IIDC.

Presumably, a static libdc1394 would work in projects entirely built with
MinGW-w64.


TODO
====

Is it possible to build libdc1394 with libusb-1.0 support on Windows?
