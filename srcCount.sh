echo compiler: 
find compiler -regex ".*\\.\(cpp\|h\)" | xargs cat | wc -l
echo compiler_test: 
find compiler_test -regex ".*\\.\(cpp\|h\|py\)" | xargs cat | wc -l
