<html>
<body>

<?php
        date_default_timezone_set('America/Los_Angeles');
        $tmStmp =  date('Ymdhis') ;
        $tmStmp1 = date('Y-m-d h:i:s');

        echo "The time is: $tmStmp1\r\n";

$con = mysql_connect('localhost', 'root', 'Mike0101') or die ("Can't connect that way!");
//if (mysql_connect_errno())
//	{
//	echo "Failed to connect to MySQL: " . mysql_connect_error();
//	}
mysql_select_db('arduino') or die ("Unable to select a database called 'arduino'");

$sql="INSERT INTO setTemp (setTemp, timeStamp) VALUE ('$_POST[setPoint]', $tmStmp)";

if (!mysql_query($sql, $con))
	{
	die ('Error: ' . mysql_error());
	}
echo "1 record added";
mysql_close($con);
header('Location: /index.php');
?>
</body>
</html>
