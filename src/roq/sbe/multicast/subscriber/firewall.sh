#!/usr/bin/env bash

firewall-cmd --add-rich-rule='rule family=ipv4 destination address="224.1.0.0/16" accept'
