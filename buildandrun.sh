#!/bin/bash

echo ""
echo ""
echo ""
echo "Compiling buffer module and test files..."
sleep 3
make

echo ""
echo ""
echo ""
echo "Loading scullBuffer..."
sleep 3
cd Buffer
sudo python load.py

echo ""
echo ""
echo ""
echo "Run test with 1 consumer and 1 producer with 1 item each..."
sleep 4
cd ../test
./prodcons 1 1 1 1

echo ""
echo ""
echo ""
echo "Storing log data in test/prodcons_run1 folder..."
sleep 3
mkdir "prodcons_run1"
mv *.log prodcons_run1/

echo ""
echo ""
echo ""
echo "Run test with 10 consumers and producers with 10 items each..."
sleep 4
./prodcons 10 10 10 10

echo ""
echo ""
echo ""
echo "Storing log data in test/prodcons_run10 folder..."
sleep 3
mkdir "prodcons_run10"
mv *.log prodcons_run10/
mkdir "prodcons_run100"

echo ""
echo ""
echo ""
echo "Run test with 100 consumers and producers with 100 items each..."
sleep 4
./prodcons 100 100 100 100

echo ""
echo ""
echo ""
echo "Storing log data in test/prodcons_run100 folder..."
sleep 3
mv *.log prodcons_run100/
cd ../Buffer

echo ""
echo ""
echo ""
echo "Unloading scullBuffer module..."
sleep 3
sudo python unload.py

echo ""
echo ""
echo ""
echo "Clean up make files..."
sleep 3
cd ..
make clean

echo ""
echo ""
echo ""

