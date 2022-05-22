#!/usr/bin/env bash
## this must be run as root!
## It should set up VMWare Shared folders on Ubuntu guests using open-vm-tools

## USAGE setup-hgfs-automount-ubuntu.sh [noauto]

rclocal="/etc/rc.local"
owner="root:root"
mountpoint="/mnt/hgfs"

## fail if we're not run as root
if [ "$EUID" != 0 ]
then
  (>&2 echo "$0 must be run as root")
  exit 127
fi

## set up FSTAB
mkdir -p $mountpoint
if [ "$1" == "noauto" ]
then
  echo ".host:/	${mountpoint}	fuse.vmhgfs-fuse	nauto,allow_other	0	0" >> /etc/fstab
else
  echo ".host:/	${mountpoint}	fuse.vmhgfs-fuse	auto,allow_other	0	0" >> /etc/fstab
  echo "You may now `mount ${mountpoint}`; it will also be auto-mounted on next boot"
  exit 0
  # if using the `auto` keyword, no rc.local setup is required, and we're done here.
fi

## set up rc.local script
if ! [ -f "$rclocal" ]
then
  echo '#!/bin/sh' > "$rclocal"
fi

echo "mount '$mountpoint'" >> "$rclocal"
chown $owner "$rclocal"
chmod 0755 "$rclocal"

## enable rc.local service
systemctl enable rc-local.service
systemctl start rc-local.service

## report and exit
(>&2 echo -e "set up and started $rclocal\nyou should be able to browse $mountpoint now and after reboots")
exit 0

