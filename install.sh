#!/bin/bash

bin_directory="$(cd "$(dirname "$0")/program" && pwd)"

install_directory="/usr/local/bin"

if [ -x "$install_directory/mvim" ]; then
    echo "mvim is already installed in $install_directory"
else
    if [ -x "$bin_directory/mvim" ]; then
        sudo mv "$bin_directory/mvim" "$install_directory"
    
        sudo chmod +x "$install_directory/mvim"
    
        echo "mvim has been installed in $install_directory"
    else
        echo "Error: mvim executable not found in the bin directory."
    fi
fi

