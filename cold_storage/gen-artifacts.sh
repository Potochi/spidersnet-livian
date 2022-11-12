#!/bin/bash

docker compose build
docker compose up -d
docker compose cp chall:/home/ctf/cold_storage ./artifacts/
docker compose cp chall:/lib/x86_64-linux-gnu/libc.so.6 ./artifacts/
docker compose down

