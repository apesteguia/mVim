#!/bin/bash

bin_directory="$(cd "$(dirname "$0")/program" && pwd)"

install_directory="/usr/local/bin"

if [ -x "$install_directory/msee" ]; then
    echo "msee is already installed in $install_directory"
else
    if [ -x "$bin_directory/msee" ]; then
        sudo mv "$bin_directory/msee" "$install_directory"
    
        sudo chmod +x "$install_directory/msee"
    
        echo "msee has been installed in $install_directory"
    else
        echo "Error: msee executable not found in the bin directory."
    fi
fi

