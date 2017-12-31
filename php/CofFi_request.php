<? #Script called by HTTP Widget to reflect the right status
include "config.php";

$time = time();
$user = $_SERVER['HTTP_USER_AGENT'];
$ip = $_SERVER['REMOTE_ADDR'];
echo "#3F250B ";
if($_GET["isClick"]=="true") 
	{
		//check if there is a recent requested coffe
		$q=mysql_query("SELECT * FROM CofFi WHERE timestampdiff(MINUTE, datetime, CURRENT_TIMESTAMP)<10");
		if(mysql_num_rows($q)>0) //request found
		{
			$a = mysql_fetch_array($q);
			$id = $a["id"];
			if($a["confirm"]==0) 
			{
				mysql_query("UPDATE CofFi SET confirm=1 WHERE id=$id");
				echo "making CofFi!";
			}
			elseif($a["done"]!=0) echo "done!";
			elseif($a["making"]!=0) echo "dripping!";
			elseif($a["heatup"]!=0) echo "heating!";
			elseif($a["start"]!=0) echo "making CofFi!";
			else echo "waiting...";
			
			
		}
		else {
			$ts = time();
			mysql_query("INSERT INTO CofFi (timestamp) VALUES ($ts)"); //log new coffee request which can be confirmed later
			echo "sure?";
		}
		
	}
else echo "CofFi!?";
?>