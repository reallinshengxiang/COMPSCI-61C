#!/usr/bin/env bash
set -euf -o pipefail

SSH_HOST=${1:-s275-4.cs.berkeley.edu}

for OUTPUT in $(ssh "$SSH_HOST" -G | grep "^identityfile " | cut -d " " -f2-)
do
    filename="${OUTPUT/#\~/$HOME}"
    if [[ -f $filename ]]; then
        echo "You already have an SSH key at: $OUTPUT"
        echo "Make note of this, as you'll need this information later!"
        exit 0
    fi
done

echo "I couldn't find an existing SSH key for you, so let's make one now..."
ssh-keygen -t ed25519 -f ~/.ssh/id_ed25519 -N ''
echo "Great! Your new SSH key is at: ~/.ssh/id_ed25519"
echo "Make note of this, as you'll need this information later!"
