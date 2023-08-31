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

$PREFIX "./roq-sbe-publisher" \
  --name "$NAME" \
  --config_file "test.toml" \
  --local_interface 192.168.188.64 \
  --multicast_address_snapshot 224.1.1.2 \
  --multicast_port_snapshot 1234 \
  --multicast_address_incremental 224.1.1.1 \
  --multicast_port_incremental 6789 \
  --multicast_ttl 4 \
  --multicast_loop=true \
  $@
