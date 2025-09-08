#!/bin/bash

TOPLEVEL=$(git rev-parse --show-toplevel)

if [ ! -e "$TOPLEVEL/.venv" ] ; then
    python3 -m venv $TOPLEVEL/.venv
	
	source $TOPLEVEL/.venv/bin/activate
    
	pip install
	pre-commit install
fi

source "$TOPLEVEL/.venv/bin/activate"