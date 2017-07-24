#!/usr/bin/sudo python
# coding: utf8
import os
import shutil
import sys
import subprocess
from subprocess import call, check_output
import shlex

def installRequirements():
    call(["apt-get", "update"])
    call(["apt-get", "upgrade"])
    call(["apt-get", "install", "iw"])
    call(["apt-get", "install", "dnsmasq"])
    call(["apt-get", "install", "hostapd"])


temp_dir = "/opt/WlanSkript"
hotspot_dir = temp_dir + "/hotspot"
client_dir = temp_dir + "/client"
network_file = "interfaces"
dhcp_file = "dhcpcd.conf"

client_network_string = "auto lo\n" \
                        "iface lo inet loopback\n\n" \
                        "iface eth0 inet manual\n\n" \
                        "allow-hotplug wlan0\n" \
                        "iface wlan0 inet manual\n" \
                        "\twpa-conf /etc/wpa_supplicant/wpa_supplicant.conf\n\n" \
                        "allow-hotplug wlan1\niface wlan1 inet manual\n" \
                        "\twpa-conf /etc/wpa_supplicant/wpa_supplicant.conf"
hotspot_network_string = "# Localhost\n" \
                         "auto lo\n" \
                         "iface lo inet loopback\n\n" \
                         "# Ethernet\n" \
                         "auto eth0\n" \
                         "iface eth0 inet manual\n\n" \
                         "# WLAN-Interface\n" \
                         "allow-hotplug wlan0\n" \
                         "iface wlan0 inet static\n" \
                         "address 192.168.1.1\n" \
                         "netmask 255.255.255.0"

dnsmasq_append_string = "\n#DHCP-Server aktiv für Wlan-Interface\n" \
                        "interface=wlan0\n\n" \
                        "#DHCP-Server nicht aktiv für bestehendes Netzwerk\n" \
                        "no-dhcp-interface=eth0\n\n" \
                        "#IPv4-Adressbereich und Lease-Time\n" \
                        "dhcp-range=192.168.1.100,192.168.1.150,24h\n\n" \
                        "#DNS\n" \
                        "dhcp-option=option:dns-server,192.168.1.1"

hostapd_string = "# WLAN-Router-Betrieb\n\n" \
                 "# Schnittstelle und Treiber\n" \
                 "interface=wlan0\n" \
                 "#driver=nl80211\n\n" \
                 "# WLAN-Konfiguration\n" \
                 "ssid=wlanName\n" \
                 "channel=1\n" \
                 "hw_mode=g\n" \
                 "ieee80211n=1\n" \
                 "ieee80211d=1\n" \
                 "country_code=DE\n" \
                 "wmm_enabled=1\n\n" \
                 "# WLAN-Verschlüsselung\n" \
                 "auth_algs=1\n" \
                 "wpa=2\n" \
                 "wpa_key_mgmt=WPA-PSK\n" \
                 "rsn_pairwise=CCMP\n" \
                 "wpa_passphrase=passwordName"

print "Skript started!\n"

continueString = raw_input("Do you run this script for the first time? (y/n) ")
if continueString.lower() == "y" or continueString.lower() == "yes":
    installRequirements()

if not os.path.exists(temp_dir):
    print "Create /opt/WlanSkript to save my backup-files.\n" \
          "The script will override these files everytime you run it: \n" \
          "\t/etc/network/interfaces \n" \
          "\t/etc/dhcpcd.conf \n" \
          "\t/etc/dnsmasq.conf \n" \
          "\t/etc/hostapd/hostapd.conf \n" \
          "\t/etc/default/hostapd\n" \
          "If you do any changes to these files and want them to stay, " \
          "change them in the backup-folder of this script!\n\n"
    os.makedirs(temp_dir)

if not os.path.exists(client_dir):
    os.makedirs(client_dir)
    networkInterfaces = open(client_dir + "/" + network_file, "w")
    networkInterfaces.write(client_network_string)
    networkInterfaces.close()

    shutil.copy2("/etc/dhcpcd.conf", client_dir)
    dhcpFile = open(client_dir + "/" + dhcp_file, "r+")
    dhcpString = dhcpFile.read();
    delIndex = dhcpString.find("denyinterfaces wlan0")
    if delIndex != -1 and dhcpString[delIndex-1] != "#" and dhcpString[delIndex-2] != "#":
        dhcpString = dhcpString[:delIndex] + "#" + dhcpString[delIndex:]
        dhcpFile.close()
        dhcpFile = open(client_dir + "/" + dhcp_file, "w")
        dhcpFile.write(dhcpString)
        dhcpFile.close()

if not os.path.exists(hotspot_dir):
    os.makedirs(hotspot_dir)
    #prepare /etc/network/interfaces backup
    networkInterfaces = open(hotspot_dir + "/" + network_file, "w");
    networkInterfaces.write(hotspot_network_string)
    networkInterfaces.close()

    #prepare /etc/dhcpcd.conf backup
    shutil.copy2(client_dir + "/" + dhcp_file, hotspot_dir)
    dhcpFile = open(hotspot_dir + "/" + dhcp_file, "r+")
    dhcpString = dhcpFile.read();
    delIndex = dhcpString.find("#denyinterfaces wlan0")
    if delIndex != -1:
        dhcpString = dhcpString[:delIndex] + dhcpString[delIndex+1:]
        dhcpFile.close();
        dhcpFile = open(hotspot_dir + "/" + dhcp_file, "w")
        dhcpFile.write(dhcpString)
        dhcpFile.close()

    #prepare /etc/dnsmasq.conf backup
    shutil.copy2("/etc/dnsmasq.conf", hotspot_dir)
    dnsmasqFile = open(hotspot_dir +"/" + "dnsmasq.conf", "r+")
    dnsmasqString = dnsmasqFile.read()
    if dnsmasq_append_string not in dnsmasqString:
        dnsmasqFile.seek(0,2)
        dnsmasqFile.write(dnsmasq_append_string)
    dnsmasqFile.close()

    #prepare /etc/hostapd/hostapd.conf backup
    hostapdFile = open(hotspot_dir + "/" + "hostapd.conf", "w+")
    robotNumber = raw_input("Number of Robot? ")
    hostapd_string = hostapd_string.replace("wlanName", "Robot" + robotNumber)
    hostapd_string = hostapd_string.replace("passwordName", "Roboter" + robotNumber)
    hostapdFile.write(hostapd_string)
    hostapdFile.close()

    #prepare /etc/default/hostapd
    shutil.copy2("/etc/default/hostapd", hotspot_dir)
    hostapdFile = open(hotspot_dir + "/hostapd", "a+")
    if not 'DAEMON_CONF="/etc/hostapd/hostapd.conf"' in hostapdFile.read():
        hostapdFile.write('\nDAEMON_CONF="/etc/hostapd/hostapd.conf"')
    hostapdFile.close()

print "Done preparing backup-files!\n"

continueString = raw_input("This script will override some of your files. Changes you might have done to these "
                           "files will be gone. Continue? (y/n) ")
if not continueString.lower() == "y" and not continueString.lower() == "yes":
    sys.exit(0);

mode = raw_input("Do you want to enable hotspot or client? (hotspot/client) ")
if mode.lower() == "client":
    #replacing /etc/network/interfaces
    print "Replacing /etc/network/interfaces ..\n"
    os.remove("/etc/network/interfaces")
    shutil.copy2(client_dir + "/" + network_file, "/etc/network")

    #replacing /tec/dhcpcd.conf
    print "Replacing /etc/dhcpcd.conf ..\n"
    os.remove("/etc/dhcpcd.conf")
    shutil.copy2(client_dir + "/" + dhcp_file, "/etc")

    #asking for new wifi-connection
    newConnect = raw_input("Do you want to add a new Wlan to connect with? (y/n) ")
    if (newConnect.lower() == "y" or newConnect.lower() == "yes"):
        wifiName = raw_input("SSID? ")
        wifipassword = raw_input("Password? ")
        #call(["sudo", "-i"])
        call(["wpa_passphrase", wifiName, wifipassword, ">>" "/etc/wpa_supplicant/wpa_supplicant.conf"])
        #call(["exit"])

    #deactivate dnsmasq when booting
    print "Deactivating dnsmasq when booting..\n"
    call(["systemctl", "disable", "dnsmasq"])

    #deactivate hostapd when booting
    print "Deactivating hostapd when booting..\n"
    call(["systemctl", "disable", "hostapd"])

elif mode.lower() == "hotspot":
    #check if AP-mode is possible with this Pi
    print "Checking if hotspot is possible..\n"
    try:
        checkAP = check_output(["iw", "list"])
    except subprocess.CalledProcessError as grepexc:
        print "error code", grepexc.returncode, grepexc.output
        
    if "AP" in checkAP:
        print "Hotspot is possible!\n"
    else:
        print "Hotspot not possible.\n"
        sys.exit(1)

    # check if dhcpcd is active
    checkDhcpcd = check_output(["systemctl", "status", "dhcpcd"])
    if "inactive" in checkDhcpcd:
        print "dhcpcd is inactive. Please activate it before running this script."
        sys.exit(1)

    #replacing /etc/network/interfaces
    print "Replacing /etc/network/interfaces ..\n"
    os.remove("/etc/network/interfaces")
    shutil.copy2(hotspot_dir + "/" + network_file, "/etc/network")

    #replacing /etc/dhcpcd
    print "Replacing /etc/dhcpcd ..\n"
    os.remove("/etc/dhcpcd.conf")
    shutil.copy2(hotspot_dir + "/" + dhcp_file, "/etc")

    #replacing /etc/dnsmasq.conf
    print "Replacing /etc/dnsmasq.conf .. \n"
    os.remove("/etc/dnsmasq.conf")
    shutil.copy2(hotspot_dir + "/" + "dnsmasq.conf", "/etc")

    #restart dnsmasq
    call(["systemctl", "restart", "dnsmasq"])
    #start dnsmasq when booting
    call(["systemctl", "enable", "dnsmasq"])

    #replacing /etc/hostapd/hostapd.conf
    print "Replacing /etc/hostapd/hostapd.conf ..\n"
    os.remove("/etc/hostapd/hostapd.conf")
    shutil.copy2(hotspot_dir + "/" + "hostapd.conf", "/etc/hostapd")
    #only root has access to this file, because of wifipassword
    call(["chmod", "600", "/etc/hostapd/hostapd.conf"])

    #replacing /etc/default/hostapd
    print "Replacing /etc/default/hostapd ..\n"
    os.remove("/etc/default/hostapd")
    shutil.copy2(hotspot_dir + "/" + "hostapd", "/etc/default")

    #starting hostapd
    print "Starting hostapd..\n"
    call(["systemctl", "start", "hostapd"])
    call(["systemctl", "enable", "hostapd"])

#reboot
reboot = raw_input("The system needs a reboot to take all the changes. Reboot now? (y/n) ")
if (reboot.lower() == "y" or reboot.lower() == "yes"):
    call(["reboot"])
