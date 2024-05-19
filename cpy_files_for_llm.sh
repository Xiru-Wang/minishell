#!/usr/bin/env bash

echo "delete llm/ .c files"
rm ~/Documents/llm/*.c
echo "copy all src/**/*.c to llm/"
cp src/**/*.c ~/Documents/llm/
