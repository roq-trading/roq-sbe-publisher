#!/usr/bin/env bash

sudo tcpdump -i enp2s0 -vvv '(host 224.1.1.1 and (port 1234 or port 6789))'
