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
  echo "------------------------------HELP------------------------------"
  echo "To download Logisim & Venus: bash test.sh download_tools"
  echo "Part A:"
  echo "    All part A tests:       bash test.sh part_a"
  echo "    ALU:                    bash test.sh test_alu"
  echo "    RegFile:                bash test.sh test_regfile"
  echo "    addi:                   bash test.sh test_addi"
  echo ""
  echo "Part B - All Tests:         bash test.sh part_b"
  echo "Part B - Unit Tests:"
  echo "    Branch comparator:      bash test.sh test_branch_comp"
  echo "    Immediate generator:    bash test.sh test_imm_gen"
  echo "    Partial load:           bash test.sh test_partial_load"
  echo "    Partial store:          bash test.sh test_partial_store"
  echo "Part B - Integration Tests (not comprehensive):"
  echo "    Registers:              bash test.sh test_integration_all_regs"
  echo "    Branches:               bash test.sh test_integration_branch"
  echo "    Immediates:             bash test.sh test_integration_immediates"
  echo "    Jumps:                  bash test.sh test_integration_jump"
  echo "    lui:                    bash test.sh test_integration_lui"
  echo "    Memory:                 bash test.sh test_integration_mem"
  echo "    Basic programs:         bash test.sh test_integration_programs"
  echo "Part B - Custom Tests:"
  echo "    Create and run custom tests:    bash test.sh test_custom"
  echo "    To only create custom tests:    bash test.sh create_custom"
  echo "    To only run custom tests:       bash test.sh run_custom"
  echo "Part B - Pipelined:"
  echo "    Add --pipelined after any command above"
  echo "    Example: bash test.sh run_custom --pipelined"
  echo "----------------------------------------------------------------"
}

if [ -z "${1:-}" ]; then
  help
  exit 1
fi

case "${1}" in
  part_a)
    echo "Testing ALU:"
    "${python_exec}" tools/run_test.py ${@:2} tests/unit-alu/
    echo ""
    echo "Testing RegFile:"
    "${python_exec}" tools/run_test.py ${@:2} tests/unit-regfile/
    echo ""
    echo "Testing addi:"
    "${python_exec}" tools/run_test.py ${@:2} tests/integration-addi/
    ;;
  test_addi)
    "${python_exec}" tools/run_test.py ${@:2} tests/integration-addi/
    ;;
  test_alu)
    "${python_exec}" tools/run_test.py ${@:2} tests/unit-alu/
    ;;
  test_regfile)
    "${python_exec}" tools/run_test.py ${@:2} tests/unit-regfile/
    ;;
  part_b)
    echo "Unit testing branch comparator:"
    echo ""
    "${python_exec}" tools/run_test.py ${@:2} tests/unit-branch-comp/
    echo "Unit testing immediate generator:"
    echo ""
    "${python_exec}" tools/run_test.py ${@:2} tests/unit-imm-gen/
    echo "Unit testing partial load:"
    echo ""
    "${python_exec}" tools/run_test.py ${@:2} tests/unit-partial-load/
    echo "Unit testing partial store:"
    echo ""
    "${python_exec}" tools/run_test.py ${@:2} tests/unit-partial-store/
    echo "Integration testing all registers:"
    echo ""
    "${python_exec}" tools/run_test.py ${@:2} tests/integration-all-regs/
    echo "Integration testing branches:"
    echo ""
    "${python_exec}" tools/run_test.py ${@:2} tests/integration-branch/
    echo "Integration testing immediates:"
    echo ""
    "${python_exec}" tools/run_test.py ${@:2} tests/integration-immediates/
    echo "Integration testing jumps:"
    echo ""
    "${python_exec}" tools/run_test.py ${@:2} tests/integration-jump/
    echo "Integration testing lui:"
    echo ""
    "${python_exec}" tools/run_test.py ${@:2} tests/integration-lui/
    echo "Integration testing memory:"
    echo ""
    "${python_exec}" tools/run_test.py ${@:2} tests/integration-mem/
    echo "Integration testing basic programs:"
    echo ""
    "${python_exec}" tools/run_test.py ${@:2} tests/integration-programs/
    ;;
  test_branch_comp)
    "${python_exec}" tools/run_test.py ${@:2} tests/unit-branch-comp/
    ;;
  test_imm_gen)
    "${python_exec}" tools/run_test.py ${@:2} tests/unit-imm-gen/
    ;;
  test_partial_load)
    "${python_exec}" tools/run_test.py ${@:2} tests/unit-partial-load/
    ;;
  test_partial_store)
    "${python_exec}" tools/run_test.py ${@:2} tests/unit-partial-store/
    ;;
  test_integration_all_regs)
    "${python_exec}" tools/run_test.py ${@:2} tests/integration-all-regs/
    ;;
  test_integration_branch)
    "${python_exec}" tools/run_test.py ${@:2} tests/integration-branch/
    ;;
  test_integration_immediates)
    "${python_exec}" tools/run_test.py ${@:2} tests/integration-immediates/
    ;;
  test_integration_jump)
    "${python_exec}" tools/run_test.py ${@:2} tests/integration-jump/
    ;;
  test_integration_lui)
    "${python_exec}" tools/run_test.py ${@:2} tests/integration-lui/
    ;;
  test_integration_mem)
    "${python_exec}" tools/run_test.py ${@:2} tests/integration-mem/
    ;;
  test_integration_programs)
    "${python_exec}" tools/run_test.py ${@:2} tests/integration-programs/
    ;;
  test_custom)
    "${python_exec}" tools/create_test.py
    "${python_exec}" tools/run_test.py ${@:2} tests/integration-custom/
    ;;
  create_custom)
    "${python_exec}" tools/create_test.py ${@:2}
    ;;
  run_custom)
    "${python_exec}" tools/run_test.py ${@:2} tests/integration-custom/
    ;;
  run)
    "${python_exec}" tools/run_test.py ${@:2}
    ;;
  diff)
    "${python_exec}" tools/diff_output.py ${@:2}
    ;;
  format)
    "${python_exec}" tools/format_output.py ${@:2}
    ;;
  download_tools)
    bash tools/download_tools.sh ${@:2}
    ;;
  *)
    help
    exit 1
    ;;
esac
