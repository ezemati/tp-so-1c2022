#!/bin/bash
TP_DIR=$PWD
PROJECTS_DIR=~/workspace

cd $PROJECTS_DIR
echo -e "\n\nInstalling commons libraries...\n\n"
COMMONS="so-commons-library"
git clone "https://github.com/sisoputnfrba/${COMMONS}.git" $COMMONS
cd $COMMONS
sudo make uninstall
make all
sudo make install

cd $TP_DIR
echo -e "\n\nBuilding projects...\n\n"
make clean -C ./consola
make clean -C ./kernel
make clean -C ./cpu
make clean -C ./memoria
make -C ./consola
make -C ./kernel
make -C ./cpu
make -C ./memoria
echo -e "\n\nDeploy done!\n\n"
