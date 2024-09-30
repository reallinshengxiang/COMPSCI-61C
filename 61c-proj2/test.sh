#!/usr/bin/env bash
set -eu

###########################################################
# Change the next line as needed if auto-detection fails. #
# When in doubt, ask on Ed.                               #
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
  echo "--------------------HELP--------------------"
  echo "To test this project type: bash test.sh all"
  echo "To test coverage, type: bash test.sh coverage"
  echo "To test a function type: bash test.sh test_FUNCTION_NAME"
  echo "    Remember to replace FUNCTION_NAME"
  echo "--------------------------------------------"
}

if [ -z "${1:-}" ]; then
  help
  exit 1
fi

case "${1}" in
  download_tools)
    bash tools/download_tools.sh venus
    ;;
  all)
    "${python_exec}" unittests.py -v -- "${@:2}"
    ;;
  coverage)
    "${python_exec}" studenttests.py -- "${@:2}"
    ;;
  test_abs)
    "${python_exec}" unittests.py -v TestAbs -- "${@:2}"
    ;;
  test_relu)
    "${python_exec}" unittests.py -v TestRelu -- "${@:2}"
    ;;
  test_argmax)
    "${python_exec}" unittests.py -v TestArgmax -- "${@:2}"
    ;;
  test_dot)
    "${python_exec}" unittests.py -v TestDot -- "${@:2}"
    ;;
  test_matmul)
    "${python_exec}" unittests.py -v TestMatmul -- "${@:2}"
    ;;
  test_read_matrix)
    "${python_exec}" unittests.py -v TestReadMatrix -- "${@:2}"
    ;;
  test_write_matrix)
    "${python_exec}" unittests.py -v TestWriteMatrix -- "${@:2}"
    ;;
  test_classify)
    "${python_exec}" unittests.py -v TestClassify -- "${@:2}"
    ;;
  test_chain)
    "${python_exec}" unittests.py -v TestChain -- "${@:2}"
    ;;
  *)
    help
    exit 1
    ;;
esac
