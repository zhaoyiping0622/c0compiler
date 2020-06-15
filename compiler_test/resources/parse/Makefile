PYS=$(shell find py -maxdepth 1 -regex ".*\\.py" -type f)
JSONS=$(shell find py -maxdepth 1 -regex ".*\\.py" -type f | sed 's/py/json/g')
TXTS=$(shell find txt -maxdepth 1 -regex ".*\\.txt" -type f)
TOKENS=$(shell find txt -maxdepth 1 -regex ".*\\.txt" -type f | sed 's/txt/token/g')
all: makeDir $(TOKENS) $(JSONS)
clean:
	rm $(TOKENS) $(JSONS)
token/%.token:txt/%.txt txt2token.py 
	python3 txt2token.py $< -o $@
json/%.json:py/%.py py/AST/AST.py py/TAC/TAC.py
	python3 $< > $@
makeDir:
	mkdir -p json token
