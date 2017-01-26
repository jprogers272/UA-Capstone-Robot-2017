# UA-Capstone-Robot-2017

To configure BeagleBone with 4.4.x kernel for use on our robot or with our code:
  1. Add the file from this repository, other/cape-universalh-00A0.dts to /opt/source/bb.org-overlays/
  2. Inside the directory /opt/source/bb.org-overlays/, run ./install.sh
  3. In /boot/uEnv.txt, uncomment the line (near the top):
      dtb=am335x-boneblack-emmc-overlay.dtb
  4. In /boot/uEnv.txt, add the line:
      cape_enable=bone_capemgr.enable_partno=cape-universalh
  5. reboot

Networking:
  To connect to both wifi and ethernet, just wait until the Beaglebone is on to plug in the ethernet cable.
  To connect to wifi with connman, edit /var/lib/connman/settings to contain the proper router settings. For us, that is ssid=NETGEAR64, pass=livelycream499, security=wpa2-psk
  Then run the following commands:
    systemctl restart connman.service
    sudo connmanctl
    tether wifi disable
    enable wifi
    scan wifi
    services
    agent on
    connect wifi_*_*_managed_psk (there may be multiple choices: choose the one that is probably the NETGEAR64)
    quit
