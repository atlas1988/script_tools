#!/bin/bash
#get the path of this file 
#this just for the command path
#temp_path=$(pwd)
temp_path=$(cd $(dirname ${BASH_SOURCE[0]}); pwd )
#echo "define a variable in this shall file"
SCP=$temp_path
#echo "$temp_path"
echo "export variable SCP for current terminal"
export SCP
echo "Set the startup environment variable SCP."
sudo sh -c 'echo "##setting the script_tools path of atlas" >> /etc/profile'
sudo sh -c "echo \"export SCP=$SCP\" >> /etc/profile"
