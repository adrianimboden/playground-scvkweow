#!/bin/bash

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"

/build.py --flavor debug --path ${SCRIPTPATH} --target $1
