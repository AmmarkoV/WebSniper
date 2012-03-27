#!/bin/bash
echo "Installation needs super user privileges.."
sudo echo "..." 
 
if [ -d "/usr/share/websniper" ]; then
  echo "WebSniper Installation detected , patching it up :)" 
else
  echo "Installing WebSniper in the system.. :)" 
  sudo mkdir /usr/share/websniper
fi

if [ -d "/usr/share/websniper/Sounds" ]; then
  echo "WebSniper Installation detected , patching it up :)" 
else
  echo "Installing WebSniper in the system.. :)" 
  sudo mkdir /usr/share/websniper/Sounds
fi 

  ./make




  if [ -e WebSniper ]
  then
    echo "WebSniper GUI App is OK :) , including it to system binaries .."
    sudo cp WebSniper /usr/bin/WebSniper 
  else
    echo "WebSniper GUI App could not be installed , you probably got a library missing"
    exit 1
  fi
 
  sudo cp Sounds/* /usr/share/websniper/Sounds
  sudo cp Screenshots/logo.png /usr/share/icons/websnipericon.png
  sudo cp WebSniper.desktop /usr/share/applications/WebSniper.desktop
   
  echo "Done" 
exit 0
