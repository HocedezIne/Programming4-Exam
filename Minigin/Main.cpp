��# i n c l u d e   < S D L . h > 
 
 
 
 # i f   _ D E B U G 
 
 / /   R e S h a r p e r   d i s a b l e   o n c e   C p p U n u s e d I n c l u d e D i r e c t i v e 
 
 # i f   _ _ h a s _ i n c l u d e ( < v l d . h > ) 
 
 # i n c l u d e   < v l d . h > 
 
 # e n d i f 
 
 # e n d i f 
 
 
 
 # i n c l u d e   " M i n i g i n . h " 
 
 # i n c l u d e   " S c e n e M a n a g e r . h " 
 
 # i n c l u d e   " R e s o u r c e M a n a g e r . h " 
 
 # i n c l u d e   " G a m e O b j e c t . h " 
 
 # i n c l u d e   " S c e n e . h " 
 
 
 
 # i n c l u d e   " T r a n s f o r m C o m p o n e n t . h " 
 
 # i n c l u d e   " T e x t u r e C o m p o n e n t . h " 
 
 # i n c l u d e   " T e x t C o m p o n e n t . h " 
 
 # i n c l u d e   " F P S C o m p o n e n t . h " 
 
 
 
 v o i d   l o a d ( ) 
 
 { 
 
 	 a u t o &   s c e n e   =   e n g i n e : : S c e n e M a n a g e r : : G e t I n s t a n c e ( ) . C r e a t e S c e n e ( " D e m o " ) ; 
 
 
 
 	 a u t o   g o   =   s t d : : m a k e _ u n i q u e < e n g i n e : : G a m e O b j e c t > ( ) ; 
 
 	 g o - > A d d C o m p o n e n t < e n g i n e : : T e x t u r e C o m p o n e n t > ( s t d : : m a k e _ u n i q u e < e n g i n e : : T e x t u r e C o m p o n e n t > ( g o . g e t ( ) ,   " b a c k g r o u n d . t g a " ) ) ; 
 
 	 s c e n e . A d d ( s t d : : m o v e ( g o ) ) ; 
 
 
 
 	 g o   =   s t d : : m a k e _ u n i q u e < e n g i n e : : G a m e O b j e c t > ( ) ; 
 
 	 g o - > A d d C o m p o n e n t < e n g i n e : : T r a n s f o r m C o m p o n e n t > ( s t d : : m a k e _ u n i q u e < e n g i n e : : T r a n s f o r m C o m p o n e n t > ( g o . g e t ( ) ,   g l m : : v e c 3 {   2 1 6 . f ,   1 8 0 . f ,   0 . f   } ) ) ; 
 
 	 g o - > A d d C o m p o n e n t < e n g i n e : : F P S C o m p o n e n t > ( s t d : : m a k e _ u n i q u e < e n g i n e : : F P S C o m p o n e n t > ( g o . g e t ( ) ) ) ; 
 
 	 s c e n e . A d d ( s t d : : m o v e ( g o ) ) ; 
 
 
 
 	 a u t o   f o n t   =   e n g i n e : : R e s o u r c e M a n a g e r : : G e t I n s t a n c e ( ) . L o a d F o n t ( " L i n g u a . o t f " ,   3 6 ) ; 
 
 	 g o   =   s t d : : m a k e _ u n i q u e < e n g i n e : : G a m e O b j e c t > ( ) ; 
 
 	 g o - > A d d C o m p o n e n t < e n g i n e : : T r a n s f o r m C o m p o n e n t > ( s t d : : m a k e _ u n i q u e < e n g i n e : : T r a n s f o r m C o m p o n e n t > ( g o . g e t ( ) ,   g l m : : v e c 3 {   8 0 . f ,   2 0 . f ,   0 . f   } ) ) ; 
 
 	 g o - > A d d C o m p o n e n t < e n g i n e : : T e x t C o m p o n e n t > ( s t d : : m a k e _ u n i q u e < e n g i n e : : T e x t C o m p o n e n t > ( g o . g e t ( ) ,   " P r o g   4   E x a m   e n g i n e " ,   f o n t ) ) ; 
 
 	 s c e n e . A d d ( s t d : : m o v e ( g o ) ) ; 
 
 } 
 
 
 
 i n t   m a i n ( i n t ,   c h a r * [ ] )   { 
 
 	 e n g i n e : : M i n i g i n   e n g i n e ( " . . / D a t a / " ) ; 
 
 	 e n g i n e . R u n ( l o a d ) ; 
 
         r e t u r n   0 ; 
 
 } 