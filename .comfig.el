(setq compilation-auto-jump-to-first-error t)

;; Point Flycheck to your custom lib directory and prevent it from using system headers
(setq flycheck-clang-args
      '("-Wall"
        "-nostdinc"
        "-nostdlib"
        "-m32"
	"-march x86_64"
        "-I/home/olivia/Documents/Vlip/src/lib"))

;; Optional: Prevent clangd from auto-inserting system headers or suggesting undeclared symbols
(setq lsp-clients-clangd-args
      '("--header-insertion=never"
        "--clang-tidy"
        "--all-scopes-completion=false"
        "--compile-commands-dir=."))

;; Optional: Disable LSP snippets and standard includes
(with-eval-after-load 'lsp-mode
  (setq lsp-enable-snippet nil)
  (setq lsp-clients-clangd-binary "/usr/bin/clangd")) ; Adjust if using a custom clangd

;; Compile command using Projectile root
(defun call/MakeFile-test ()
  (interactive)
  (let ((default-directory (projectile-project-root)))
    (compile "make -j3 run")))

(global-set-key (kbd "C-c m t") 'call/MakeFile-test)
