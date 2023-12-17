#! /bin/bash

if [ -z "$1" ]; then
    echo -e "para ayuda \n./main.sh -h"
elif [ "$1" = "-h" ]; then
    echo -e "para compilar todos los algoritmos \n./main.sh -c"
    echo -e "para ejecutar la calculadora \n./main.sh -e"
elif [ "$1" = "-c" ]; then
    echo "compilando..."
    rm md4.out md5.out sha1.out sha256.out hmac.out
    g++ md4.cpp -o md4.out
    g++ md5.cpp -o md5.out
    g++ sha1.cpp -o sha1.out
    g++ sha256.cpp -o sha256.out
    g++ hmac.cpp -o hmac.out
    if [ ! "0" -eq "$?" ]; then
        echo "error al compilar los algoritmos"
    else
        echo "compilado con exito"
    fi
elif [ "$1" = "-e" ]; then
    python3 main.py
fi
