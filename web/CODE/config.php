<?php
/** START SETTINGS **/

//LANG
define('CONFIG_LANG', 'fr');   //LANGUAGE (FR or US)

//ARDUINO
define('CONFIG_IP', '255.255.255.255');  //HOME IP
define('CONFIG_PORT', '7766');           //HOME PORT

//MYSQL
define('CONFIG_SQL_USER','wrc');        //SQL USER 
define('CONFIG_SQL_HOST','255.255.255.255');       //SQL HOST
define('CONFIG_SQL_PASS','xzersfdjcpocplmld');    //SQL PASSWORD
define('CONFIG_SQL_DATABASE','wrc');    //SQL BASE NAME

//OTHER
define('CONFIG_TIME','1');  //Time display: 1 = 24h OR 2 = 12h
define('CONFIG_DATE','1');  //Date display: 1 = day/month/year OR 2 = month/day/year
define('CONFIG_TEMPS','1');  //Temperature display: 1 = C° OR 2 = F
define('CONFIG_DISPLAY_ERRORS', false);        //DISPLAY PHP ERRORS (false or true)
define('CONFIG_TIMEZONE','Europe/Paris');      //SEE http://www.php.net/manual/en/timezones.php

//SECURITY
define('CONFIG_RESTRICT_ACCESS','true');   //Restrict the access to some IP's (true or false). If false the webpage will be public /!\
define('CONFIG_AUTH_IP','255.255.255.255');  //If CONFIG_RESTRICT_ACCESS is true these ip are authorized to access the controller. ( separate ip's with a comma )


/** END SETTINGS, NO NEED TO EDIT BELLOW **/

//FILES
define('CONFIG_FILE_DATA','controller.txt');
define('CONFIG_FILE_CONNECT','connect.txt');
define('CONFIG_FILE_MYSQL','logs/mysql.txt');

ini_set( "display_errors", CONFIG_DISPLAY_ERRORS );
date_default_timezone_set( CONFIG_TIMEZONE );   
 
function handleException( $exception ) {
  echo "Sorry, a problem occurred. Please try again...";
  error_log( $exception->getMessage() );
}
 
set_exception_handler( 'handleException' );
?>
