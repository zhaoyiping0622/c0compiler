sed 's/ .*//g' grammar.en > enkeys
sed 's/ .*//g' grammar.zh > zhkeys
python dst.py
rm enkeys zhkeys
