

This device or software is not LXI certified or guaranteed to be compatible in any form.

> "LXI" is a trademark of the LXI Consortium, Inc. All rights reserved.

# LXI 1.5 compatibility

| ❌ | 🔄 | ⚠️ | ✅ |
|:---:|:---:|:---:|:---:|
| Todo | WIP | Debug (broken) | Done (working) |



### 1.4 Applicable Standards and Documents

- 1.4.4 LXI Device Specification and Extended Functions 
	+ 1.4.4.2 Conformance requirements (must conform with all of v1.5)
		* 1.4.4.2.1 LXI Device Specification Conformance Requirements (also add. rules e.g. IVI etc.)
- 1.4.5 Functional Declaraion (declare in datasheet that it conforms to 1.4.4.2)
- 1.4.6 Web Indication of Functional Declaration (also declared on web interface `1.5 LXI Device Specification 2016`)
- 1.4.7 Terms using the LXI Trademark

## 2 LXI Physical Specifications

### 2.4 Electrical Standards

- 2.4.5 LAN Configuration Initialize (LCI)
	+ 2.4.5.1 LCI Mechanism ("LAN RESET" menu entry that, when activated, places its network settings in a default state)
	+ 2.4.5.2 LXI Devices without a front-panel manual data-entry method (button instead of menu)
	+ 2.4.5.3 LCI Mechanism Protection (by a time-delay, user query, or mechanical protection feature)
- 2.4.9 LAN Connectors
	* 2.4.9.1 - IEEE 802.3 compliant

### 2.5 Status Indicators
- 2.5.1 Power Indicator 
	+ 2.5.1.1 Power Indicator (off - solid orange - solid green)
- 2.5.2 LAN Status Indicator 
	+ 2.5.2.1 LAN Status Indicator (solid green - flashing green - solid red)

### 2.6 LXI Device and Documentation Labeling Requirements

- 2.6.1.1 RULE – Front Panel Labeling Requirements 

## 3 LXI Device Synchronization and Events

### 3.5 LXI Event Handling
- 3.5.1 Measurement-related Functions Initiated by LXI Events (any meas-related function via the IVI driver shall also be executable from within the LXI Device)
	+ 3.5.1.1 Recommendation – Include Conventional Triggers (10 MHz rate may be impractical to handle in software)
	+ 3.5.1.3 Specify Trigger Response Times
	+ 3.5.2.1 Specify Trigger Output Response Times

### 3.7 Internal Log File for Events
- timestamp and event for all TCP/UDP calls
- timestamp is IEEE1588 compliant or 0
- enable / disable from driver

### 6.1 IVI Driver Requirement (All LXI Devices shall provide an IVI Specific Driver)

- 6.1.1 Trigger and Event Required API (shall conform to the IVI-3.15 IviLxiSync specification when required by an Extended Function)

### 6.2 Syntax of the Device Address (VISA resource names)
- `TCPIP[board]::host address[::LAN device name][::INSTR] `
- `TCPIP[board]::host address::port::SOCKET11 `
- `TCPIP[board]::host address[::HiSLIP device name[,HiSLIPport]][::INSTR]`

### 6.3 IVI Property for Referencing a Signal Source
shall have a property of type BSTR named Source, or ending in Source

### 6.4 Eight LXI Events for Arm/Trigger and Eight for LXI Event Messages
- 6.4.2 RULE – 3.15 IviLxiSync API Routes Events to LAN 
- 6.4.3 RULE – LXI Events Encode the Sense of the Event in Packet 
- 6.4.4 RULE – Standard Strings Used to Designate Events 
	+ 6.4.4.1 RULE – Only Signals Corresponding to Implemented Capability Required 
	+ 6.4.4.2 RULE – Devices Shall Document Supported Signals 
- 6.4.5 RULE – LXI Event Names Beginning with LXI Reserved 
- 6.4.6 RULE – Destination Path Syntax 

### 6.5 RULE – API Shall Represent Time as Two 64-bit Floats
- 6.5.1 RULE – Property Names for Real-Time Representation 
- 6.5.2 RULE – Property Names for Real-Time Timestamp 

### 6.6 RULE – Domain Property to Facilitate Multiple Systems on a Single LAN

## 7 LAN Specifications

### 7.1 Ethernet Required
 a minimum of 100 Mbits/second, IEEE 802.3 Type 100 BASE-TX.   

- 7.1.2 Proper Operation in Slower Networks (10 10Mbits/second)

### 7.2 MAC Address Display (display or label)

### 7.3 Ethernet Connection Monitoring (DHCP)

### 7.5 Label Required on LXI Devices Without Auto-MDIX

### 7.6 Enable Auto-Negotiation by Default

### 7.7 Multiple LAN Interfaces

## 8 IPv4 LAN Configuration

### 8.1 TCP/IP, UDP, IPv4 Network Protocols
### 8.2 ICMP Ping Responder
### 8.3 ICMP Ping Responder Enabled by Default
### 8.6 IP Address Configuration Techniques (DHCP, Auto-IP, manual)
- 8.6.1 Options for LAN configuration (auto vs. manual)
- 8.6.3 Explicitly Request All Desired DHCP Parameters (gateway + mask)
- 8.6.5 Do Not Require Additional DHCP Options for Normal Operations 
- 8.6.6 Stop Using IP Address If DHCP Lease Not Renewed (error message)
- 8.6.7 Honor New DHCP Options at Lease Renewal 
- 8.6.10  RULE – RFC 3927 
### 8.7 Duplicate IP Address Detection
### 8.10 Provide an Error Indicator for LAN Configuration Faults
### 8.13 LAN Configuration Initialize (LCI) (reset / reboot automatically without confirmation)


## 9 Web Interface 
### 9.1 Web Pages Using W3C Compliant Browsers
- 9.1.1 Protocol and Port Number (http:80)
### 9.2 Welcome Web Page Display Items
- Items (read only)
	+ LXI Device Model 
	+ Manufacturer 
	+ Serial Number 
	+ Description12 
	+ LXI Extended Functions 
	+ LXI version 
	+ Hostname13 
	+ MAC Address <XX-XX-XX-XX-XX-XX>
	+ TCP/IP Address <DDD.DDD.DDD.DDD>
	+ Firmware and/or Software Revision 
	+ LXI Device Address String [VISA] 
- 9.2.1 LXI Device Address String on Welcome Page 
	+ `TCPIP[board]::host address::port::SOCKET `
- 9.2.2 Recommendation – Web Page Title 
	+ LXI – Manufacturer-Model-<Optional Serial Number>-<Optional Description> 
- 9.2.3 Actual Hostname Display (DNS/mDNS)
	+ 9.2.3.1 Recommendation - How To Determine Actual Hostname with Unicast DNS 
	+ 9.2.3.2 Hostname Display (instead show the assigned IP address or a blank field for the hostname)
	+ 9.2.3.3 mDNS Hostname Format 
### 9.3 Device Identification Functionality on the Web Page (control the LAN Status Indicator)
### 9.4 control the LAN Status Indicator (two hyperlinks/buttons, user to configure LXI Device settings)
### 9.5 LAN Configuration Web Page Contents
- Fields:
	+ Hostname 
	+ Description 
	+ TCP/IP Configuration Mode 
	+ Static IP address 
	+ Subnet mask 
	+ Default Gateway 
	+ DNS Server(s) 
- 9.5.1 Recommendation – Default Description for LXI Device
- 9.5.7 Reverting Hostname to Factory Default 
- 9.5.8 Reverting Device Description to Factory Default 

### 9.6 Sync Configuration Web Page Contents 
### 9.7 Recommendation – Status Web Page Contents 
### 9.8 Web Page Security (page(s) that allows user to change the instrument’s settings shall be password protected)
### 9.9 LXI Logo

## 10 LAN Discovery and Identification 
### 10.2 XML Identification Document
- 10.2.2 Content Type Header 
- 10.2.3 Schema Location Attribute 
- 10.2.4 Connected Device URLs 
	+ 10.2.4.1 Connected Device XML Identification Document URLs 
	+ 10.2.4.2 RULE – Connected Device XML Identification Document Schema Location Attribute
- 10.2.5 LXI Extended Function Elements 
### 10.3 Support mDNS
- 10.3.1 RULE – Claiming Hostnames 
	+ 10.3.1.1 RULE – Hostname Conflicts 
- 10.3.3 RULE – Dynamic DNS Update and mDNS Hostname 
- 10.3.4 RULE – DHCP “Host Name” Option and mDNS Hostname 
### 10.4 RULE – Support mDNS Service Discovery
- 10.4.1 RULE – Claiming Service Name 
- 10.4.2 RULE – Single Service Instance Name for LXI Defined Services 
	+ 10.4.2.1 RULE – User Configurable Service Name 
	+ 10.4.2.3 RULE – Service Name Conflicts 
- 10.4.3 Rule - Required Service Advertisements and TXT Record Keys 
	+ 10.4.3.1 RULE – TXT Records Are Required 
	+ 10.4.3.2 RULE – TXT Records Consist of Key/Value Pairs 
	+ 10.4.3.3 RULE – TXT Record Keys Are Case-Insensitive ASCII 
	+ 10.4.3.4 RULE – TXT Record Values 
	+ 10.4.3.6 RULE – TXT Record Key Order 
	+ 10.4.3.7 RULE – LXI Consortium TXT Record Keys 
	+ 10.4.3.8 RULE – Vendor Defined TXT Record Keys 
	+ 10.4.3.11 RULE – Service Advertisement Order 
### 10.5 RULE – mDNS and DNS-SD Enabled by Default
- 10.5.1 RULE – mDNS and DNS-SD Enabled by LAN Configuration Initialize (LCI)
### 10.6 RULE – mDNS Name Resolution
### 10.7 RULE – Nonvolatile Hostnames and Service Names
### 10.8 RULE – Link Changes 

## 11 Documentation 
### 11.1 RULE – Full Documentation on IVI Interface 
### 11.2 RULE – Registration of the IVI Driver 
### 11.3 Recommendation – Documentation on LXI Device Web Page


