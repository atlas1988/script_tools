
#unzip platform-tools_r22-linux.zip -d ./
#sudo mv platform-tools /
#mkdir temp
#sudo cp /etc/profile temp/
echo $PATH
sudo sh -c 'echo "export PATH=/platform-tools:\$PATH" >> /etc/profile'
