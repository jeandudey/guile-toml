;;; SPDX-FileCopyrightText: © 2024 Jean-Pierre De Jesus DIAZ <me@jeandudey.tech>
;;; SPDX-License-Identifier: GPL-3.0-or-later

(use-modules ((toml) #:prefix toml:)
             (ice-9 format))

(define %example-toml
  "
root-value = true
[package]
name = \"wonderful\"
description = \"useful package\"
version = \"0.1.0\"
timestamp = 2022-01-12")

(define* (print-table table
                      #:optional
                      (port (current-output-port)))
  (format port "{")
  (do ((i 0 (1+ i)))
      ((not (toml:key-in table i)))

    (let* ((key      (toml:key-in table i))
           (array    (toml:array-in table key))
           (subtable (toml:table-in table key))
           (value    (toml:value-in table key)))
      (when (> i 0)
        (format port ","))
      (format port "~s:" key)
      (cond
        (subtable (print-table subtable port))
        ((boolean? value) (print-boolean value port))
        ((string? value) (print-string value port))
        ((toml:timestamp? value) (print-timestamp value port)))))
  (format port "}"))

(define (print-boolean value port)
  (format port "~:[false~;true~]" value))

(define (print-string value port)
  (display "\"" port)
  (display value port)
  (display "\"" port))

(define (print-timestamp value port)
  (cond
    ((and (toml:timestamp-year value)
          (toml:timestamp-hour value))
     (format port "{~s:~s,"
             "type"
             (if (toml:timestamp-zone value)
                 "datetime-local"
                 "datetime"))
     (format port "~s:" "value")
     (format port "\"~4,'0d-~2,'0d-~2,'0dT~2,'0d:~2,'0d:~2,'0d}"
             (toml:timestamp-year value)
             (toml:timestamp-mon value)
             (toml:timestamp-mday value)
             (toml:timestamp-hour value)
             (toml:timestamp-minute value)
             (toml:timestamp-sec value)))
    ((toml:timestamp-year value)
     (format port "{~s:~s," "type" "date-local")
     (format port "~s:" "value")
     (format port "\"~4,'0d-~2,'0d-~2,'0d\""
             (toml:timestamp-year value)
             (toml:timestamp-mon value)
             (toml:timestamp-mday value)))
    ((toml:timestamp-hour value)
     (format port "{~s:~s" "type" "time-local")
     (format port "~s:" "value")
     (format port "\"~2,'0d:~2,'0d:~2,'0d\""
             (toml:timestamp-hour value)
             (toml:timestamp-minute value)
             (toml:timestamp-sec value)))))

(let ((root (toml:parse %example-toml 200)))
  (print-table root)
  (newline))
