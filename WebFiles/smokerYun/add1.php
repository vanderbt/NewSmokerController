<?php
function addSetPoint() {   	include("connect.php");
   	$link=Connection();
	$result = mysql_query("SELECT setTemp FROM setTemp ORDER BY id DESC LIMIT 1", $link); 
        if($result!==FALSE){
                     while($row = mysql_fetch_array($result)) {
                        echo $row["setTemp"];
                     }
                     mysql_free_result($result);
                     mysql_close();
                  }

	mysql_close($link);
	}
?>
