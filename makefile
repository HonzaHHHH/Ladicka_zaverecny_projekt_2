KOMPILATOR = gcc
VYSTUP = ladicka
SESTAVENI = build
ZDROJAKY = src
HLAVICKY = include

KODY = $(wildcard $(ZDROJAKY)/*.c)
OBJEKTY = $(patsubst $(ZDROJAKY)/%.c, $(SESTAVENI)/%.o, $(KODY))

all: $(SESTAVENI)/$(VYSTUP)

$(SESTAVENI)/$(VYSTUP) : $(OBJEKTY)
	$(KOMPILATOR) $(OBJEKTY) -lm -lportaudio -g -I$(HLAVICKY) -o $@

$(SESTAVENI)/%.o : $(ZDROJAKY)/%.c | $(SESTAVENI)
	$(KOMPILATOR) -g -c $< -I$(HLAVICKY) -o $@

$(SESTAVENI):
	mkdir -p $(SESTAVENI)

clean:
	rm -rf $(SESTAVENI)/* $(SESTAVENI)/$(VYSTUP)

rebuild: clean all