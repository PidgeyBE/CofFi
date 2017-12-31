<?
//connect to Database
$dhost = "localhost"; //usually localhost, or ip
$dusername = ""; // database user
$dpassword = ""; // database pass
$ddatabase = ""; // database name

$con = mysql_connect($dhost, $dusername, $dpassword) or die("Cannot Connect"); 
mysql_select_db($ddatabase, $con);


//functions

?>