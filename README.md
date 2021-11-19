# STM32F7_LXI_Device
STM32 based LXI Device using Ethernet, LwIP, httpd, SCPI

[![Build Status](https://jenkins.kaon.ch/buildStatus/icon?job=STM32f7+TCPI+VXI+Device&build=1)](https://jenkins.kaon.ch/job/STM32f7%20TCPI%20VXI%20Device/1/)
---

- `stm32f746zg` (Nucleo)
- STM32CubeIDE

### Working

- ✅ DHCP IP Address Management
    + MCU continues to run even without active Ethernet connection
    + MCU connects to Network when pluged in via Ethernet
- ✅ http web interface
    + ✅ has switches to turn on/off configurations
    + ✅ uses CGI/SSI
    + ⚠️ mandatory `/lxi/identification.xml` is present (but not detected by the LXI tool)
- ✅ custom physical MAC address from within firmware
    + ✅ unique EUI48 address is read from EEPROM
- ✅ EEPROM to save/load user settings (IP, DHCP config ...)
- ✅ Settings allow to enable/disable DHCP + AutoIP or manual settings (IP/Netmask/Gateway)

---

### Sources

- How to make Ethernet and lwIP working on STM32 [](https://community.st.com/s/question/0D50X0000BOtfhnSQB/how-to-make-ethernet-and-lwip-working-on-stm32)
- Tutorial [HTTPd web-server on STM32 with SSI](http://ausleuchtung.ch/stm32-nucleo-f767zi-web-server/)
- STM32F7 [LwIP_TCP_Echo_Server](https://github.com/STMicroelectronics/STM32CubeF7/tree/master/Projects/STM32756G_EVAL/Applications/LwIP/LwIP_TCP_Echo_Server)
- STM32H7 [LwIP_UDP_Echo_Server](https://github.com/STMicroelectronics/STM32CubeH7/blob/master/Projects/STM32H743I-EVAL/Applications/LwIP/LwIP_UDP_Echo_Server/Src/udp_echoserver.c)
- [SCPI parser](https://www.jaybee.cz/scpi-parser/) Library
- List of [LXI Ports & Protocols](https://www.lxistandard.org/About/LXI-Protocols.aspx)
- UDP/Portmap Identification Broadcast example packet: [liblxi](https://github.com/lxi-tools/liblxi/blob/master/src/vxi11.c#L57)
- Correct SSI API application: [ssi_example.c](https://github.com/particle-iot/lwip/blob/master/contrib/examples/httpd/ssi_example/ssi_example.c)

### Tools

- official [LXI Discovery](https://www.lxistandard.org/About/LXI-Discovery-Tools.aspx) Tool
    + This sends out a `UDP/RPC/Portmap` broadcast with the `GETPORT` command
    + the LXI device must reply to this broadcast in `rcp_server.c`
    + Then the tool requests the `http://<host>/lxi/identification.xml` from the device
- [lxi-tools](https://lxi-tools.github.io/)
    + This sends out a `UDP/RPC/Portmap` broadcast with the `GETPORT` command
    + according to wireshark, the device does not reply --> how does it work then?
    + The tool will attempt to connect to the device via VXI-11 and `*IRQ?` the name
- [Wireshark](https://www.wireshark.org/)

---

### Todo

| ❌ | 🔄 | ⚠️ | ✅ |
|:---:|:---:|:---:|:---:|
| Todo | WIP | Debug (broken) | Done (working) |

#### Todo Critical

- 🔄 [Feature] make dynamic version of `/lxi/identification.xml` with appropriate SSI implementation
    + https://www.nongnu.org/lwip/2_0_x/group__httpd.html
    + (?) reverse changes in `httpd.c` because SSI can be controlled by makefsdata
    + (?) since the xml file contains `<!--comments-->` the LXI Discovery tool does not recognize the device anymore
    + ⚠️ LXI Identification still unclear, lxi-tools vs. LXI Identification Tool do not behave the same
    + Assertion "sys_timeout: timeout != NULL, pool MEMP_SYS_TIMEOUT is empty" failed at line 190 in ../Middlewares/Third_Party/LwIP/src/core/timeouts.c

#### Todo Whishlist

- ❌ [Feature] function to check web connection status
    + or callback handlers when connected / disconnected
- ❌ [reliability] Add further `ASSERT()` statements throughout the code (e.g. for SSI)
- ❌ [refactor] author/licence/description header for each file
- 🔄 [refactor] cleanup spaghetti code of global variables, introduce hierarchy of config headers
- ❌ [refactor] implement clear MVC structure for SCPI commands
- ❌ [Feature] add hislip compatibility
- ❌ [refactor] refactor/rename scpi_server.c to tcp/ip (does this have a name? VXI?)
- ❌ [reliability] use MUTEX for UART ringbuffer?
- ❌ [reliability] implement a memory monitor (FreeRTOS?) to check on heap/stack/RAM usage
- ❌ [refactor] use SCPI_Result**** API as return throughout scpi User Code
- ❌ [Feature] enable https within lwip
- ❌ [Feature] enable web login
    + probably not supported by LwIP, requires form post and session creation using a cookie in the post header
    + password is stored on EEPROM
    + redesign of web interface is required



---

### Documentation

- exclude from build:
    + `fs_data.c`
    + `fs_data_custom.c`

#### Helper Libraries

- `lwip/ip4_addr.h` parse and verify IPv4 addresses ([documentation](https://www.nongnu.org/lwip/2_0_x/ip4__addr_8h.html))

- `scpi/parser.h` parse and extract parameters from ascii ([documentation](https://www.jaybee.cz/scpi-parser/api/))

#### Features

- Webinterface (http / port 80)
    + http://192.168.1.179/
    + http://192.168.1.179/lxi/identification 

#### File Structure

`Core/Src` and `Core/Inc`

- `scpi-def.c`
    + SCPI commands definition
    + SCPI command callback functions
    + command parsing, argument preprocessing + sanitization
- `stm32f7xx_it.c`
    + USART Interrupt for UART SCPI
- `http_cgi_app.c`
    + CGI + SSI implementation for the httpd Webserver
    + SSI works for both the Website and the `identification.xml`
- `rpc_server.c`
    + implements the Sun-RPC Protocol based on UDP (more specifically the `Portmap` protocol)
    + When a UDP Broadcast (IP address: `xxx.xxx.xxx.255`) is received, the LXI device must answer correctly

`Tools/fs`

- Filesystem for the Webserver
- use any program `makefsdata` (perl, C, ...) to convert the file System to a C source file `fs_data_custom.c`
- `fs_data_custom.c` must be copied to `./Middlewares/Third_Party/LwIP/src/apps/http` after re-generation

`LWIP/App`

- `lwip.c` 
    + initialization with DHCP
    + initialization with manual IP

`LWIP/Target`

- `LWIP/Target/ethernetif.c` contains some User Code to set the PHY/MAC address

- `eeprom_24aa.h` contains macros to store and retreive data (e.g. manual IP config) in EEPROM


#### Webserver

##### SSI CGI

`#define LWIP_HTTPD_MAX_TAG_NAME_LEN 8`
`#define LWIP_HTTPD_MAX_TAG_INSERT_LEN 192`

































