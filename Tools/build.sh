#!/bin/bash
perl ./makefsdata.pl

echo "copying files to ../Middlewares/Third_Party/LwIP/src/apps/http"
cp fsdata.c ../Middlewares/Third_Party/LwIP/src/apps/http/
cp fsdata_custom.c ../Middlewares/Third_Party/LwIP/src/apps/http/


