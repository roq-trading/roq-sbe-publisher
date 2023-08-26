#!/usr/bin/env bash

NAME="risk"

if [ "$1" == "debug" ]; then
  KERNEL="$(uname -a)"
  case "$KERNEL" in
    Linux*)
      PREFIX="gdb --args"
      ;;
    Darwin*)
      PREFIX="lldb --"
      ;;
  esac
  shift 1
else
  PREFIX=
fi

$PREFIX "./roq-sbe-multicast-test-subscriber" \
  --local_interface 192.168.188.64 \
  --multicast_address 224.1.1.1 \
  --multicast_port 6789 \
  $@
