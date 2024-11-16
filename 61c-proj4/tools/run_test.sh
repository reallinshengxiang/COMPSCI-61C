#!/usr/bin/env bash
set -eu

help() {
	echo "Usage: bash run_test.sh [command]"
}

if [ -z "${2:-}" ]; then
	help
	exit 1
fi

echo "Command: ${@:1}"

start_time="$(date +%s%3N)"
${1} ${@:2}
return_code="$?"
end_time="$(date +%s%3N)"

echo "Time elapsed: $((end_time - start_time))ms"

if [ $return_code -ne 0 ]; then
	exit $return_code
fi
