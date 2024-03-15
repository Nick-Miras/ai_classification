import subprocess

desired_ip = "192.168.254.10"
desired_subnet = "255.255.255.0"
desired_gateway = "192.168.254.1"
# This would require elevated privileges (sudo) and wouldn't be recommended in most cases
subprocess.run(["ifconfig", "wlan0", "down"])  # Bring down the interface
subprocess.run(["ifconfig", "wlan0", "hw", "up"])  # Bring it back up
subprocess.run(["ifconfig", "wlan0", "inet", desired_ip, "netmask", desired_subnet])
subprocess.run(["route", "add", "default", "gw", desired_gateway])
 