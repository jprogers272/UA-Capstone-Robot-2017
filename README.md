# UA-Capstone-Robot-2017

To configure BeagleBone with 4.4.x kernel for use on our robot or with our code:
  1. Add the file from this repository, other/cape-universalh-00A0.dts to /opt/source/bb.org-overlays/
  2. Inside the directory /opt/source/bb.org-overlays/, run ./install.sh
  3. In /boot/uEnv.txt, uncomment the line (near the top):
      dtb=am335x-boneblack-emmc-overlay.dtb
  4. In /boot/uEnv.txt, add the line:
      cape_enable=bone_capemgr.enable_partno=cape-universalh
  5. reboot
