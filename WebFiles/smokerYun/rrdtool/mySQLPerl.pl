#!/usr/bin/perl
#!/var/www/html/smokerYun/rrdtool/mySQLPerl.pl

  use strict;
  use warnings;
  use DBI;
  use RRDs;
my $fanSpeed = 0;
my $smokerTemp = 0;
my $meatTemp = 0;
my $setTemp = 0;
my $timeStamp = 0;
my $ID = 0;

  # Connect to the database.
  my $dbh = DBI->connect("DBI:mysql:database=arduino;host=localhost",
                         "root", "Mike0101",
                         {'RaiseError' => 1});

  # retrieve data from environment.
  my $sth = $dbh->prepare("SELECT * FROM smoker ORDER BY id DESC LIMIT 1");
  $sth->execute();
  my @row;
	while (@row = $sth->fetchrow_array) {

        $smokerTemp = "$row[0]";
        $meatTemp = "$row[1]";
	$fanSpeed = "$row[3]";
	$timeStamp = "$row[4]";


    print ("Smoker Temp: ", $row[0],  "\n");
    print ("Meat Temp: ", $row[1],  "\n");
    print ("Fan Speed: ", $row[3], "\n");
    print ("Time: ", $row[4], "\n");
  }
  $sth->finish();

  my $sth1 = $dbh->prepare("SELECT setTemp FROM setTemp ORDER BY id DESC LIMIT 1");
  $sth1->execute();
  my @row1;
        while (@row1 = $sth1->fetchrow_array) {

        $setTemp = "$row1[0]";

    print ("Setpoint Temp: ", $row1[0],  "\n");
  }
  $sth->finish();


  # Disconnect from the database.
  $dbh->disconnect();
# insert values into rrd database
$ENV{TZ} = ":/usr/share/zoneinfo/America/Los_Angeles";

RRDs::update "smokerSession.rrd",
	"-t", "setTemp:smokerTemp:meatTemp:fanSpeed",
	"N:$setTemp:$smokerTemp:$meatTemp:$fanSpeed";


# generate graph
RRDs::graph "tempGraph.png", 
	"-w","873",
	"-h","216",
	"-a", "PNG",
	"-t Weber Smokey Mountain Smoker" ,
	"-v $timeStamp",
	"--slope-mode",
        "--start","-86400s",
	"--end","now","--slope-mode",
	"DEF:smokerTemp=/var/www/html/smokerYun/rrdtool/smokerSession.rrd:smokerTemp:AVERAGE", 
	"DEF:meatTemp=/var/www/html/smokerYun/rrdtool/smokerSession.rrd:meatTemp:AVERAGE",
	"DEF:fanSpeed=/var/www/html/smokerYun/rrdtool/smokerSession.rrd:fanSpeed:AVERAGE",
	"DEF:setTemp=/var/www/html/smokerYun/rrdtool/smokerSession.rrd:setTemp:AVERAGE",
	"LINE2:smokerTemp#ff0000:Current Smoker Temp =$smokerTemp",
	"LINE2:meatTemp#00ff00:Current Meat Temp =$meatTemp",
	"LINE2:fanSpeed#0000ff:Current Fan Speed =$fanSpeed",
	"LINE2:setTemp#171A14:Current Setpoint Temp =$setTemp";


my $ERR=RRDs::error;
die "ERROR while creating graph: $ERR\n" if $ERR;
