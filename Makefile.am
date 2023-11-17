include $(top_srcdir)/build/modmake.rulesam
MODNAME=mod_extra

mod_LTLIBRARIES = mod_extra.la
mod_extra_la_SOURCES  = mod_extra.c
mod_extra_la_CFLAGS   = $(AM_CFLAGS)
mod_extra_la_LIBADD   = $(switch_builddir)/libfreeswitch.la
mod_extra_la_LDFLAGS  = -avoid-version -module -no-undefined -shared