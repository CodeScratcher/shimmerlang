" Vim syntax file
" Language: Shimmer
" Maintainer: Ari Fisher
" Latest Revision: 4 October 2020

if exists("b:current_syntax")
  finish
endif

syn keyword shmrLanguageKeywords if ifElse define $
syn match shmrNumber '\d\+'

syn region shmrString start='"' end='"'

hi def link shmrString     Constant
hi def link shmrNumber     Constant
hi def link shmrLanguageKeywords              Statement