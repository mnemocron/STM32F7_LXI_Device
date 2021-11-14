#!/bin/bash
perl ./makefsdata.pl

echo "copying files to ../Middlewares/Third_Party/LwIP/src/apps/http"
cp fsdata.c ../Middlewares/Third_Party/LwIP/src/apps/http/
cp fsdata_custom.c ../Middlewares/Third_Party/LwIP/src/apps/http/

# in case the httpd file is overwritten by automatic code generation
rm ../Middlewares/Third_Party/LwIP/src/apps/httpd.c
cp ./src/httpd.c ../Middlewares/Third_Party/LwIP/src/apps/http/httpd.c
