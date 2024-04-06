/**
 * SPDX-FileCopyrightText: © 2024 Jean-Pierre De Jesus DIAZ <me@jeandudey.tech>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <libguile.h>
#include <toml.h>

static SCM scm_toml_array_t;
static SCM scm_toml_table_t;

SCM_GLOBAL_SYMBOL (scm_invalid_document, "invalid-document");

SCM_DEFINE (scm_toml_parse, "parse", 2, 0, 0,
           (SCM conf, SCM error_buffer_size),
        "Parse the TOML document in @code{conf}, the error buffer size \n"
        "is specified by @code{error_buffer_size}, in bytes.\n\n"
        "If an error occurs an error with 'invalid-document type will be \n"
        "raised.")
{
  int errbufsz;
  char *errbuf;
  toml_table_t *tab;

  errbufsz = scm_to_int (error_buffer_size);
  errbuf = scm_malloc (errbufsz);

  tab = toml_parse (scm_to_utf8_stringn (conf, NULL), errbuf, errbufsz);
  if (tab == 0)
    scm_error (scm_invalid_document, "parse", errbuf, SCM_BOOL_F,
               SCM_BOOL_F);

  return scm_make_foreign_object_1 (scm_toml_table_t, tab);
}

SCM_DEFINE (scm_toml_array_in, "array-in", 2, 0, 0,
           (SCM table, SCM key),
        "Return an array using the key or @code{#f} if it doesn't exist.")
{
  toml_array_t *arr;

  scm_assert_foreign_object_type (scm_toml_table_t, table);
  arr = toml_array_in (scm_foreign_object_ref (table, 0),
                       scm_to_utf8_stringn (key, NULL));
  if (arr == 0)
    return SCM_BOOL_F;

  return scm_make_foreign_object_1 (scm_toml_array_t, arr);
}

SCM_DEFINE (scm_toml_key_in, "key-in", 2, 0, 0,
           (SCM table, SCM index),
        "Return the key at @code{index} or @code{#f} if out of range.")
{
  const char *key;
  
  scm_assert_foreign_object_type (scm_toml_table_t, table);
  key = toml_key_in (scm_foreign_object_ref (table, 0), scm_to_int (index));
  return (key == 0) ? SCM_BOOL_F : scm_from_utf8_string (key);
}

SCM_DEFINE (scm_toml_table_in, "table-in", 2, 0, 0,
           (SCM table, SCM key),
        "Return a subtable using the key or @code{#f} if it doesn't exist.")
{
  toml_table_t *tab;

  scm_assert_foreign_object_type (scm_toml_table_t, table);
  tab = toml_table_in (scm_foreign_object_ref (table, 0),
                       scm_to_utf8_stringn (key, NULL));
  if (tab == 0)
    return SCM_BOOL_F;

  return scm_make_foreign_object_1 (scm_toml_table_t, tab);
}

SCM_DEFINE (scm_toml_table_key, "table-key", 1, 0, 0,
           (SCM table),
        "Return the key of a table or return @code{#f} if the table does \n"
        "not have a key.")
{
  const char *key;

  scm_assert_foreign_object_type (scm_toml_table_t, table);
  key = toml_table_key (scm_foreign_object_ref (table, 0));
  scm_remember_upto_here_1 (table);
  return (key == NULL) ? SCM_BOOL_F : scm_from_utf8_string (key);
}

SCM_DEFINE (scm_toml_table_length, "table-length", 1, 0, 0,
           (SCM table),
        "Return the number of sub-tables in a table.")
{
  toml_table_t *tab;
  int length;

  scm_assert_foreign_object_type (scm_toml_table_t, table);
  length = toml_table_ntab (scm_foreign_object_ref (table, 0));
  scm_remember_upto_here_1 (table);
  return scm_from_int (length);
}

SCM_DEFINE (scm_toml_value_in, "value-in", 2, 0, 0,
           (SCM table, SCM key),
        "Return the value of @code{key} in the @code{table} or @code{#f}.")
{
  toml_table_t *arr;
  toml_datum_t datum;
  char *k;
  SCM result;

  scm_assert_foreign_object_type (scm_toml_table_t, table);
  arr = scm_foreign_object_ref (table, 0);
  k = scm_to_utf8_stringn (key, NULL);

  datum = toml_string_in (arr, k);
  if (datum.ok) {
    result = scm_from_utf8_string (datum.u.s);
    free (datum.u.s);
    return result;
  }

  datum = toml_bool_in (arr, k);
  if (datum.ok)
    return datum.u.b ? SCM_BOOL_T : SCM_BOOL_F;

  datum = toml_int_in (arr, k);
  if (datum.ok)
    return scm_from_int64 (datum.u.i);

  datum = toml_double_in (arr, k);
  if (datum.ok)
    return scm_from_double (datum.u.d);

  datum = toml_timestamp_in (arr, k);
  if (datum.ok) {
      result = scm_c_make_vector (8, SCM_BOOL_F);
      if (datum.u.ts->millisec)
        SCM_SIMPLE_VECTOR_SET (result,0, scm_from_int (*datum.u.ts->millisec));
      if (datum.u.ts->second)
        SCM_SIMPLE_VECTOR_SET (result,1, scm_from_int (*datum.u.ts->second));
      if (datum.u.ts->minute)
        SCM_SIMPLE_VECTOR_SET (result,2, scm_from_int (*datum.u.ts->minute));
      if (datum.u.ts->hour)
        SCM_SIMPLE_VECTOR_SET (result,3, scm_from_int (*datum.u.ts->hour));
      if (datum.u.ts->day)
        SCM_SIMPLE_VECTOR_SET (result,4, scm_from_int (*datum.u.ts->day));
      if (datum.u.ts->month)
        SCM_SIMPLE_VECTOR_SET (result,5, scm_from_int (*datum.u.ts->month));
      if (datum.u.ts->year)
        SCM_SIMPLE_VECTOR_SET (result,6, scm_from_int (*datum.u.ts->year));
      if (datum.u.ts->z)
        SCM_SIMPLE_VECTOR_SET (result,7, scm_from_utf8_string (datum.u.ts->z));
      free (datum.u.ts);
      return result;
  }

  scm_remember_upto_here_1 (table);

  return SCM_BOOL_F;
}

static void
scm_toml_table_finalize (SCM table)
{
  toml_table_t *tab;

  tab = scm_foreign_object_ref (table, 0);
  if (tab)
    toml_free (tab);
}

void
scm_toml_init ()
{
  toml_set_memutil (scm_malloc, free);

  scm_toml_array_t =
    scm_make_foreign_object_type (scm_from_utf8_symbol ("toml_array_t"),
                                  scm_list_1 (scm_from_utf8_symbol ("pointer")),
                                  NULL);

  scm_toml_table_t =
    scm_make_foreign_object_type (scm_from_utf8_symbol ("toml_table_t"),
                                  scm_list_1 (scm_from_utf8_symbol ("pointer")),
                                  scm_toml_table_finalize);

#include "toml.x"
}
