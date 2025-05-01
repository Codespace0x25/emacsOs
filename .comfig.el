(setq flycheck-clang-language-standard "c++17")
(setq flycheck-clang-includes '("~/Documents/call/src/"))
(setq flycheck-clang-args '("-Wall" "-std=c++17"))
(setq compilation-auto-jump-to-first-error t)


(defun call/MakeFile-test ()
  (interactive
  (let ((default-directory (projectile-project-root)))
    (compile "make -j 3 test"))))

(defun call/MakeFile-comp ()
  (interactive)
  (let ((default-directory (projectile-project-root)))
    (compile "make -j 3 all")))

(defun call/MakeFile-dry-test ()
  (interactive
  (let ((default-directory (projectile-project-root)))
    (compile "make -j 3 clean test"))))

(defun call/MakeFile-dry-comp ()
  (interactive)
  (let ((default-directory (projectile-project-root)))
    (compile "make -j 3 all")))



(global-set-key (kbd "C-c m t") 'call/MakeFile-test)
(global-set-key (kbd "C-c m c") 'call/MakeFile-dry-test)
