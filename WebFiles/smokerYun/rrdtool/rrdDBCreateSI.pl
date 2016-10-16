#!/usr/bin/perl
use RRDs;

my $filename = "smokerSessionShortInterval.rrd";
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
    "DS:smokerTemp:GAUGE:120:0:400",
    "DS:meatTemp:GAUGE:120:0:200",
    "DS:setTemp:GAUGE:120:0:300",
    "DS:fanSpeed:GAUGE:120:0:300",
    "RRA:MAX:0.5:1:1500",
}

