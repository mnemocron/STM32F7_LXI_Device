
# LXI Device Testing Procedures

--- 

- 2.4.5 LAN Configuration Initialize (LCI)
	+ 2.4.5.1 LCI Mechanism ("LAN RESET" menu entry that, when activated, places its network settings in a default state)
	+ 2.4.5.2 LXI Devices without a front-panel manual data-entry method (button instead of menu)
	+ 2.4.5.3 LCI Mechanism Protection (by a time-delay, user query, or mechanical protection feature)

Practically, this means:

- Pressing a button resets the Ethernet LAN without crashing the MCU.

---

Switching between DHCP and manual IP

- the IP config is
- the manual IP is stored when powered off
- in default DHCP mode, a manual IP can be set and the device adjusts to this IP without crashing
- from manual IP mode, the DHCP can be enabled again and the device negociates a new IP without crashing


---

:SYST:COMM:TCPIP:DHCP 0
:SYST:COMM:TCPIP:IP "192.168.1.173"
:SYST:COMM:TCPIP:MASK "255.255.255.0"
:SYST:COMM:TCPIP:GATE "192.168.1.1"


:MEM:IP?
:MEM:DHCP?
:MEM:MASK?
:MEM:GATE?






