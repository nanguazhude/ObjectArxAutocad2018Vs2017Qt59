;;; DimFillResize.lsp
;;;
;;; Michael Partenheimer                                     12 March 2012
;;;
;;;
;;; Change dimension "Fill Color" size from 1.25 text
;;; height to 1.12 text height. 
;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


(defun c:DFR (/ obj Fsize test1 __________fsn2)
   (if (= __________fsn nil)
      (setq __________fsn 1.005 )
      (progn
         (setq __________fsn2 __________fsn)
         (setq __________fsn 1.005 )
         (if (= __________fsn nil )(
            setq __________fsn __________fsn2)
         )
      );end progn
   );end if
   (if (= __________fsn nil)
      (setq __________fsn 1.005 ) 
   )
   (setq obj nil)
   (setq obj (nentsel "\nPick the text portion of a dimension: "))

;If pick is missed early
   (while (= obj nil)
      (setq obj (nentsel "\nTry again Deadeye. You missed the dimension text: "))
   );end while

;If Non-Text Entity is picked
   (setq test1 (cdr (assoc 0 (entget (car obj)))))
   (while (/= test1 "MTEXT")
      (setq obj (nentsel "\nTry again. Pick a dimension text this time:"))
;If pick is missed late
         (while (= obj nil)
            (setq obj (nentsel "\nTry again. You picked air that time: "))
         );end while
      (setq test1 (cdr (assoc 0 (entget (car obj)))))
   );end while

   (setq obj (entget (car obj)))
   (setq Fsize (assoc 45 obj))
   (setq obj (subst (cons 45 __________fsn) Fsize obj))
   (entmod obj)
   (command "REGEN")
   (princ)
)




