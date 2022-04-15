#!/bin/bash
echo -e "\n\Cleaning projects...\n\n"
make clean -C ./consola
make clean -C ./cpu
make clean -C ./kernel
make clean -C ./memoria
echo -e "\n\Clean done!\n\n"