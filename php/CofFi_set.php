<? #Script called by ESP8266 to update coffee status in database
include "config.php";

$time = time();
$user = $_SERVER['HTTP_USER_AGENT'];
$ip = $_SERVER['REMOTE_ADDR'];
if($user=="PidgeyCofFi") 
	{
		if(isset($_GET["id"]))
		{
			$id = $_GET["id"];
			if(isset($_GET["start"])) mysql_query("UPDATE CofFi SET start=CURRENT_TIMESTAMP WHERE id=$id");
			if(isset($_GET["heatup"])) mysql_query("UPDATE CofFi SET heatup=CURRENT_TIMESTAMP WHERE id=$id");
			if(isset($_GET["making"])) mysql_query("UPDATE CofFi SET making=CURRENT_TIMESTAMP WHERE id=$id");
			if(isset($_GET["done"])) mysql_query("UPDATE CofFi SET done=CURRENT_TIMESTAMP WHERE id=$id");				
		}
		echo "ok tnx";
	}
else echo "wrong user";
?>