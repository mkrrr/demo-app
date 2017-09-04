#!/bin/bash
q=$(( $RANDOM % 5 ))
port=$(( $RANDOM % 1000 + 1000))
host="localhost"

python2 asker.py $host $port $q
