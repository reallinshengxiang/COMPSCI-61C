#!/usr/bin/env bash
set -u

COLOR_GREEN=
COLOR_RESET=
if [[ "${NO_COLOR:-}" == "" ]]; then
  if [[ "$TERM" == xterm* ]]; then
    COLOR_GREEN="$(tput setaf 2)"
    COLOR_RESET="$(tput sgr0)"
  fi
fi

./snake -i "tests/22-nonexistent-in.snk" -o "tests/22-nonexistent-out.snk"
if [ $? -eq 255 ]; then
  echo "${COLOR_GREEN}Passed 22-nonexistent-input-file${COLOR_RESET}";
else
  echo "Failed 22-nonexistent-input-file";
fi
