#!/bin/bash
make

cd Buffer

sudo python load.py

cd ../test

./prodcons 1 1 1 1

mkdir "prodcons_run1"

mv *.log prodcons_run1/

./prodcons 10 10 10 10

mkdir "prodcons_run10"

mv *.log prodcons_run10/

mkdir "prodcons_run100"

./prodcons 100 100 100 100

mv *.log prodcons_run100/

cd ../Buffer

sudo python unload.py
