import time
import wifi
import ipaddress
from secrets import secrets
ip_to_ping = "1.1.1.1"

wifi.radio.connect(ssid=secrets['ssid'],password=secrets['password'])

print("my IP addr:", wifi.radio.ipv4_address)
print("pinging ",ip_to_ping)
ip1 = ipaddress.ip_address(ip_to_ping)
while True:
    print("ping:", wifi.radio.ping(ip1))
    time.sleep(1)