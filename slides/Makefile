all: slide-2013-12-04.pdf

%.pdf: %.markdown
	pandoc $< -o $@ -t beamer

open: slide-2013-12-04.pdf
	gvfs-open $<
