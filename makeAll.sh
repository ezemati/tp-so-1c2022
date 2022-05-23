#!/bin/bash
echo -e "\n\nBuilding projects...\n\n"
make -C ./shared
make -C ./memoria
make -C ./cpu
make -C ./kernel
make -C ./consola
echo -e "\n\Build done!\n\n"