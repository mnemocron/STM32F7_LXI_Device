
## makefsdata

---


```powershell
cmd > makefsdata.exe -e
```


```
 makefsdata - HTML to C source converter
     by Jim Pettinato               - circa 2003
     extended by Simon Goldschmidt  - 2009

 Failed to open directory "fs".

 Usage: htmlgen [targetdir] [-s] [-i] [-f:<filename>]

targetdir: relative or absolute path to files to convert
switch -s: toggle processing of subdirectories (default is on)
switch -e: exclude HTTP header from file (header is created at runtime, default is off)
switch -11: include HTTP 1.1 header (1.0 is default)
switch -c: precalculate checksums for all pages (default is off)
switch -f: target filename (default is "fsdata.c")
```

newer versions:

```
Usage: htmlgen [targetdir] [-s] [-e] [-11] [-nossi] [-ssi:<filename>] [-c] [-f:<filename>] [-m] [-svr:<name>] [-x:<ext_list>] [-xc:<ext_list>" USAGE_ARG_DEFLATE NEWLINE NEWLINE);

targetdir: relative or absolute path to files to convert" NEWLINE);
switch -s: toggle processing of subdirectories (default is on)" NEWLINE);
switch -e: exclude HTTP header from file (header is created at runtime, default is off)" NEWLINE);
switch -11: include HTTP 1.1 header (1.0 is default)" NEWLINE);
switch -nossi: no support for SSI (cannot calculate Content-Length for SSI)" NEWLINE);
switch -ssi: ssi filename (ssi support controlled by file list, not by extension)" NEWLINE);
switch -c: precalculate checksums for all pages (default is off)" NEWLINE);
switch -f: target filename (default is \"fsdata.c\")" NEWLINE);
switch -m: include \"Last-Modified\" header based on file time" NEWLINE);
switch -svr: server identifier sent in HTTP response header ('Server' field)" NEWLINE);
switch -x: comma separated list of extensions of files to exclude (e.g., -x:json,txt)" NEWLINE);
switch -xc: comma separated list of extensions of files to not compress (e.g., -xc:mp3,jpg)" NEWLINE);
```


