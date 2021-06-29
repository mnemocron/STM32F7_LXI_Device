# STM32F7_LXI_Device
STM32 based LXI Device using Ethernet, LwIP, httpd, SCPI

[![Build Status](https://jenkins.kaon.ch/buildStatus/icon?job=STM32f7+TCPI+VXI+Device&build=1)](https://jenkins.kaon.ch/job/STM32f7%20TCPI%20VXI%20Device/1/)
---

- `stm32f746zg` (Nucleo)
- STM32CubeIDE

### Working

- DHCP IP Address Management
    + MCU continues to run even without active Ethernet connection
    + MCU connects to Network when pluged in via Ethernet
- Telnet Echo Server on Port (23 / 5025)
    + `$ > telnet 192.168.1.180 5025`


---

### Sources

- How to make Ethernet and lwIP working on STM32 [](https://community.st.com/s/question/0D50X0000BOtfhnSQB/how-to-make-ethernet-and-lwip-working-on-stm32)
- Tutorial [HTTPd web-server on STM32 with SSI](http://ausleuchtung.ch/stm32-nucleo-f767zi-web-server/)
- STM32F7 [LwIP_TCP_Echo_Server](https://github.com/STMicroelectronics/STM32CubeF7/tree/master/Projects/STM32756G_EVAL/Applications/LwIP/LwIP_TCP_Echo_Server)
- STM32H7 [LwIP_UDP_Echo_Server](https://github.com/STMicroelectronics/STM32CubeH7/blob/master/Projects/STM32H743I-EVAL/Applications/LwIP/LwIP_UDP_Echo_Server/Src/udp_echoserver.c)
- [SCPI parser](https://www.jaybee.cz/scpi-parser/)
- [LXI Ports & Protocols](https://www.lxistandard.org/About/LXI-Protocols.aspx)

---

### Todo

- switching DHCP vs. Static IP not working
    + CPU crashes
    + Assertion "no packet queues allowed!" failed at line 1009 in ../Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.c
- implement clear MVC structure for SCPI commands
- make dynamic version of `/lxi/identification.xml` with appropriate SSI implementation (not possible because of file extension?) https://www.nongnu.org/lwip/2_0_x/group__httpd.html
    + this requries a hack inside the httpd.c code to add "xml" as supported SSI file --> working now
    + SSI / CGI information still missing



---

### Documentation

- exclude from build:
    + fs_data.c
    + fs_data_custom.c


#### Helper Libraries

- `lwip/ip4_addr.h` parse and verify IPv4 addresses ([documentation](https://www.nongnu.org/lwip/2_0_x/ip4__addr_8h.html))

- `scpi/parser.h` parse and extract parameters from ascii ([documentation](https://www.jaybee.cz/scpi-parser/api/))

#### Features

- Webinterface (http / port 80)
    + http://192.168.1.179/
    + http://192.168.1.179/lxi/identification 
- 

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

`Tools/fs`

- Filesystem for the Webserver
- use any program `makefsdata` (perl, C, ...) to convert the file System to a C source file

`LWIP/App`

- `lwip.c` 
    + initialization for DHCP
    + initialization with static IP

#### Webserver

##### SSI CGI

`#define LWIP_HTTPD_MAX_TAG_NAME_LEN 8`
#define LWIP_HTTPD_MAX_TAG_INSERT_LEN 192






























