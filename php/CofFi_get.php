<? #page which is called by the ESP8266 to check if there is a coffee to make
include "config.php";

$time = time();
$user = $_SERVER['HTTP_USER_AGENT'];
$ip = $_SERVER['REMOTE_ADDR'];
if(isset($_GET["deepsleep"]))
{
	echo "60";
	
}else {
		$q=mysql_query("SELECT id FROM CofFi WHERE timestampdiff(MINUTE, datetime, CURRENT_TIMESTAMP)<10 AND confirm=1 AND start=0");
		if(mysql_num_rows($q)>0) //request confirmed
		{
			$a=mysql_fetch_array($q);
			$id = $a["id"];
			echo "CofFi ".$id;
		}
		else echo "no request";
}
?>