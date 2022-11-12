#!/bin/bash

(cd ./build/ && cmake .. && make)
cp ./build/chall.gba ./artifacts/chall.gba
