# SPDX-FileCopyrightText: © 2024 Jean-Pierre De Jesus DIAZ <me@jeandudey.tech>
# SPDX-License-Identifier: GPL-3.0-or-later

moddir=$(prefix)/share/guile/site/$(GUILE_EFFECTIVE_VERSION)
godir=$(libdir)/guile/$(GUILE_EFFECTIVE_VERSION)/site-ccache

SUFFIXES = .scm .c .x

libguile_toml_la_SOURCES = toml.c
libguile_toml_la_CFLAGS = $(LIBTOML_CFLAGS) $(GUILE_CFLAGS)
libguile_toml_la_LIBADD = $(LIBTOML_LIBS) $(GUILE_LIBS)
lib_LTLIBRARIES = libguile_toml.la

SOURCES = toml.scm
GOBJECTS = $(SOURCES:%.scm=%.go)

nobase_mod_DATA = $(SOURCES)
nobase_go_DATA = $(GOBJECTS)

BUILT_SOURCES = toml.x
CLEANFILES = $(GOBJECTS) toml.x
GUILE_WARNINGS =                    \
  -Wunbound-variable                \
  -Warity-mismatch                  \
  -Wformat

guile_install_go_files = install-nobase_goDATA
$(guile_install_go_files): install-nobase_modDATA

.c.x:
	$(AM_V_GEN)$(top_builddir)/pre-inst-env guile-snarf -o $@ $< $(libguile_toml_la_CFLAGS)

.scm.go:
	$(AM_V_GEN) $(CROSS_COMPILING_VARIABLE) $(top_builddir)/pre-inst-env \
	$(GUILE_TOOLS) compile --target=$(host) $(GUILE_TARGET) $(GUILE_WARNINGS) -o "$@" "$<"

$(SOURCES): $(lib_LTLIBRARIES)

EXTRA_DIST = $(SOURCES) pre-inst-env.in
