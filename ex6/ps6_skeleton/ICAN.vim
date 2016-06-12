let SessionLoad = 1
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
cd ~/Homework/TDT4205/ex6/ps6_skeleton
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +291 src/generator.c
badd +6 vsl_programs/euclid.vsl
badd +1 vsl_programs/fibonacci_iterative.vsl
badd +1 vsl_programs/fibonacci_recursive.vsl
badd +10 vsl_programs/globals_and_args.vsl
badd +9 vsl_programs/newton.vsl
badd +10 vsl_programs/prime.vsl
badd +11 make.sh
badd +124 ~/Homework/TDT4205/ex5/ps5_skeleton/src/generator.c
badd +1 term://.//12391:bash
badd +0 vimfiler:explorer@1
badd +1 src/vslc.c
badd +0 term://.//14421:bash
badd +2 build/output.s
badd +9 test.c
badd +34 test.s
badd +0 term://.//16430:bash
badd +1 build/outputs
badd +1 build/output.
badd +5 scratch.txt
badd +104 src/parser.y
badd +1 ../../ex4/ps5_skeleton/src/generator.c
badd +173 ~/Homework/TDT4205/ex4/ps4_skeleton/src/ir.c
argglobal
silent! argdel *
argadd src/generator.c
edit src/generator.c
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
wincmd _ | wincmd |
split
1wincmd k
wincmd _ | wincmd |
vsplit
wincmd _ | wincmd |
vsplit
wincmd _ | wincmd |
vsplit
3wincmd h
wincmd w
wincmd w
wincmd w
wincmd w
wincmd _ | wincmd |
vsplit
wincmd _ | wincmd |
vsplit
2wincmd h
wincmd w
wincmd w
wincmd t
set winheight=1 winwidth=1
exe 'vert 1resize ' . ((&columns * 105 + 159) / 318)
exe '2resize ' . ((&lines * 29 + 31) / 63)
exe 'vert 2resize ' . ((&columns * 53 + 159) / 318)
exe '3resize ' . ((&lines * 29 + 31) / 63)
exe 'vert 3resize ' . ((&columns * 54 + 159) / 318)
exe '4resize ' . ((&lines * 29 + 31) / 63)
exe 'vert 4resize ' . ((&columns * 51 + 159) / 318)
exe '5resize ' . ((&lines * 29 + 31) / 63)
exe 'vert 5resize ' . ((&columns * 51 + 159) / 318)
exe '6resize ' . ((&lines * 29 + 31) / 63)
exe 'vert 6resize ' . ((&columns * 106 + 159) / 318)
exe '7resize ' . ((&lines * 29 + 31) / 63)
exe 'vert 7resize ' . ((&columns * 52 + 159) / 318)
exe '8resize ' . ((&lines * 29 + 31) / 63)
exe 'vert 8resize ' . ((&columns * 52 + 159) / 318)
argglobal
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
22
normal! zo
22
normal! zc
34
normal! zo
34
normal! zc
49
normal! zo
49
normal! zc
98
normal! zo
101
normal! zo
105
normal! zo
106
normal! zo
111
normal! zo
112
normal! zo
122
normal! zo
139
normal! zo
141
normal! zo
141
normal! zc
181
normal! zo
186
normal! zo
188
normal! zo
139
normal! zc
215
normal! zo
233
normal! zo
235
normal! zo
238
normal! zo
235
normal! zc
233
normal! zc
269
normal! zo
289
normal! zo
306
normal! zo
326
normal! zo
345
normal! zo
364
normal! zo
387
normal! zo
407
normal! zo
426
normal! zo
426
normal! zc
364
normal! zc
467
normal! zo
469
normal! zo
487
normal! zo
495
normal! zo
497
normal! zo
512
normal! zo
535
normal! zo
let s:l = 269 - ((55 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
269
normal! 029|
wincmd w
argglobal
edit scratch.txt
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
8
normal! zo
let s:l = 1 - ((0 * winheight(0) + 14) / 29)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
1
normal! 0
lcd ~/Homework/TDT4205/ex6/ps6_skeleton
wincmd w
argglobal
edit ~/Homework/TDT4205/ex6/ps6_skeleton/vsl_programs/prime.vsl
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
6
normal! zo
let s:l = 15 - ((14 * winheight(0) + 14) / 29)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
15
normal! 09|
wincmd w
argglobal
edit ~/Homework/TDT4205/ex6/ps6_skeleton/test.s
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 17 - ((13 * winheight(0) + 14) / 29)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
17
normal! 09|
lcd ~/Homework/TDT4205/ex6/ps6_skeleton
wincmd w
argglobal
edit ~/Homework/TDT4205/ex6/ps6_skeleton/test.c
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
3
normal! zo
let s:l = 8 - ((7 * winheight(0) + 14) / 29)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
8
normal! 017|
lcd ~/Homework/TDT4205/ex6/ps6_skeleton
wincmd w
argglobal
edit term://.//12391:bash
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 1029 - ((28 * winheight(0) + 14) / 29)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
1029
normal! 07|
wincmd w
argglobal
edit ~/Homework/TDT4205/ex6/ps6_skeleton/build/output.s
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 1 - ((0 * winheight(0) + 14) / 29)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
1
normal! 0
lcd ~/Homework/TDT4205/ex6/ps6_skeleton
wincmd w
argglobal
edit ~/Homework/TDT4205/ex6/ps6_skeleton/make.sh
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 13 - ((12 * winheight(0) + 14) / 29)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
13
normal! 0
lcd ~/Homework/TDT4205/ex6/ps6_skeleton
wincmd w
exe 'vert 1resize ' . ((&columns * 105 + 159) / 318)
exe '2resize ' . ((&lines * 29 + 31) / 63)
exe 'vert 2resize ' . ((&columns * 53 + 159) / 318)
exe '3resize ' . ((&lines * 29 + 31) / 63)
exe 'vert 3resize ' . ((&columns * 54 + 159) / 318)
exe '4resize ' . ((&lines * 29 + 31) / 63)
exe 'vert 4resize ' . ((&columns * 51 + 159) / 318)
exe '5resize ' . ((&lines * 29 + 31) / 63)
exe 'vert 5resize ' . ((&columns * 51 + 159) / 318)
exe '6resize ' . ((&lines * 29 + 31) / 63)
exe 'vert 6resize ' . ((&columns * 106 + 159) / 318)
exe '7resize ' . ((&lines * 29 + 31) / 63)
exe 'vert 7resize ' . ((&columns * 52 + 159) / 318)
exe '8resize ' . ((&lines * 29 + 31) / 63)
exe 'vert 8resize ' . ((&columns * 52 + 159) / 318)
tabedit ~/Homework/TDT4205/ex6/ps6_skeleton/src/vslc.c
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
wincmd t
set winheight=1 winwidth=1
exe 'vert 1resize ' . ((&columns * 158 + 159) / 318)
exe 'vert 2resize ' . ((&columns * 159 + 159) / 318)
argglobal
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 13 - ((12 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
13
normal! 0
lcd ~/Homework/TDT4205/ex6/ps6_skeleton
wincmd w
argglobal
edit term://.//14421:bash
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 393 - ((36 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
393
normal! 09|
wincmd w
exe 'vert 1resize ' . ((&columns * 158 + 159) / 318)
exe 'vert 2resize ' . ((&columns * 159 + 159) / 318)
tabedit ~/Homework/TDT4205/ex6/ps6_skeleton/vsl_programs/fibonacci_iterative.vsl
set splitbelow splitright
wincmd _ | wincmd |
vsplit
wincmd _ | wincmd |
vsplit
2wincmd h
wincmd w
wincmd w
wincmd t
set winheight=1 winwidth=1
exe 'vert 1resize ' . ((&columns * 35 + 159) / 318)
exe 'vert 2resize ' . ((&columns * 140 + 159) / 318)
exe 'vert 3resize ' . ((&columns * 141 + 159) / 318)
argglobal
enew
file ~/Homework/TDT4205/ex6/ps6_skeleton/vimfiler:explorer@2
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal nofen
wincmd w
argglobal
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 13 - ((12 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
13
normal! 0
wincmd w
argglobal
edit ~/Homework/TDT4205/ex6/ps6_skeleton/src/parser.y
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
102
normal! zo
let s:l = 113 - ((15 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
113
normal! 0
wincmd w
exe 'vert 1resize ' . ((&columns * 35 + 159) / 318)
exe 'vert 2resize ' . ((&columns * 140 + 159) / 318)
exe 'vert 3resize ' . ((&columns * 141 + 159) / 318)
tabnext 1
if exists('s:wipebuf') && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=1 shortmess=aTI
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
