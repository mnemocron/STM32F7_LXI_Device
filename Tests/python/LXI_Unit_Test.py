# -*- coding: utf-8 -*-
"""
@file    LXI_Unit_Test.py
@brief   automated testing of the LXI features
@author  Simon Burkhardt
@date    2021-11-19
@version 0.0.1
@note    this test is for firmware version: v0.1.0 
"""

#%%
import traceback
import requests
import pyvisa as visa
from time import sleep

# SETTINGS
DEVICE_IP = '192.168.1.7'
DEVICE_GATEWAY = '192.168.1.1'
DEVICE_SUBNET = '255.255.255.0'
DEVICE_FW_VERSION = 'v0.1.0'
DEVICE_SERIAL_NO = 'AABBCCDD'
DEVICE_PHY_MAC = ''
DEVICE_DESCRIPTION = 'lxi demo'
DEVICE_MANUFACTURER = 'ETA SYSTEMS GMBH'

# test behaviour
TIMEOUT_WEB = 3
requests.adapters.DEFAULT_RETRIES = 3

# generate constants
DEVICE_TCP_STRING = 'TCPIP0::' + DEVICE_IP + '::5025::SOCKET'
DEVICE_USB_STRING = 'ASRL9::INSTR'
# DEVICE_RSRC_STRING = 'TCPIP0::' + DEVICE_IP + '::5555::SOCKET'
IDN_STRING = DEVICE_MANUFACTURER+','+DEVICE_DESCRIPTION+','+DEVICE_SERIAL_NO+','+DEVICE_FW_VERSION

DEVICE_URL = 'http://' + DEVICE_IP + ''
DEVICE_URL_404 = 'http://' + DEVICE_IP + '/bingbong'
DEVICE_URL_IDENT = 'http://' + DEVICE_IP + '/lxi/identification'
DEVICE_URL_IDENT_S = 'http://' + DEVICE_IP + '/lxi/identification/'

CGI_URL_P1 = DEVICE_URL + '/settings.cgi?p=1'
CGI_URL_P2 = DEVICE_URL + '/settings.cgi?p=2'
CGI_URL_P1P2 = DEVICE_URL + '/settings.cgi?p=1,p=2'

# define error messages
MSG_URL_200 = 'http ronse should be 200'
MSG_URL_404 = 'http ronse should be 404'
MSG_WEB_404 = 'website does not contain a "404" string'
MSG_WEB_FW = 'website does not contain correct firmware version string: ' +  DEVICE_FW_VERSION
MSG_WEB_SERIALNO = 'website does not contain correct serial number: ' + DEVICE_SERIAL_NO
MSG_WEB_DESCRIPTION = 'website does not contain correct device descriptor: ' + DEVICE_DESCRIPTION
MSG_WEB_RSRC_STRING = 'lxi identification contains wrong resource string: ' + DEVICE_TCP_STRING
MSG_SCPI_IDN = 'SCPI command *IDN? returned wrong string'

#%%
# PERFORM TEST
try:
    # test web interface
    r = requests.get(DEVICE_URL)
    assert r.status_code == 200, MSG_URL_200
    assert DEVICE_FW_VERSION.lower()   in r.text.lower(), MSG_WEB_FW
    assert DEVICE_SERIAL_NO.lower()    in r.text.lower(), MSG_WEB_SERIALNO
    assert DEVICE_DESCRIPTION.lower()  in r.text.lower(), MSG_WEB_DESCRIPTION
    
    # test 404 web page
    r = requests.get(DEVICE_URL_404, timeout=TIMEOUT_WEB)
    assert r.status_code == 404, MSG_URL_404
    assert '404' in r.text
    
    # test cgi functionality
    r = requests.get(CGI_URL_P1, timeout=TIMEOUT_WEB)
    assert r.status_code == 200, MSG_URL_200
    # assert '' in r.text # todo
    
    # test lxi xml document
    r = requests.get(DEVICE_URL_IDENT, timeout=TIMEOUT_WEB)
    assert r.status_code == 200, MSG_URL_200
    assert DEVICE_FW_VERSION.lower()   in r.text.lower(), MSG_WEB_FW
    assert DEVICE_SERIAL_NO.lower()    in r.text.lower(), MSG_WEB_SERIALNO
    assert DEVICE_DESCRIPTION.lower()  in r.text.lower(), MSG_WEB_DESCRIPTION
    assert MSG_WEB_RSRC_STRING.lower() in r.text.lower(), MSG_WEB_RSRC_STRING
    
    # test SCPI commands over TCP/IP
    rm = visa.ResourceManager()
    dut = rm.open_resource(DEVICE_TCP_STRING)  # Visa Drivers over GPIB-USB Adapter
    dut.timeout = 1000;
    dut.read_termination = '\n'
    pause = 0.1
    sleep(pause)
    
    # test IDN
    r = dut.query('*IDN?')
    assert IDN_STRING.upper() in r.upper(), MSG_SCPI_IDN
    # test phy
    r = dut.query(':SYST:COMM:TCPIP:PHY?')
    assert DEVICE_PHY_MAC.lower() in r.lower(), 'MAC addresses do not match'
    # test IP config
    r = dut.query(':SYST:COMM:TCPIP:IP?')
    assert DEVICE_IP in r, 'IP addresses do not match'
    r = dut.query(':SYST:COMM:TCPIP:GATE?')
    assert DEVICE_GATEWAY in r, 'Gateway addresses do not match'
    r = dut.query(':SYST:COMM:TCPIP:MASK?')
    assert DEVICE_SUBNET in r, 'Subnet mask addresses do not match'
    
    # test SCPI library tests
    r = dut.query(':TEST:BOOL 1')
    assert '1' in r, ':TEST:BOOL 1 does not return 1'
    r = dut.query(':TEST:BOOL 0')
    assert '1' in r, ':TEST:BOOL 0 does not return 0'
    
    r = dut.query(':TEST:NUM 7')
    assert '7' in r, ':TEST:NUM 7 does not return 7'
    r = dut.query(':TEST:NUM 123')
    assert '123' in r, ':TEST:NUM 123 does not return 123'
    
    r = dut.query(':TEST:TEXT "Hello, World!"')
    assert 'Hello, World!' in r, ':TEST:TEXT "Hello, World!" does not return correct text'
    
    dut.close()
    
    # test SCPI commands over USB-Serial
    dut = rm.open_resource(DEVICE_USB_STRING)  # Visa Drivers over GPIB-USB Adapter
    dut.timeout = 1000;
    dut.read_termination = '\n'
    pause = 0.1
    sleep(pause)
    
    r = dut.query('*IDN?')
    assert IDN_STRING.upper() in r.upper(), MSG_SCPI_IDN

except Exception:
    print(traceback.format_exc())
    try:
        dut.close()
    except Exception:
        pass

















#%%

import pyvisa as visa
# import vxi11 as vxi
from time import sleep


# GPIB / VISA Resource Manager
rm = visa.ResourceManager()
print(rm.list_resources())

#%%
dut = rm.open_resource(DEVICE_TCP_STRING)  # Visa Drivers over GPIB-USB Adapter
dut.timeout = 1000;
dut.read_termination = '\n'

pause = 0.1
sleep(pause)


print('LXI > ' + dut.query('*IDN?'))

#%%
# print(dut.query('*IDN?'))
# print(dut.write(':TEST:BOOL 1'))
# print(dut.query(':TEST:CHOICE? BUS?'))
print('LXI > ' + dut.query(':TEST:TEXT "Hello"'))
print('LXI > ' + dut.query(':TEST5:NUM7'))
print('LXI > ' + dut.query(':SYST:COMM:TCPIP:DHCP?'))
print('LXI > ' + dut.query(':SYST:COMM:TCPIP:IP?'))
print('LXI > ' + dut.query(':SYST:COMM:TCPIP:GATE?'))
print('LXI > ' + dut.query(':SYST:COMM:TCPIP:MASK?'))
print('LXI > ' + dut.query(':SYST:COMM:TCPIP:PHY?'))

dut.close()

#%%
dut = rm.open_resource(DEVICE_RSRC_STRING)  # Visa Drivers over GPIB-USB Adapter
dut.timeout = 1000;
dut.read_termination = '\n'

pause = 0.1
sleep(pause)

dut.write(':SYST:COMM:TCPIP:IP "192.168.1.173"')
dut.write(':SYST:COMM:TCPIP:MASK "255.255.255.0"')
dut.write(':SYST:COMM:TCPIP:GATE "192.168.1.1"')
dut.write(':SYST:COMM:TCPIP:DHCP 0')

dut.close()

#%%
dut.close()

#%%
dut = vxi.Instrument(DEVICE_TCP_STRING)
dut.timeout = 10000;
print(dut.ask('*IDN?'))


