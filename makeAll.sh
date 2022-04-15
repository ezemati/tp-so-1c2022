#!/bin/bash
echo -e "\n\nBuilding projects...\n\n"
make -C ./consola
make -C ./cpu
make -C ./kernel
make -C ./memoria
echo -e "\n\Build done!\n\n"