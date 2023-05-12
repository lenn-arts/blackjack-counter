#!/bin/bash
git pull && ./test
sleep 5
git add ./rgb*
git commit -m "image uploaded"
git push
sleep 20
git pull
cat output.txt

