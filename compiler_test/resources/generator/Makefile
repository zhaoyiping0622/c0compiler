PYS=$(shell find py -maxdepth 1 -regex ".*\\.py" -type f)
JSONS=$(shell find py -maxdepth 1 -regex ".*\\.py" -type f | sed 's/py/json/g')
all: $(JSONS)
clean:
	rm $(JSONS)
json/%.json:py/%.py py/TAC/TAC.py
	python3 $< > $@
