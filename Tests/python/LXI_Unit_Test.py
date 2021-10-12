# -*- coding: utf-8 -*-
"""
Created on Sat Mar 27 11:13:11 2021

@author: simon

*IDN?
:TEST:BOOL 1
:TEST:BOOL 0
:TEST5:NUM7
:TEST:TEXT "Hello"
:TEST:CHOICE? BUS

:SYST:COMM:TCPIP:IP <ipaddr>
:SYST:COMM:TCPIP:IP?
:SYST:COMM:TCPIP:MASK <ipaddr>
:SYST:COMM:TCPIP:MASK?
:SYST:COMM:TCPIP:GATE <ipaddr>
:SYST:COMM:TCPIP:GATE?
:SYST:COMM:TCPIP:DHCP 1
:SYST:COMM:TCPIP:DHCP?

:SYST:COMM:TCPIP:PHY?
"""

import pyvisa as visa
# import vxi11 as vxi
from time import sleep

DEVICE_IP = '192.168.1.179'
DEVICE_RSRC_STRING = 'TCPIP0::' + DEVICE_IP + '::5025::SOCKET'

# GPIB / VISA Resource Manager
rm = visa.ResourceManager()
print(rm.list_resources())

#%%
dut = rm.open_resource(DEVICE_RSRC_STRING)  # Visa Drivers over GPIB-USB Adapter
dut.timeout = 1000;
dut.read_termination = '\n'

pause = 0.1
sleep(pause)

# print(dut.query('*IDN?'))
# print(dut.write(':TEST:BOOL 1'))
# print(dut.query(':TEST:CHOICE? BUS?'))
print('LXI > ' + dut.query(':TEST:TEXT "Hello"'))
#print('LXI > ' + dut.query(':TEST5:NUM7'))
print('LXI > ' + dut.query(':SYST:COMM:TCPIP:DHCP?'))
print('LXI > ' + dut.query(':SYST:COMM:TCPIP:IP?'))
print('LXI > ' + dut.query(':SYST:COMM:TCPIP:GATE?'))
print('LXI > ' + dut.query(':SYST:COMM:TCPIP:MASK?'))
print('LXI > ' + dut.query(':SYST:COMM:TCPIP:PHY?'))

dut.close()

#%%
dut.close()

#%%
dut = vxi.Instrument(DEVICE_RSRC_STRING)
dut.timeout = 10000;
print(dut.ask('*IDN?'))

#%%
dut = rm.open_resource('ASRL9::INSTR')  # Visa Drivers over GPIB-USB Adapter
print(dut.query('*IDN?'))

