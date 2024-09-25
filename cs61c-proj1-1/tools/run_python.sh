#!/usr/bin/env bash
set -eu

###########################################################
# Change the next line as needed if auto-detection fails. #
# When in doubt, ask on Piazza.                           #
#   e.g. python_exec="py"                                 #
###########################################################
python_exec=""
if [ -z "${python_exec}" ]; then
  if command -v py 2>&1 >/dev/null; then
    python_exec=py
  elif command -v python3 2>&1 >/dev/null; then
    python_exec=python3
  elif command -v python 2>&1 >/dev/null; then
    python_exec=python
  fi
fi
if [[ "$("${python_exec}" -c 'import sys; print(sys.version_info[0])')" -ne "3" ]]; then
  echo "Error: couldn't detect Python version. Is `${python_exec}` Python 3?"
  "${python_exec}" -V
  exit 1
fi

help() {
  echo "Expected arg: path to Python script"
}

if [ -z "${1:-}" ]; then
  help
  exit 1
fi

exec "${python_exec}" "${1}"
