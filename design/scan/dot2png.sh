for file in `ls dot/*.dot`
do
dot -T png ${file} -o png/`basename ${file} .dot`.png
done
