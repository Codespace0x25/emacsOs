(setq flycheck-clang-language-standard "c++17")
(setq flycheck-clang-args '("-Wall"))
(setq compilation-auto-jump-to-first-error t)


(defun call/MakeFile-test ()
  (interactive
  (let ((default-directory (projectile-project-root)))
    (compile "make -j 3 run"))))


(global-set-key (kbd "C-c m t") 'call/MakeFile-test)
