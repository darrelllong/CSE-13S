TEXROOTS=coding.tex

OUTPUTS=$(TEXROOTS:.tex=.pdf)
DEPS=$(TEXROOTS:.tex=.d)

#ifeq ("$(wildcard bib/csrg.bib)","")
#$(error "Please update the bib submodule: git submodule update --init --recursive")
#endif

all: $(OUTPUTS)

-include $(DEPS)

%.pdf: %.tex
	latexmk -pdf -M -MF $(DEPS) $<

clean:
	$(RM) *.bak *.dvi *.bbl *.blg *.aux *.fls *.log *.fdb_latexmk $(DEPS) *.out *.def *.ques *.listing

spotless: clean
	$(RM) $(OUTPUTS)

.PHONY: clean spotless all
