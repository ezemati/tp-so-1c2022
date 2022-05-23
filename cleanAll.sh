#!/bin/bash
echo -e "\n\Cleaning projects...\n\n"
make clean -C ./shared
make clean -C ./memoria
make clean -C ./cpu
make clean -C ./kernel
make clean -C ./consola
echo -e "\n\Clean done!\n\n"