
### Modified `httpd.c`

This is a modified version of the `httpd.c` which enables it to react to the `/lxi/identification` URL appropriately using SSI.

Usually, the httpd identifies directories by the URL ending with a slash.
This then triggers the mechanism to serve any index.* file present in that directory.
If the slash is missing, the httpd server thinks, a file is being requested.

The LXI identification tool will request the directory `/lxi/identification` (without a slash).
The httpd server therefore wants to serve the file `identification` from the directory `/lxi/`.
This behaviour is wrong, that is why this httpd file is adjusted to account for this special case.

If the STM32CubeMX performs the automatic code generation, this `httpd.c` file is overwritten.
That is why it is backed up here and will automatically be copied with the `../build.sh` script.


