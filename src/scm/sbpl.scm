;;;;;; Sandbox Profile Language stub

;;; This stub is loaded before the sandbox profile is evaluated.  When version
;;; is called, the SBPL prelude and the appropriate SBPL version library are
;;; loaded.  Combined with functions defined in the scheme interpreter by
;;; sbpl_parser.c, these implement the profile language.

(define (version n)
  (case n
    (( 1) (%version-1))
    (else (error "unsupported version" n)))
  (set! version
        (lambda (o)
          (if (not (= n o))
              (error "only one version may be specified")))))

(define (%finalize)
  (error "no version specified"))