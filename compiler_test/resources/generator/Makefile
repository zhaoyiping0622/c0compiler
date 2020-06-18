PYS=$(shell find py -maxdepth 1 -regex ".*\\.py" -type f)
JSONS=$(shell find py -maxdepth 1 -regex ".*\\.py" -type f | sed 's/py/json/g')
C0=$(shell find c0 -maxdepth 1 -regex ".*\\.c0" -type f)
TACJSONS=$(shell find c0 -maxdepth 1 -regex ".*\\.c0" -type f | sed 's/\(.*\)c0/\1json/g' | sed 's/c0/tacJSON/')
TACS=$(shell find c0 -maxdepth 1 -regex ".*\\.c0" -type f | sed 's/c0/tac/g')
all: $(JSONS) $(TACJSONS) $(TACS)
clean:
	rm $(JSONS) $(TACJSONS) $(TACS)
json/%.json:py/%.py py/TAC/TAC.py
	python3 $< > $@
tacJSON/%.json:c0/%.c0
	./compiler-v0.1 --tac $< -o $@
tac/%.tac:tacJSON/%.json tacJSON2TAC.py
	python3 tacJSON2TAC.py $< $@
