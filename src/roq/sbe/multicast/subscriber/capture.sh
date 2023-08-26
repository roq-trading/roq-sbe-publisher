#!/usr/bin/env bash

tcpdump -i enp2s0 -s 65535 -w test.pcap '(host 224.1.1.1 or host 224.2.2.2) and (port 6789 or port 4321)'
