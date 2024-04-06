dnl SPDX-FileCopyrightText: © 2024 Jean-Pierre De Jesus DIAZ <me@jeandudey.tech>
dnl SPDX-License-Identifier: GPL-3.0-or-later

AC_INIT([guile-toml], [0.1.0], [me@jeandudey.tech], [guile-toml],
        [https://github.com/jeandudey/guile-tomlc99])

AM_INIT_AUTOMAKE([-Wall foreign color-tests])

dnl Enable silent rules by default.
AM_SILENT_RULES([yes])

AM_PROG_AR
AC_PROG_CC

LT_INIT([disable-static])

PKG_CHECK_MODULES([GUILE], [guile-3.0])
PKG_CHECK_MODULES([LIBTOML], [libtoml])

GUILE_PKG([3.0])
GUILE_PROGS
GUILE_SITE_DIR

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([pre-inst-env], [chmod +x pre-inst-env])

AC_OUTPUT
