#!/bin/sh

if [ -e ".venv" ] ; then
	python3 -m venv .venv
	source .venv/bin/activate
	pip install pre-commit cpplint
	pre-commit install
fi
