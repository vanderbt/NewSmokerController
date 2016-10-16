<?php
        date_default_timezone_set('America/Los_Angeles');
	$tmStmp =  date('Ymdhis') ;
	$tmStmp1 = date('Y-m-d h:i:s');

	echo "The time is: $tmStmp1\r\n";
mysql_connect('localhost', 'root', 'Mike0101') or die("Can't connect that way!");

@mysql_select_db('grow') or die("Unable to select a database called 'grow'");
//if (ISSET($_GET['d']) {
	$aTemp = $_GET["at"];
	$aHum = $_GET["ah"];
	$h2Temp = $_GET["h2t"];
        echo "$aTemp\r\n";
	echo "$aHum\r\n";
	echo "$h2Temp\r\n";
	echo "";
	$qryAll = "INSERT INTO environment (airTemp, airHum, h20Temp, timeStamp) VALUES ('$aTemp','$aHum', '$h2Temp', '$tmStmp')";
	echo "$qryAll\r\n";
	mysql_query($qryAll);
	mysql_close();
//	sleep(25);
	exit(200);
//} 
?>
