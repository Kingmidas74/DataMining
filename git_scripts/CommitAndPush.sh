#!/bin/sh

cd ..
git add .
git commit -a -m "$@"
git push
