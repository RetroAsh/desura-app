#!/bin/sh
# Determine the Linux distribution that is being run and install the build dependencies for it.

if [ "$(whoami)" != "root" ]; then
	echo "Run this as root! sudo should do the trick."
	exit

echo "NOTE: It'd be a good idea in general to read the contents of this script if you haven't already."
# Sounds scary, just to catch attention. Thanks for reading!

PASS_MESSAGE="Please provide root password to install build dependecies"
DISTRIBUTION="Unknown"
UNSUPPORTED_DISTROS="Unknown distribution."
UNSUPPORTED_MESSAGE=" is not supported yet, consult this script and figure out dependencies for it."

if [ -f /etc/SuSE-release ]; then
	DISTRIBUTION="SUSE"
elif [ -f /etc/debian_version ]; then
	DISTRIBUTION="Debian"
	if [ "$(whoami)" != 'root' ]; then
		echo $PASS_MESSAGE
	fi
	su -c 'apt-get install gcc git-core subversion m4 build-essential binutils automake autoconf libtool libgtk2.0-dev libnss3-dev libgconf2-dev libgnome-keyring-dev libdbus-glib-1-dev gperf bison libcups2-dev flex libjpeg-dev libasound2-dev libbz2-dev libxpm-dev libx11-dev libssl-dev libnotify-dev scons libv8-dev libc-ares-dev libboost-dev libboost-date-time-dev libboost-filesystem-dev libboost-system-dev libboost-thread-dev'
elif [ -f /etc/redhat-release ]; then
	DISTRIBUTION="Red Hat"
	if [ "$(whoami)" != 'root' ]; then
		echo $PASS_MESSAGE
	fi
	su -c 'yum install git subversion m4 autoconf gcc-c++ glibc-devel binutils autoconf libtool gtk2-devel nss-devel GConf2-devel libgnome-keyring-devel dbus-glib-devel gperf bison cups-devel flex libjpeg-turbo-devel alsa-lib-devel bzip2-devel libXpm-devel libX11-devel openssl-devel libnotify-devel scons'
elif [ -f /etc/arch-release ]; then
	DISTRIBUTION="Arch"
	if [ "$(whoami)" != 'root' ]; then
		echo $PASS_MESSAGE
	fi
	DEPS="$(pacman -T git subversion m4 autoconf gcc glibc binutils autoconf libtool gtk2 nss libgnome-keyring dbus-glib gperf bison cups flex libjpeg-turbo alsa-lib bzip2 libxpm libx11 openssl scons gconf libnotify boost c-ares v8 bzip2 tinyxml sqlite)"
	su -c 'pacman -S $DEPS'
elif [ -f /etc/slackware-version ]; then
	DISTRIBUTION="Slackware"
fi

echo $UNSUPPORTED_DISTROS| grep -q $DISTRIBUTION
if [ $? -eq 0 ];then
  echo $DISTRIBUTION $UNSUPPORTED_MESSAGE
fi
