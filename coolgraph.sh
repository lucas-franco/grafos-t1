let SIZE=1000
for ((i = 0; i<$SIZE; i++))
do
    for ((j = 0; j<$SIZE; j++))
    do
        echo "$i $j"
    done
done > testes/big-$SIZE.txt