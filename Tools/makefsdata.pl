#!/usr/bin/perl
use warnings;

# This script creates our output file were we will store all the converted html data to C arrays.
#
# The data is saved into the file in HEX values but the data is written to corespond to the
# following structure. Actually at the end of the script the arrays are pointed to the structures.
#
#  struct httpd_fsdata_file_noconst
#  {
#    struct httpd_fsdata_file *next;
#    unsigned char *name;
#    unsigned char *data;
#    int len;
#  };
#

# open the out file for writing
#open(OUTPUT, "> fsdata.c");
open(OUTPUT, "> fsdata_custom.c");

# change to were all the files are located.
chdir("fs");

# open directory and serch all the working files. Ignore cvs. svn or other
# files not needed to convert
opendir(DIR, ".");

# collecting the file names in the root and all subdirectories to proccess.
@files =  grep { !/^\./ && !/(CVS|~)/ } readdir(DIR);

print "\nopening root directory, found the following files and all subdirectories to process:\n  ";

print_file_names(@files);

closedir(DIR);


# process directories and add their files as well.
foreach $file (@files)
{
  if(-d $file && $file !~ /^\./)
  {
    print "\nProcessing directory $file\n";
    opendir(DIR, $file);
    @newfiles =  grep { !/^\./ && !/(CVS|~)/ } readdir(DIR);
    closedir(DIR);
    print "  Adding files: @newfiles\n";
    @files = (@files, map { $_ = "$file/$_" } @newfiles);
    next;
  }
}

print "\n\nStarting convert files to arrays process:\n";

foreach $file (@files)
{
  if(-f $file)
  {
    print "  Adding file: $file\n";

	# open the data file and convert it to HEX valuse...
    open(FILE, $file) || die "Could not open file $file\n";
      binmode(FILE);

	my ($dev, $ino, $mode, $nlink, $uid, $gid, $rdev, $size, $atime, $mtime, $ctime, $blksize, $blocks) = stat($file);

    $file =~ s-^-/-;
    $fvar = $file;
    $fvar =~ s-/-_-g;
    $fvar =~ s-\.-_-g;

    # for AVR, add PROGMEM here
    # write the prefix and array name to hold the converted data
    print(OUTPUT "static const unsigned char data".$fvar."[] = {\n");

    # write the original directory and the file name source
    print(OUTPUT "\t/* File: $file */\n\t");

    # process the file. Convert its data to HEX values.
    # first write the file name to the structure
    $file_len = length($file);
    for($j=0; $j<$file_len; $j++)
    {
      printf(OUTPUT "0x%02X, ", unpack("C", substr($file, $j, 1)));
    }
    printf(OUTPUT "0,\n");


    # process the file data and convert it to binary
    print(OUTPUT "\t/* here starts the data */\n\t");


    $i = 0;
    $items_per_line = 10;

	# this loop proccesses the input file. It converts every byte
	# to a binary value and stors it in file.
    for($bytesread=0; $bytesread<$size; $bytesread++)
	{
	  # read one byte from file
  	  read(FILE, $data, 1);

	  # print it in HEX format
      printf(OUTPUT "0x%02X", unpack("C", $data));

      # if the are more bytes to process add a seperator !
      if($bytesread < ($size-1))
      {
	    print(OUTPUT ", ");
	  }

	  $i++;

      if($i == $items_per_line)
      {
        print(OUTPUT "\n\t");
        $i = 0;
      }
    }

    # no more bytes to process close the array.
    print(OUTPUT "};\n\n");

    print "  $file, Bytesread = $bytesread\n\n";
    close(FILE);

    push(@fvars, $fvar);
    push(@pfiles, $file);
  }
}


# here we start creating the structures definitions. See at begining of the script.
for($i = 0; $i < @fvars; $i++)
{
  $file = $pfiles[$i];
  $fvar = $fvars[$i];

  # the first structure does not point to any somone else so we write NULL.
  if($i == 0)
  {
    $prevfile = "NULL";
  }
  else
  {
    $prevfile = "file" . $fvars[$i - 1];
  }

  print(OUTPUT "const struct fsdata_file file".$fvar."[] = {{$prevfile, data$fvar, ");
  print(OUTPUT "data$fvar + ". (length($file) + 1) .", ");
  print(OUTPUT "sizeof(data$fvar) - ". (length($file) + 1) ."}};\n\n");
}

print(OUTPUT "#define FS_ROOT file$fvars[$i - 1]\n\n");
print(OUTPUT "#define FS_NUMFILES $i\n");


# subrutine...
sub print_file_names()
{
  my $size = @_;

  for($count=0; $count<$size; $count++)
  {
    printf("%s ", $_[$count]);
  }
  print "\n";
}
