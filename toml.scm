;;; SPDX-FileCopyrightText: © 2024 Jean-Pierre De Jesus DIAZ <me@jeandudey.tech>
;;; SPDX-License-Identifier: GPL-3.0-or-later

(define-module (toml)
  #:export (;; Timestamp accessors.
            timestamp?
            timestamp-ms
            timestamp-sec
            timestamp-min
            timestamp-hour
            timestamp-mday
            timestamp-mon
            timestamp-year
            timestamp-zone

            ;; API from the C extension. 
            parse
            array-in
            key-in
            table-in
            table-key
            table-length
            value-in))

(define (timestamp? obj)
  (and (vector? obj)
       (eq? (vector-length obj) 8)))

(define (timestamp-ms obj) (vector-ref obj 0))
(define (timestamp-sec obj) (vector-ref obj 1))
(define (timestamp-min obj) (vector-ref obj 2))
(define (timestamp-hour obj) (vector-ref obj 3))
(define (timestamp-mday obj) (vector-ref obj 4))
(define (timestamp-mon obj) (vector-ref obj 5))
(define (timestamp-year obj) (vector-ref obj 6))
(define (timestamp-zone obj) (vector-ref obj 7))

(eval-when (expand load eval)
  (load-extension "libguile_toml" "scm_toml_init"))
