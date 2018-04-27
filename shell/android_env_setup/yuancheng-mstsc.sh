sudo apt-get -y install xrdp
sudo apt-get -y install vnc4server
#sudo apt-get -y install xubuntu-desktop
###echo "xfce4-session" >~/.xsession
echo "gnome-session --session=ubuntu-2d">~/.xsession
sudo service xrdp restart


