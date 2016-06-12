let SessionLoad = 1
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
cd ~/Homework/TDT4205/ex6/ps6_skeleton
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +280 src/generator.c
badd +1 vsl_programs/euclid.vsl
badd +1 vsl_programs/fibonacci_iterative.vsl
badd +1 vsl_programs/fibonacci_recursive.vsl
badd +10 vsl_programs/globals_and_args.vsl
badd +9 vsl_programs/newton.vsl
badd +11 vsl_programs/prime.vsl
badd +12 make.sh
badd +178 ~/Homework/TDT4205/ex5/ps5_skeleton/src/generator.c
badd +0 term://.//13322:bash
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
1wincmd h
wincmd w
wincmd w
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
wincmd t
set winheight=1 winwidth=1
exe 'vert 1resize ' . ((&columns * 79 + 119) / 239)
exe '2resize ' . ((&lines * 31 + 33) / 67)
exe 'vert 2resize ' . ((&columns * 79 + 119) / 239)
exe '3resize ' . ((&lines * 31 + 33) / 67)
exe 'vert 3resize ' . ((&columns * 79 + 119) / 239)
exe '4resize ' . ((&lines * 31 + 33) / 67)
exe 'vert 4resize ' . ((&columns * 79 + 119) / 239)
exe '5resize ' . ((&lines * 31 + 33) / 67)
exe 'vert 5resize ' . ((&columns * 79 + 119) / 239)
argglobal
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
50
normal! zo
100
normal! zo
103
normal! zo
107
normal! zo
113
normal! zo
114
normal! zo
236
normal! zo
238
normal! zo
250
normal! zo
260
normal! zo
280
normal! zo
306
normal! zo
322
normal! zo
let s:l = 274 - ((17 * winheight(0) + 31) / 63)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
274
normal! 0
lcd ~/Homework/TDT4205/ex6/ps6_skeleton
wincmd w
argglobal
edit ~/Homework/TDT4205/ex5/ps5_skeleton/src/generator.c
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
103
normal! zo
110
normal! zo
199
normal! zo
217
normal! zo
229
normal! zo
let s:l = 114 - ((7 * winheight(0) + 15) / 31)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
114
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
let s:l = 12 - ((10 * winheight(0) + 15) / 31)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
12
normal! 05|
wincmd w
argglobal
edit term://.//13322:bash
setlocal fdm=indent
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 31 - ((30 * winheight(0) + 15) / 31)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
31
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
let s:l = 12 - ((8 * winheight(0) + 15) / 31)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
12
normal! 05|
lcd ~/Homework/TDT4205/ex6/ps6_skeleton
wincmd w
4wincmd w
exe 'vert 1resize ' . ((&columns * 79 + 119) / 239)
exe '2resize ' . ((&lines * 31 + 33) / 67)
exe 'vert 2resize ' . ((&columns * 79 + 119) / 239)
exe '3resize ' . ((&lines * 31 + 33) / 67)
exe 'vert 3resize ' . ((&columns * 79 + 119) / 239)
exe '4resize ' . ((&lines * 31 + 33) / 67)
exe 'vert 4resize ' . ((&columns * 79 + 119) / 239)
exe '5resize ' . ((&lines * 31 + 33) / 67)
exe 'vert 5resize ' . ((&columns * 79 + 119) / 239)
tabedit ~/Homework/TDT4205/ex6/ps6_skeleton/vsl_programs/prime.vsl
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
wincmd t
set winheight=1 winwidth=1
exe 'vert 1resize ' . ((&columns * 35 + 119) / 239)
exe 'vert 2resize ' . ((&columns * 203 + 119) / 239)
argglobal
enew
file ~/Homework/TDT4205/ex6/ps6_skeleton/vimfiler:explorer@1
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
let s:l = 5 - ((4 * winheight(0) + 31) / 63)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
5
normal! 0
wincmd w
exe 'vert 1resize ' . ((&columns * 35 + 119) / 239)
exe 'vert 2resize ' . ((&columns * 203 + 119) / 239)
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
