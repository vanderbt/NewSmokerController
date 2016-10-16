<?php

	include("connect.php"); 	
	
	$link=Connection();

	$result=mysql_query("SELECT * FROM `environment` ORDER BY `ID` DESC LIMIT 1",$link);
?>

<html>
   <head>
      <title>Sensor Data</title>
   </head>
<body>
   <h1>Temperature / Humidity Sensor Readings</h1>

   <table border="1" cellspacing="1" cellpadding="1">
		<tr>
			<td>&nbsp;timeStamp&nbsp;</td>	
			<td>&nbsp;airTemp&nbsp;</td>
			<td>&nbsp;airHum&nbsp;</td>
			<td>&nbsp;h20Temp&nbsp;</td>
		</tr>

      <?php 
		  if($result!==FALSE){
		     while($row = mysql_fetch_array($result)) {
		        printf("<tr><td> &nbsp;%s </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp <td></tr>", 
		           $row["timeStamp"], $row["airTemp"], $row["airHum"], $row["h20Temp"]);
		     }
		     mysql_free_result($result);
		     mysql_close();
		  }
      ?>

   </table>
	<img src="/rrdtool/tempGraph.png" alt="My Graph">
 	<img src="/rrdtool/tempGraph2.png" alt="My Graph">
 	<img src="/rrdtool/tempGraph3.png" alt="My Graph">

 	<img src="/rrdtool/tempGraph4.png" alt="My Graph">
</body>
</html>

