#!/usr/bin/python3
import os
makefile = './Debug/makefile'

rf = open(makefile)
temp = ''
for line in rf:
	pattern = ''
	if('Users' in str(line)):
		lin = str(line).replace('\n','').split(' ')
		for arg in lin:
			if('Users' in arg):
				pattern = arg
				arg = arg.split('\\')
				nln = arg[0].split('C:')[0] + '../' +  arg[-1]
	if(len(pattern) > 1):
		line = line.replace(pattern, nln)
	temp += line
rf.close()

os.remove(makefile)

wf = open(makefile, 'w')
wf.write(temp)
wf.close()


