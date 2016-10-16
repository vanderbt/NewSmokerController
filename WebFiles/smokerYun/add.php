<?php
   	include("connect.php");
   	
   	$link=Connection();
        
	date_default_timezone_set('America/Los_Angeles');
#       $tmStmp =  date('Ymdhis') ;
        $tmStmp1 = date('Y-m-d H:i:s');
#       $tmStmp1 = date('H:m:s \m \i\s\ \m\o\n\t\h');
#       $tmStmp1 = date("H:i:s");   
#        echo "The time is: $tmStmp1\r\n";

	$dTemp=$_POST["d"];
	$eTemp=$_POST["e"];
	$fSpeed=$_POST["f"];

	$query = "INSERT INTO smoker (smokerTemp, meatTemp, fanSpeed, timeStamp) 
		VALUES ('$dTemp','$eTemp','$fSpeed','$tmStmp1')"; 
   	
   	mysql_query($query,$link);
	mysql_close($link);

   	header("Location: ../index.php");
?>

