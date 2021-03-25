# STM32F7_LXI_Device
STM32 based LXI Device using Ethernet, LwIP, httpd, SCPI

[![Build Status](https://jenkins.kaon.ch/buildStatus/icon?job=STM32f7+TCPI+VXI+Device&build=1)](https://jenkins.kaon.ch/job/STM32f7%20TCPI%20VXI%20Device/1/)
---

- `stm32f46zg` (Nucleo)
- STM32CubeIDE

### Working

- DHCP IP Address Management
    + MCU continues to run even without active Ethernet connection
    + MCU connects to Network when pluged in via Ethernet
- Telnet Echo Server on Port (23 / 5025)
    + `$ > telnet 192.168.1.180 5025`


---

### Sources

- Tutorial [HTTPd web-server on STM32 with SSI](http://ausleuchtung.ch/stm32-nucleo-f767zi-web-server/)
- STM32F7 [LwIP_TCP_Echo_Server](https://github.com/STMicroelectronics/STM32CubeF7/tree/master/Projects/STM32756G_EVAL/Applications/LwIP/LwIP_TCP_Echo_Server)
- [SCPI parser](https://www.jaybee.cz/scpi-parser/)
- [LXI Ports & Protocols](https://www.lxistandard.org/About/LXI-Protocols.aspx)

---

### Todo

- USART3 Rx Interrupt misses every other character (216 MHz / 19200 bd) --> use DMA with "\n" detection instead
- SCPI Library outputs to UART
    + add detection to output either to UART or TCPIP
    + manage TCPIP connections because they can be open/closed





