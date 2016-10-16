<html>
<head>
style="background-image: url(http://beavercreek-smokehouse.com/wp-content/themes/Pluto/data/1402773744_bbqfire.jpg); width:800 px;hieght:400 px;">
<h1 style="color: red;">Smoker Status<br>
</h1>
<title>Sensor Data</title>
</head>
<body>
<table>
  <tr>
    <td align="center">SMOKER DATA</td>
  </tr>
  <tr>
    <td>
      <table border="1">
      <tr>
        <td>TIME STAMP</td>
        <td>SMOKER TEMP</td>
        <td>MEAT TEMP</td>
        <td>FAN Speed</td>
      </tr>
<?php
mysql_connect("localhost","root","Mike0101");
mysql_select_db('arduino');
$order = "SELECT *  FROM smoker ORDER BY id DESC LIMIT 1";
$result = mysql_query($order);while ($data = mysql_fetch_row($result)){
  echo("<tr><td>$data[4]</td><td>$data[0]</td><td>$data[1]</td><td>$data[3]</td></tr>");
}
?>
    </table>
  </td>
</tr>
</table>
</body>
</html>

<form action="insert.php" method="post">
Smoker Setpoint: <input type="text" name="setPoint"><br><br>

<input type="submit" />

</form>



</tbody>
</table>
<img src="/smokerYun/rrdtool/tempGraph.png" alt="My Graph"> <img
</body>
</html>

