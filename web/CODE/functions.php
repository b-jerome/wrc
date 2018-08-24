<?php
/*
+---------------------------------------------------------------------------
|   > Copyright (c) 2013
|   > Bourgeois Jérôme
+---------------------------------------------------------------------------
|   > Controller FUNCTIONS
|   > Module written by Bourgeois Jérôme
|   > mail.bjerome@gmail.com
----------------------------------------------
|   > Date started: Aug 2, 2012
|   > Release updated: Mar 29, 2013 - 01:05 AM
+---------------------------------------------
|   > Module Version Number: 0.0.1.2
+---------------------------------------------
|   > Web Reef Controller
+--------------------------------------------------------------------------
*/
class functions
{
       var $do   = 0;
       var $to   = 0;
       var $val  = 0;
       var $func = 0;
       var $date = 0;
       var $sql_log;

       /*-------------------------------------------------------------------------*/
	// Auto-run
       /*-------------------------------------------------------------------------*/

       function auto_run()
       {

            //CONFIG
            require_once('config.php');

            //LANGUAGE
            include('lang/' . CONFIG_LANG . '/lang.global.php');

            //IP
            $ip = $_SERVER["REMOTE_ADDR"];

            //AUTORIZED IP'S IN CONFIG FILE
            $auth = explode(',',CONFIG_AUTH_IP);

            if ( !in_array ( $ip, $auth ) && CONFIG_RESTRICT_ACCESS == 'true' )
            {
                 echo "false|NOT ALLOWED";
                 exit;
            }

            $this->inputs = $_GET ? $_GET : $_POST;

            if ( isset( $this->inputs['do'] ) )
            {
                 switch( $this->inputs['do'] )
                 {
			case 'data':
			      $this->get_data();
			      break; 
			case 'temps':
			      $this->get_temps();
			      break;
			case 'meteo':
			      $this->get_meteo();
			      break;  
			case 'udp':
			      $this->udp();
			      break;  
		       default:
			      $this->udp();
			      break;
                 }
            }
       }

       /*-------------------------------------------------------------------------*/
	// UDP
	/*-------------------------------------------------------------------------*/

       function udp ( )
       {
           if ( isset($this->inputs['to']) )
           {
              $to   = $this->inputs['to'];
           }
           if ( isset($this->inputs['to']) )
           {
              $val  = $this->inputs['val'];
           }
           if ( isset($this->inputs['func']) )
           {
              $func = $this->inputs['func'];
           }
           if ( isset($this->inputs['date']) )
           {
              $date = $this->inputs['date'];
           }          

           if ( !$to && !$val && !$func )
           {
                return;
           }

           //IP to send 
           $host=CONFIG_IP;

           //port to send
           $port=CONFIG_PORT;

           //Create Socket
           $socket = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);
    
           //Make socket request
           $sock = socket_sendto($socket, $to.','.$val.','.$func.','.$date, strlen($to.','.$val.','.$func.','.$date), 0, $host, $port);

           //Close socket
           socket_close($socket);

           if ( $func == 6 )
           {
                $this->load_data();
           }
           else
           {
                exit;
           }
       }

       /*-------------------------------------------------------------------------*/
	// GET DATA FROM ARDUINO
       /*-------------------------------------------------------------------------*/

       function get_data()
       {
           //We are connected to arduino
           $this->saveconnect("true");
 
           //Save Data
           $save = $this->savedata();

           echo $save;
           exit;
       }

       /*-------------------------------------------------------------------------*/
	// LOAD DATA
       /*-------------------------------------------------------------------------*/

       function load_data()
       {
                //Are we connected with arduino ? 
                sleep ( 2 );

                $this->checkconnect();

                //Load data file. 

                if ( !$handle = fopen(CONFIG_FILE_DATA, 'r') ) 
                {
                     echo "false|".LANG_impossible." (".CONFIG_FILE_DATA.")";
                     exit;
                }

                if ( !$contents = fread($handle, filesize(CONFIG_FILE_DATA)) ) 
                {
                     echo "false|".LANG_nodata." (".CONFIG_FILE_DATA.")";
                     exit;
                }

                fclose($handle);

                if ( $contents )
                {
                     $contents = explode ( ',', $contents );
                     $contents = implode ( '|', $contents );
                     print ( "true|".LANG_dataok."|".$contents );
                } 
                else
                {
                     echo "false|".LANG_datanotok."(".CONFIG_FILE_DATA.")";
                }

                exit;
       }

       /*-------------------------------------------------------------------------*/
	// SAVE DATA (ARDUINO)
       /*-------------------------------------------------------------------------*/

       function savedata( )
       {
           if ( is_writable ( CONFIG_FILE_DATA ) ) 
           {
                $content = $this->inputs['data'];
                if ( !$handle = fopen(CONFIG_FILE_DATA, 'w') ) 
                {
                     echo LANG_impossible." (".CONFIG_FILE_DATA.")";
                     exit;
                }

                if (fwrite($handle, $content) === FALSE) 
                {
                    echo LANG_impossiblee." (".CONFIG_FILE_DATA.")";
                    exit;
                }

                echo LANG_writeend;

                fclose($handle);
           } 

           if ( !$this->inputs['func'] )
           {
                exit;
           }

           $now = time();

           $mysql = mysql_connect ( CONFIG_SQL_HOST, CONFIG_SQL_USER, CONFIG_SQL_PASS );

           if ( !$mysql )  
           {
               $this->log ( "[".$this->get_date( $now )."] ".mysql_error()."#e1" );
           }

           //STORM / CLOUD
           if ( $this->inputs['func'] == 2 )
           {
                //make sure we only have one prevision for today
                $tms = strtotime('today');
                $sql = 'DELETE FROM '.CONFIG_SQL_DATABASE.'.saison WHERE date >= '.$tms;

                $req =  mysql_query ( $sql );

                //Do query
                if ( ! $req )
                {
                     $now = time();
                     $this->log ( "[".$this->get_date( $now )."] ".mysql_error()."#e2" );
                }

                $sec_req = 'INSERT INTO '.CONFIG_SQL_DATABASE.'.saison SET storm_start='.intval( $this->inputs['storm_start'] ).', storm_end='.intval( $this->inputs['storm_end'] ).', cloud_start='.intval( $this->inputs['cloud_start'] ).', cloud_end='.intval ( $this->inputs['cloud_end'] ).', date='.$now;
        
                //Do query
                if ( !mysql_query ( $sec_req ) )
                {
                     $this->log ( "[".$this->get_date( $now )."] ".mysql_error()."#e3" );
                }
           }
           //TEMPS
           else if ( $this->inputs['func'] == 3 )
           {
                $req = "";
                $req = 'INSERT INTO '.CONFIG_SQL_DATABASE.'.temps SET temp1="'.floatval ( $this->inputs['temp1'] ).'", temp2="'.floatval ( $this->inputs['temp2'] ).'", date='.$now;
           }

           //Do query
           if ( !mysql_query ( $req ) )
           {
               $this->log ( "[".$this->get_date( $now )."] ".mysql_error()."#e4" );
           }

           mysql_close( $mysql );
       }

       /*-------------------------------------------------------------------------*/
	// TEMPERATURES
       /*-------------------------------------------------------------------------*/

       function get_temps()
       { 
           $result = 'false';

           $mysql = mysql_connect ( CONFIG_SQL_HOST, CONFIG_SQL_USER, CONFIG_SQL_PASS );

           if ( !$mysql )  
           {
               $this->log ( "[".$this->get_date( $now )."] ".mysql_error()."#e5" );
           }

           $sql = 'SELECT * FROM '.CONFIG_SQL_DATABASE.'.temps ORDER BY id DESC limit 9';

           $req =  mysql_query ( $sql );

           //Do query
           if ( ! $req )
           {
               $now = time();
               $this->log ( "[".$this->get_date( $now )."] ".mysql_error()."#e6" );
           }
           else
           {
               $num_rows = mysql_num_rows($req);

               if ( $num_rows )
               {
                    while ( $r = mysql_fetch_assoc ( $req ) )
                    {
                         $date[]  = $this->get_date( $r['date'], 1 );
                         $temp1[] = $r['temp1'];
                         $temp2[] = $r['temp2'];
                    }

                    $date = array_reverse($date,true);
                    $temp1 = array_reverse($temp1,true);
                    $temp2 = array_reverse($temp2,true);

                    if ( is_array ( $date ) && count ( $date ) )
                    {
                         $data['date'] = implode(',' ,$date);
                    }

                    if ( is_array ( $temp1 ) && count ( $temp1 ) )
                    {
                         $data['temp1'] = implode(',' ,$temp1);
                    }

                    if ( is_array ( $temp2 ) && count ( $temp2 ) )
                    {
                         $data['temp2'] = implode(',' ,$temp2);
                    }

                    if ( $data['date'] && $data['temp1'] && $data['temp2'] )
                    {
                         $result = 'success|'.$data['date'].'|'.$data['temp1'].'|'.$data['temp2'];
                    }
               }

               print ( $result );
           }

           mysql_close( $mysql );
           exit;
       }

       /*-------------------------------------------------------------------------*/
	// METEO PREVISIONS
       /*-------------------------------------------------------------------------*/

       function get_meteo()
       { 
           $result = 'false';

           $mysql = mysql_connect ( CONFIG_SQL_HOST, CONFIG_SQL_USER, CONFIG_SQL_PASS );

           if ( !$mysql )  
           {
               $this->log ( "[".$this->get_date( $now )."] ".mysql_error()."#e7" );
           }

           $sql = 'SELECT * FROM '.CONFIG_SQL_DATABASE.'.saison ORDER BY id DESC limit 9';

           $req =  mysql_query ( $sql );

           //Do query
           if ( ! $req )
           {
               $now = time();
               $this->log ( "[".$this->get_date( $now )."] ".mysql_error()."#e8" );
           }
           else
           {
               $num_rows = mysql_num_rows($req);

               if ( $num_rows )
               {
                    while ( $r = mysql_fetch_assoc ( $req ) )
                    {
                         $date[] = $this->get_date( $r['date'], 2 );

                         //Storm
                         $storm_start[]  = date( 'H:i:s',( strtotime('today') + $r['storm_start'] ) );
                         $storm_end[]    = date( 'H:i:s',( strtotime('today') + $r['storm_end'] ) );
                         $storm_length[] = round ( ( $r['storm_end'] - $r['storm_start']  ) / 60, 0 );

                         //Clouds  
                         $cloud_start[]  = date( 'H:i:s',( strtotime('today') + $r['cloud_start'] ) );
                         $cloud_length[] = round( ( $r['cloud_end'] - $r['cloud_start'] ) / 60, 0 );
                         $cloud_end[]    = date( 'H:i:s',( strtotime('today') + $r['cloud_end'] ) );
                    }


                    $date = array_reverse($date,true);
                    $storm_start = array_reverse($storm_start,true);
                    $storm_end = array_reverse($storm_end,true);
                    $storm_length = array_reverse($storm_length,true);

                    $cloud_start = array_reverse($cloud_start,true);
                    $cloud_end = array_reverse($cloud_end,true);
                    $cloud_length = array_reverse($cloud_length,true);

                    if ( is_array ( $date ) && count ( $date ) )
                    {
                         $data['date'] = implode(',' ,$date);
                    }

                    if ( is_array ( $storm_start ) && count ( $storm_start ) )
                    {
                         $data['storm_start'] = implode(',' ,$storm_start);
                    }

                    if ( is_array ( $storm_end ) && count ( $storm_end ) )
                    {
                         $data['storm_end'] = implode(',' ,$storm_end);
                    }

                    if ( is_array ( $storm_length ) && count ( $storm_length ) )
                    {
                         $data['storm_length'] = implode(',' ,$storm_length);
                    }

                    if ( is_array ( $cloud_start ) && count ( $cloud_start ) )
                    {
                         $data['cloud_start'] = implode(',' ,$cloud_start);
                    }

                    if ( is_array ( $cloud_end ) && count ( $cloud_end ) )
                    {
                         $data['cloud_end'] = implode(',' ,$cloud_end);
                    }

                    if ( is_array ( $cloud_length ) && count ( $cloud_length ) )
                    {
                         $data['cloud_length'] = implode(',' ,$cloud_length);
                    }

                    if ( $data['date'] )
                    {
                         $result = 'success|'.$data['date'].'|'.$data['storm_start'].'|'.$data['storm_end'].'|'.$data['storm_length'].'|'.$data['cloud_start'].'|'.$data['cloud_end'].'|'.$data['cloud_length'];
                    }
               }

               print ( $result );
           }

           mysql_close( $mysql );
           exit;
       }

       /*-------------------------------------------------------------------------*/
	// CONNECTION TO ARDUINO
       /*-------------------------------------------------------------------------*/

       function saveconnect( $content="true" )
       {
           if ( is_writable ( CONFIG_FILE_CONNECT ) ) 
           {
                if ( !$handle = fopen(CONFIG_FILE_CONNECT, 'w') ) 
                {
                     echo LANG_impossible." (".CONFIG_FILE_CONNECT.")";
                     exit;
                }

                if (fwrite($handle, $content) === FALSE) 
                {
                    echo LANG_impossiblee." (".CONFIG_FILE_CONNECT.")";
                    exit;
                }

                fclose($handle);
           } 
           else 
           {
                echo CONFIG_FILE_CONNECT." ".LANG_perms;
                exit;
           }
       }

       /*-------------------------------------------------------------------------*/
	// CONNECTION TO ARDUINO
       /*-------------------------------------------------------------------------*/

       function checkconnect()
       {
                if ( !$handle = fopen(CONFIG_FILE_CONNECT, 'r') ) 
                {
                     echo "false|".LANG_impossiblee." (".CONFIG_FILE_CONNECT.")";
                     exit;
                }

                if ( !$contents = fread($handle, filesize(CONFIG_FILE_CONNECT)) ) 
                {
                     echo "false|".LANG_impossibler." (".CONFIG_FILE_CONNECT.")";
                     exit;
                }

                fclose($handle);

                if ( $contents == "true" )
                {
                     //we are connected update file
                     $this->saveconnect("false");
                } 
                else
                {
                     echo "false|". LANG_connect;
                     exit;
                }
       }

       /*-------------------------------------------------------------------------*/
	// LOGS
       /*-------------------------------------------------------------------------*/

       function log( $content="" )
       { 
           if ( is_writable ( CONFIG_FILE_MYSQL ) && $content ) 
           {
                $handle = fopen(CONFIG_FILE_MYSQL, 'a+'); 

                fwrite($handle, $content."\r\n");

                fclose($handle);
           } 
       }

       /*-------------------------------------------------------------------------*/
	// DATE
       /*-------------------------------------------------------------------------*/

       function get_date( $date="", $func=0 )
       { 
           $ret = 0;

           if ( $date ) 
           {
               if ( CONFIG_TIME == 1 )
               {
                    $t = "H:i:s";
               }
               else
               {
                    $t = "h:i:s";
               }

               if ( CONFIG_DATE == 1 )
               {
                    $d = "d/m/y";
               }
               else
               {
                    $d = "m/d/y";
               }

               if ( $func == 1 )
               {
                    $ret = date( $t.' - ('.$d.')', $date );
               }
               else if ( $func == 2 )
               {
                    $ret = date( $d, $date );
               }
               else
               {
                    $ret = date( $d.' - '.$t, $date );
               }
           } 

           return $ret;
       }
}

//run class
$init = new functions;
$init->auto_run();

?>