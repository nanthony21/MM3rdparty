lib_LTLIBRARIES=libfreeimageplus.la

include_HEADERS=FreeImagePlus.h

libfreeimageplus_la_SOURCES = src/fipImage.cpp src/fipMemoryIO.cpp src/fipMetadataFind.cpp src/fipMultiPage.cpp src/fipTag.cpp src/fipWinImage.cpp src/FreeImagePlus.cpp ./FreeImagePlus.h 

libfreeimageplus_la_LIBADD=$(top_builddir)/Source/libfreeimage.la 

noinst_PROGRAMS=fipTest

fipTest_SOURCES=test/fipTest.cpp test/fipTestMPage.cpp test/fipTestMPageMemory.cpp test/fipTestMPageStream.cpp test/fipTestMemIO.cpp test/fipTest.h

fipTest_LDADD=$(top_builddir)/Wrapper/FreeImagePlus/libfreeimageplus.la $(top_builddir)/Source/libfreeimage.la

AM_CPPFLAGS = -I$(top_srcdir)/. -I$(top_srcdir)/Source/

AM_CFLAGS=-fexceptions -fvisibility=hidden -DNO_LCMS

AM_CXXFLAGS=-fexceptions -fvisibility=hidden -Wno-ctor-dtor-privacy


