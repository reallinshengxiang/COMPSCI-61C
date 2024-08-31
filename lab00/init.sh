#!/bin/bash
set -eu

lab00_ref="starter/lab00-oski"

git fetch starter

lab00_start_ref="$(git rev-parse "${lab00_ref}~1")"

git tag -f lab00-backup
git push -f origin refs/tags/lab00-backup
git reset --hard "$lab00_start_ref"
git push -f origin "${lab00_ref}:main"
git update-ref refs/remotes/origin/main "$lab00_start_ref" "$lab00_ref"

echo "Finished setup"
