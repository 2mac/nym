ACLOCAL_AMFLAGS = -I m4 --install
dist_doc_DATA = README

noinst_LIBRARIES = libnym.a
bin_PROGRAMS = nym

libnym_a_SOURCES = nym.c
libnym_a_CFLAGS = -Wall -Wextra -Wunreachable-code -ftrapv -std=c89

nym_SOURCES = main.c
nym_CFLAGS = -Wall -Wextra -Wunreachable-code -ftrapv -std=c89
nym_LDADD = libnym.a

AM_CFLAGS = $(DEPS_CFLAGS)
AM_LIBS = $(DEPS_LIBS)
