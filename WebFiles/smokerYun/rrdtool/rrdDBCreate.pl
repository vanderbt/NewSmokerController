#!/usr/bin/perl
use RRDs;

my $filename = "smokerSession.rrd";
my $path = "/var/www/html/smokerYun/rrdtool/";
my $img = "/var/www/html/rrdtool/images";
my $HOST = "localhost";
# if rrdtool database doesn't exist, create it
print ("$path$filename");
print ("");
if (! -e "$path$filename") {
print ("I'm here!");
  RRDs::create "$path$filename",
    "-s 60",
    "DS:smokerTemp:GAUGE:600:0:400",
    "DS:meatTemp:GAUGE:600:0:200",
    "DS:setTemp:GAUGE:6900:0:300",
    "DS:fanSpeed:GAUGE:6900:0:300",
    "RRA:AVERAGE:0.5:1:1440",
    "RRA:AVERAGE:0.5:5:288",
    "RRA:AVERAGE:0.5:30:672",
    "RRA:AVERAGE:0.5:120:732",
    "RRA:AVERAGE:0.5:720:1460";
}

