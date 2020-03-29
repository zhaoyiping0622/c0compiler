TXTs=$(shell ls txt | grep -v py | grep -v total | sed "s:^:txt/:")
PNGs=$(subst txt,png,$(TXTs) txt/total.txt )
CPPs=$(subst txt,cpp,$(TXTs) txt/total.txt )
DOTs=$(subst txt,dot,$(TXTs) txt/total.txt )
all : $(TXTs) $(CPPs) $(DOTs) $(PNGs)
echo :
	echo $(TXTs)
	echo $(PNGs)
	echo $(CPPs)
	echo $(DOTs)
txt/total.txt: $(TXTs)
	cd txt && python total.py
cpp/%.cpp:txt/%.txt
	python txt2cpp.py $< -o $@
dot/%.dot:txt/%.txt
	python txt2dot.py $< -o $@
png/%.png:dot/%.dot
	dot $< -T png -o $@
clean:
	rm png/* cpp/* dot/*
