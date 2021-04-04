#!/bin/bash

cd ./Tools
perl ./makefsdata.pl
cp fsdata.c ../Middlewares/Third_Party/LwIP/src/apps/http
cp fsdata_custom.c ../Middlewares/Third_Party/LwIP/src/apps/http

