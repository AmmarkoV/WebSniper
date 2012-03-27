#!/bin/bash 

  if [ -e /usr/bin/WebSniper  ]
  then
    sudo rm  /usr/bin/WebSniper 
  else  
   #nothing to do :P
  fi

  if [ -e /usr/share/icons/websnipericon.png ]
  then
    sudo rm  /usr/share/icons/websnipericon.png
  else 
   #nothing to do :P
  fi

  if [ -e /usr/share/applications/WebSniper.desktop ]
  then
    sudo rm  /usr/share/applications/WebSniper.desktop 
  else 
   #nothing to do :P
  fi

if [ -d "/usr/share/websniper" ]; then
  echo "WebSniper Installation detected , uninstalling it "  
  sudo rmdir -r /usr/share/websniper
else 
   #nothing to do :P
fi 

if [ -d "~/.WebSniper" ]; then
  echo "WebSniper Installation detected in home , uninstalling it "  
  sudo rmdir -r ~/.WebSniper
else 
   #nothing to do :P
fi 
  
 
  echo "Done" 
exit 0
