KOMPILATOR = gcc
VYSTUP = ladicka
SESTAVENI = build
ZDROJAKY = src
HLAVICKY = include
KODY = $(wildcard $(ZDROJAKY)/*.c)
OBJEKTY = $(patsubst $(ZDROJAKY)/%.c, $(SESTAVENI)/%.o, $(KODY))


all: $(VYSTUP)

VYSTUP : $(OBJEKTY)
	$(KOMPILATOR) $(OBJEKTY) -I$(HLAVICKY) -o $(SESTAVENI)/$

$(SESTAVENI)/%.o : $(ZDROJAKY)/%.c
	$(KOMPILATOR) $(ZDROJAKY)/%.c -o $@

clean:
rm -r ./$(SESTAVENI)/*