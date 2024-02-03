#!/usr/bin/env bash

NAME="trader"

KERNEL="$(uname -a)"

case "$KERNEL" in
  Linux*)
    LOCAL_INTERFACE=$(ip route get 8.8.8.8 | sed -n 's/.*src \([^\ ]*\).*/\1/p')
    ;;
  Darwin*)
    LOCAL_INTERFACE=$(osascript -e "IPv4 address of (system info)")
    ;;
  *)
    (>&2 echo -e "\033[1;31mERROR: Unknown architecture.\033[0m") && exit 1
esac

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
  --local_interface "$LOCAL_INTERFACE" \
  --multicast_address_snapshot 224.1.1.1 \
  --multicast_port_snapshot 1234 \
  --multicast_address_incremental 224.1.1.1 \
  --multicast_port_incremental 6789 \
  --multicast_ttl 4 \
  --multicast_loop=true \
  --snapshot_publish_freq 5s \
  --max_depth 5 \
  $@
