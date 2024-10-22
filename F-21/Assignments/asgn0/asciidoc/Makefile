DOC = asgn0.adoc
SECTIONS = $(wildcard sections/*.adoc)
PDF = $(DOC:%.adoc=%.pdf)
HTML = $(DOC:%.adoc=%.html)
EPUB = $(DOC:%.adoc=%.epub)
ATTR = -a mathematical-format=svg

all: $(PDF) $(HTML) $(EPUB)

$(PDF): $(DOC) $(SECTIONS)
	asciidoctor-pdf -r asciidoctor-mathematical $(ATTR) -o $@ $<

$(HTML): $(DOC) $(SECTIONS)
	asciidoctor -a latexmath -o $@ $<

$(EPUB): $(DOC) $(SECTIONS)
	asciidoctor-epub3 -r asciidoctor-mathematical $(ATTR) -o $@ $<

clean:
	rm -f *.{pdf,html,epub,svg,png}
