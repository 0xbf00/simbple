;;;;;; Sandbox Profile Language version 1

;;; Directives

;; Wrap standard I/O procedures to notify when additional paths are opened.
(let ((old-load load)
      (old-open-input-file open-input-file)
      (old-open-output-file open-output-file))
  (set! load
        (lambda (path)
          (%notify-file-open path 'read #t)
          (old-load path)))
  (set! open-input-file
        (lambda (path)
          (let ((port (old-open-input-file path)))
            (%notify-file-open path 'read (input-port? port))
            port)))
  (set! open-output-file
        (lambda (path)
          (let ((port (old-open-output-file path)))
            (%notify-file-open path 'write (output-port? port))
            port))))

;; Changed only slightly in 10.15 Catalina.
(if string>=? *platform-version* "10.15")
  ;; Definition from 10.15
  ;; Define the import directive.
  (define (import path)
    (define import-dirs
      (if (param "IMPORT_DIR")
          (list (param "IMPORT_DIR"))
          (list "/System/Library/Sandbox/Profiles"
                "/usr/share/sandbox")))
    (if (or (= 0 (string-length path))
            (eqv? #\/ (string-ref path 0)))
        ;; Absolute path, load it directly.
        (load path)
        ;; Relative path, search import-dirs.
        (let search ((dirs import-dirs))
          (if (null? dirs)
              (throw (string-append "unable to open " path)))
          ;; Attempt to open the path relative to the next dir in the list.
          (let* ((try (string-append (car dirs)
                                     "/"
                                     path))
                 (tried (open-input-file try)))
            ;; Load the file if it could be opened, otherwise keep searching.
            (if tried
                (begin (close-input-port tried)
                       (load try))
                (search (cdr dirs)))))))  
  ;; Definition from 10.14 and below.
  ;; Define the import directive.
  (define (import path)
    (define import-dirs
      (if (param "IMPORT_DIR")
          (list (param "IMPORT_DIR"))
          (list "/System/Library/Sandbox/Profiles"
                "/usr/share/sandbox")))
    (if (or (= 0 (string-length path))
            (eqv? #\/ (string-ref path 0)))
        ;; Absolute path, load it directly.
        (load path)
        ;; Relative path, search import-dirs.
        (let search ((dirs import-dirs))
          (if (null? dirs)
              (error (string-append "unable to open " path)))
          ;; Attempt to open the path relative to the next dir in the list.
          (let* ((try (string-append (car dirs)
                                     "/"
                                     path))
                 (tried (open-input-file try)))
            ;; Load the file if it could be opened, otherwise keep searching.
            (if tried
                (begin (close-input-port tried)
                       (load try))
                (search (cdr dirs)))))))

;; Define the trace directive.
(define *trace* #f)
(define (trace path)
  (cond
    ((not (string? path))
       (error "argument to trace must be a string"))
     (*trace*
       (error "only one trace path may be specified"))
     (else
       (set! *trace* path))))

;;; Utilities

;; Introduced in 10.15 Catalina
(if (string>=? *platform-version* "10.15")
  ;;;
  ;;; Add list of operations to SBPL actions.
  ;;; The first argument is a list of operations.  Any actions emitted as a
  ;;; result of evaluating the additional arguments are modified to apply to
  ;;; all of the listed operations in addition to any operations specified by
  ;;; the rules themselves.
  ;;;
  (macro (with-operations form)
    (let* ((ps (cdr form))
           (extra-operations (car ps))
           (rules (cdr ps)))
      `(letrec
         ((inject-operations
            (lambda args
              (append ,extra-operations args)))
          (orig-allow allow)
          (orig-deny deny)
          (wrapper
            (lambda (action)
              (lambda args (apply action (apply inject-operations args))))))
         (set! allow (wrapper orig-allow))
         (set! deny (wrapper orig-deny))
         ,@rules
         (set! deny orig-deny)
         (set! allow orig-allow))))
  ())

;;;
;;; Apply a SBPL filter to actions.
;;; The first argument is the filter to apply.  Any actions emitted as a result
;;; of evaluating the additional arguments are modified to be contingent on
;;; the specified filter.
;;;
(macro (with-filter form)
  (let* ((ps (cdr form))
         (extra-filter (car ps))
         (rules (cdr ps)))
    `(letrec
       ((inject-filter
          (lambda args
            (let* ((collected (partition sbpl-filter? args))
                   (filters (car collected))
                   (non-filters (cadr collected)))
              (if (null? filters)
                  (cons ,extra-filter non-filters)
                  ;; The order of the filters shouldn't matter, but we put the extra filter last
                  ;; since we frequently call this function with require-entitlement in the extra
                  ;; filter and that is still fairly slow (<rdar://problem/11578372>).
                  (cons (require-all (apply require-any filters) ,extra-filter) non-filters)))))
        (orig-allow allow)
        (orig-deny deny)
        (wrapper
          (lambda (action)
            (lambda args (apply action (apply inject-filter args))))))
       (set! allow (wrapper orig-allow))
       (set! deny (wrapper orig-deny))
       ,@rules
       (set! deny orig-deny)
       (set! allow orig-allow))))

;;;
;;; Apply a SBPL modifier to actions.
;;; The first argument is the modifier to apply.  Any actions emitted as a
;;; result of evaluating the additional arguments are modified to include
;;; the specified modifier.
;;;
(macro (with-modifier form)
  (let* ((ps (cdr form))
         (extra-modifier (car ps))
         (rules (cdr ps)))
    `(letrec
       ((inject-modifier
          (lambda args
	    (cons ,extra-modifier args)))
        (orig-allow allow)
        (orig-deny deny)
        (wrapper
          (lambda (action)
            (lambda args (apply action (apply inject-modifier args))))))
       (set! allow (wrapper orig-allow))
       (set! deny (wrapper orig-deny))
       ,@rules
       (set! deny orig-deny)
       (set! allow orig-allow))))


;; Introduced in 10.15 Catalina
(if (string>=? *platform-version* "10.15")
  ;;;
  ;;; Return filter matching all of the ancestor directories of the given path(s).
  ;;;
  (define (path-ancestors . paths)
     (letrec ((find-all-pos-of-char
                (lambda (str char pos)
                  (cond ((>= pos (string-length str)) '())
                        ((char=? char (string-ref str pos)) (cons pos (find-all-pos-of-char str char (+ pos 1))))
                        (else (find-all-pos-of-char str char (+ pos 1))))))
              (start-with-slash?
                (lambda (s)
                  (eqv? (string-ref s 0) #\/)))
              (start-with-variable?
                (lambda (s)
                  (equal? (substring s 0 2) "${")))
              (end-with-slash?
                (lambda (s)
                  (eqv? (string-ref s (- (string-length s) 1)) #\/)))
              (ensure-trailing-slash
                (lambda (path)
                  (if (end-with-slash? path) path (string-append path "/"))))
              (dirhierarchy-list
                (lambda (short long)
                  (map
                    (lambda (pos)
                      (cond ((= pos 0) "/")
                            (else (substring long 0 pos))))
                    (find-all-pos-of-char (ensure-trailing-slash long) #\/ (- (string-length short) 1)))))
              (drop-last
                (lambda (l)
                  (reverse (cdr (reverse l)))))
              (parentdirs-list
                (lambda (p) (drop-last (dirhierarchy-list "/" p))))
              (ancestor-filter
                (lambda (p)
                  (if (or (start-with-slash? p)
                          (start-with-variable? p))
                    (apply require-any (map literal (parentdirs-list p)))
                  ; else
                    (error "path must start with a /")))))
       (apply require-any (map ancestor-filter paths))))
  ())

;; The %finalize function is called after a profile has been evaluated.
(set! %finalize (lambda ()))

(define (entitlement-value arg)
  (cond ((string? arg) (%entitlement-string arg))
        ((boolean? arg) (%entitlement-boolean arg))
        (else (error "entitlement-value argument must be string or boolean"))))

(define (entitlement-value-regex arg)
  (cond ((string? arg) (%entitlement-string-regex arg))
        (else (error "entitlement-value-regex argument must be string"))))

(define (require-entitlement entitlement-name . args)
  (let ((numargs (length args)))
    (cond ((zero? numargs) (require-entitlement entitlement-name (%entitlement-boolean #t)))
          ((= 1 numargs) (require-all (%entitlement-load entitlement-name) (car args)))
          (else (error "too many arguments to require-entitlement")))))

;; Introduced in 10.15 Catalina
(if (string>=? *platform-version* "10.15")
  ;; Helper to define static storage classes in terms of path filters.
  ;; Static storage class mappings are only supported in the platform sandbox profile.
  ;; Example usage:
  ;;    (define-storage-class "foo" (subpath "/foo/bar" "/foo/baz"))
  ;; It is an error to define multiple storage classes with the same name.
  ;; The usual SBPL precedence rules apply: if filters for multiple classes overlap,
  ;; later class definitions take precedence of earlier definitions.
  (begin
  (define %storage-class-names ())
  (define (define-storage-class class-name filter)
    (if (member class-name %storage-class-names)
      (error (string-append "storage class \"" class-name "\" multiply defined") class-name)
    ; else
      (begin
        (set! %storage-class-names (cons class-name %storage-class-names))
        (allow (with assign-storage-class class-name) storage-class-map filter)))))
  ())

;; Helper function for composing fsctl / ioctl commands.
(define (_IO g n)
   (+ n (* 256 (char->integer (car (string->list g))))))

;; Removed / moved in 10.15 Catalina
(if (string<? *platform-version* "10.15")
  (define (process-is-plugin)
    (process-attribute is-plugin))
  ())

;; Removed / moved in 10.15 Catalina
(if (string<? *platform-version* "10.15")
  (define (process-is-installer)
    (process-attribute is-installer))
  ())

;;; Deprecated Modifiers

(define no-callout
  (list 'deprecated
        (lambda args
          (if (= 0 (length args))         ; no arguments
            (disable-callouts)            ; superseded by sandbox option
            (error "unexpected argument")))))
(define partial-symbolication
  (list 'deprecated
        (lambda args
          (if (= 0 (length args))         ; no arguments
            (disable-full-symbolication)  ; superseded by sandbox option
            (error "unexpected argument")))))

;; Introduced in 10.15 Catalina
(if (string>=? *platform-version* "10.15")
  ;;; Alias Modifiers
  (define termination
    (list 'deprecated
          (lambda args
            (if (= 0 (length args))         ; no arguments
              (send-signal SIGKILL)         ; (with termination) ==> (with send-signal SIGKILL)
              (error "unexpected argument")))))
  ())

;;; Aliases

(macro (debug args))
(define literal path)
(define prefix path-prefix)
(define regex path-regex)
(define mount-relative-literal mount-relative-path)
(define mount-relative-regex mount-relative-path-regex)
(define getenv param)
(define file-fsctl system-fsctl)
(define ipc-posix-sem ipc-posix-sem*)
(define ipc-posix-shm ipc-posix-shm*)
(define sysctl-write sysctl*)
(define system-misc system*)
(define time-set system-set-time)
(define from local)
(define to remote)
(define no-profile no-sandbox)
(define no-log no-report)
(define granted-extensions extension)
;; Changed in 10.15 Catalina
(if (string>=? *platform-version* "10.15")
  (define (executable-bundle) (extension *sandbox-executable-bundle*))
  ())
(define (container) (extension *ios-sandbox-container*))
(if (string<? *platform-version* "10.15")
  (define (executable-bundle) (extension *ios-sandbox-executable*))
  ())
(define (application-group) (extension *ios-sandbox-application-group*))
(define (system-container) (extension *ios-sandbox-system-container*))
(define (system-group) (extension *ios-sandbox-system-group*))
(define (eliminate-duplicate-rules))
(define (record path))
(define file-issue-extension* file-issue-extension)
(define file-issue-extension-read file-issue-extension)
(define file-issue-extension-write file-issue-extension)
(define file-unlink file-write-unlink)
(define mach-extension extension)
(define (tty) (vnode-type TTY))
(define file-write-mount file-mount)
(define file-write-unmount file-unmount)
(define file-write-umount file-unmount)
(define process-exec process-exec*)
(define rootless-modifier rootless)
(define (debug-mode) (system-attribute sandbox-debug-mode))
(define iokit-user-client-class iokit-registry-entry-class)
(define iokit-user-client-class-regex iokit-registry-entry-class-regex)
(define managed-preference* managed-preference-read)
;; Added in 10.15 Catalina
(if (string>=? *platform-version* "10.15")
  (begin
    (define (process-is-plugin) (process-attribute is-plugin))
    (define (process-is-installer) (process-attribute is-installer))
    (define (datavault-file-filter) (file-attribute datavault))
    (define (rootless-file-filter) (file-attribute sip-protected))
    (define ipc-posix-shm-read* ipc-posix-shm-read-data)
    (define ipc-posix-shm-read-metadata 'no-op)
    (define mach-per-user-lookup 'no-op)
    (define system-chud 'no-op)
    (define qtn-download 'no-op))
  ())

;;; Support for old syntax for unix domain sockets
;;; e.g. (allow network-outbound
;;;             (remote unix-socket (path-literal "/some/path")))
;;;
;;; These should now be properly written as:
;;; (allow network-outbound
;;;        (literal "/some/path"))
(define unix 'unix)
(define unix-socket unix)
(define path-literal path)