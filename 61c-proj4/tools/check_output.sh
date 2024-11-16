#!/usr/bin/env bash
set -eu

help() {
	echo "Usage: bash check_output.sh <output_dir> [reference_dir]"
}

if [ -z "${1:-}" ]; then
	help
	exit 1
fi

if [ -z "${2:-}" ]; then
	out_dir="${1}"
	ref_dir="${1}"
else
	out_dir="${1}"
	ref_dir="${2}"
fi

diff_ret=0
diff -q ${ref_dir}/input.txt ${out_dir}/input.txt >/dev/null || diff_ret=$?

if [ $diff_ret -ne 0 ]; then
	echo "${ref_dir} and ${out_dir} contain different input.txts"
	exit -1
fi

passed=0
total=0

while IFS= read -r task_name; do
	diff_ret=0
	diff -q ${ref_dir}/${task_name}/ref.bin ${out_dir}/${task_name}/out.bin >/dev/null || diff_ret=$?

	if [ $diff_ret -ne 0 ]; then
		echo "${task_name}: output does not match reference"
	else
		passed=$(($passed + 1))
	fi
	total=$(($total + 1))
done < <(tail -n +2 ${ref_dir}/input.txt)

echo "${passed}/${total} tests passed"

if [ "$passed" -ne "$total" ]; then
	exit 61
fi
