<?php
require_once('config.php');
include('lang/' . CONFIG_LANG . '/lang.global.php');

//IP
$ip = $_SERVER["REMOTE_ADDR"];

//AUTORIZED IP'S IN CONFIG FILE
$auth = explode(',',CONFIG_AUTH_IP);

if ( !in_array ( $ip, $auth ) && CONFIG_RESTRICT_ACCESS == 'true' )
{
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
        <title>404</title>
        <link rel="stylesheet" type="text/css" href="css/style.css" media="screen" />
    </head>

    <body>
        <div id="r404">
           OOPS ! ACCESS NOT ALLOWED :(
       </div>
    </body>

</html>
<?php
exit;
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
        <title>Web Reef Controller</title>
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"></script>
        <script src="http://ajax.googleapis.com/ajax/libs/jqueryui/1.8/jquery-ui.min.js"></script>
        <script src="scripts/controller.js"></script>
        <script src="scripts/touch.js"></script>
        <script src="scripts/wipetouch.js"></script>
        <script src="scripts/graphs/highcharts.js"></script>
        <script src="scripts/graphs/themes/gray.js"></script>
        <script src="scripts/timepicker/jquery-ui-timepicker-addon.js"></script>
        <script src="scripts/timepicker/jquery-ui-sliderAccess.js"></script>
        <script src="scripts/timepicker/localization/jquery-ui-timepicker-<?php echo CONFIG_LANG ?>.js"></script>
        <link rel="stylesheet" type="text/css" href="css/style.css" media="screen" />
        <link href="images/ui/1/theme.css" rel="stylesheet" type="text/css"/>
        <link href="scripts/timepicker/jquery-ui-timepicker-addon.css" rel="stylesheet" type="text/css"/>

		<script type="text/javascript">

	$(document).ready(function() {
 
       chart = new Highcharts.Chart({
	 colors: ["#55BF3B", "#7798BF", "#DF5353", "#aaeeee", "#ff0066", "#eeaaee",
		"#55BF3B", "#DF5353", "#7798BF", "#aaeeee"],
            chart: {
                renderTo: 'temps_charts',
                type: 'spline',
                marginRight: 130,
                marginBottom: 50
            },

            title: {
                text: '<?php echo LANG_temp; ?>',
                x: -20 //center
            },
            subtitle: {
                text: '<?php echo LANG_piece; ?> - <?php echo LANG_aqua; ?>',
                x: -20
            },
            xAxis: {
                categories: [0]
            },
            yAxis: {
                title: {
                    text: '<?php echo LANG_temp; ?> (°<?php if( CONFIG_TEMPS == 1 ) { echo 'C'; }else{ echo 'F'; }?>)'
                },
                plotLines: [{
                    value: 0,
                    width: 1,
                    color: '#808080'
                }]
            },
            tooltip: {
                formatter: function() {
                        return '<b>'+ this.series.name +'</b><br />'+
                        this.x +'<br />'+ this.y +'°<?php if( CONFIG_TEMPS == 1 ) { echo 'C'; }else{ echo 'F'; }?>';
                }
            },
            legend: {
                layout: 'vertical',
                align: 'right',
                verticalAlign: 'top',
                x: -10,
                y: 100,
                borderWidth: 0
            },
            series: [{
                name: '<?php echo LANG_piece; ?>',
                data: [0]
            }, {
                name: '<?php echo LANG_aqua; ?>',
                data: [0]
            }]
        });

 
         //Set up charts Data
         requestData();

         function requestData() {

              $.get(scr+"?do=temps", function(data)
              {
                    var Temps = data.split("|");

                    if ( Temps[0]=='success' )
                    {
                         date = new Array();
                         date = Temps[1].split(',');

                         temp1 = new Array();
                         temp1 =  Temps[2].split(',');
                         temp1f = new Array();

                         for (var i=0; i < temp1.length; i++) 
                         {
                            if ( !isNaN ( parseFloat(temp1[i]) ) )
                            {
                                 temp1f.push( parseFloat(temp1[i]) );
                            }
                         }

                         temp2 = new Array();
                         temp2 =  Temps[3].split(',');
                         temp2f = new Array();

                         for (var i=0; i < temp2.length; i++) 
                         {
                            if ( !isNaN ( parseFloat(temp2[i]) ) )
                            {
                                 temp2f.push( parseFloat(temp2[i]) );
                            }
                         }

                         chart.xAxis[0].setCategories( date );
                         chart.series[0].setData( temp1f );
                         chart.series[1].setData( temp2f );
                     }
               });

               //refresh charts every 15 Min ( 900 000 milliseconds )
               setTimeout(requestData, 900000);  
         }

        /** chart 2 **/

              $.get(scr+"?do=meteo", function(data)
              {
                    var Temps = data.split("|");

                    if ( Temps[0]=='success' )
                    {
                         date = new Array();
                         date = Temps[1].split(',');

                         t1 = new Array();
                         t1 =  Temps[4].split(',');
                         t1f = new Array();

                         for (var i=0; i < t1.length; i++) 
                         {
                            if ( !isNaN ( parseFloat(t1[i]) ) )
                            {
                                 t1f.push( parseFloat(t1[i]) );
                            }
                         }

                         t2 = new Array();
                         t2 =  Temps[7].split(',');
                         t2f = new Array();

                         for (var i=0; i < t2.length; i++) 
                         {
                            if ( !isNaN ( parseFloat(t2[i]) ) )
                            {
                                 t2f.push( parseFloat(t2[i]) );
                            }
                         }

                         ss = new Array();
                         ss =  Temps[2].split(',');

                         se = new Array();
                         se =  Temps[3].split(',');

                         ssf = new Array();

                         for (var i=0; i < ss.length; i++) 
                         {
                                 ssf[date[i]+t1[i]] =  '<?php echo LANG_de; ?> '+ ss[i] +' <?php echo LANG_to; ?> '+se[i] ;
                         }

                         cs = new Array();
                         cs =  Temps[5].split(',');

                         ce = new Array();
                         ce =  Temps[6].split(',');

                         csf = new Array();

                         for (var i=0; i < cs.length; i++) 
                         {
                                 csf[date[i]+t2[i]] =  '<?php echo LANG_de; ?> '+ cs[i] +' <?php echo LANG_to; ?> '+ce[i] ;
                         }

        var chart2 = new Highcharts.Chart({
            chart: {
                renderTo: 'saison_charts',
                type: 'spline',
                marginRight: 130,
                marginBottom: 25
            },
            title: {
                text: '<?php echo LANG_saison; ?>',
                x: -20 //center
            },
            subtitle: {
                text: '<?php echo LANG_tstorm; ?> - <?php echo LANG_tcloud; ?>',
                x: -20
            },
            xAxis: {
                categories: date
            },
            yAxis: {
                title: {
                    text: '<?php echo LANG_dmin; ?>'
                },
                plotLines: [{
                    value: 0,
                    width: 1,
                    color: '#808080'
                }]
            },
            tooltip: {
                formatter: function() {
                        if ( this.series.name == "<?php echo LANG_tstorm; ?>" )
                        {
                            var n = ssf[this.x+this.y];
                        }
                        else
                        {
                            var n = csf[this.x+this.y];
                        }
                        return '<b>'+ this.series.name +'</b><br/>'+
                        this.x +' (<b>'+ this.y +' <?php echo LANG_min; ?></b>)<br />'+'<br />'+n;
                }
            },
            legend: {
                layout: 'vertical',
                align: 'right',
                verticalAlign: 'top',
                x: -10,
                y: 100,
                borderWidth: 0
            },
            series: [{
                name: '<?php echo LANG_tstorm; ?>',
                data: t1f,
            }, {
                name: '<?php echo LANG_tcloud; ?>',
                data: t2f
            }]
        });

                     }
               });
});
		</script>
    </head>

    <body>
        <div id="main">
           <div class="top left"><img src="images/layout/logo.png"></div>
           <div class="top right"><div class="upper pointer btn pan" pan="1"><img src="images/icons/home.png"> <span><?php echo LANG_dashboard; ?></span></div><div class="upper pointer btn pan" pan="2"><img src="images/icons/lightBulb.png"> <span><?php echo LANG_light; ?></span></div><div class="upper pointer btn pan" pan="3"><img src="images/icons/cog2.png"> <span><?php echo LANG_opts; ?></span></div></div>
          <div class='clear'></div>
          <div class="pannel">
              <div class="info display_none"></div>
              <div class="error display_none"></div>
              <div class="success display_none"></div>
              <div class="warning display_none"></div>
          </div>
          <div class='clear'></div>
          <div class='clear'></div>
          <!-- slider container -->
          <div class='iosSlider'>
	   <!-- slider -->
	   <div class='slider'>
          <!-- slides -->
           <div class='slide view1'>
                <div class='upper'><h2><img src="images/icons/tedit.png" class="left margin_top_10 display_none" id="showdotime"> <span id="time" class="left"><?php echo LANG_datec; ?>:</span><input type="text" id="timedo" class="display_none"></h2></div>
                <div class='clear'></div>
                <div class='upper wbg'><div class="upper pointer temps orange margin5 tp1">?</div> <?php echo LANG_tempb; ?> <div class="upper pointer temps green margin5 tp2">?</div> <?php echo LANG_tempp; ?><div class='clear'></div></div>
                <div class='clear margin'></div>
                <div id="temps_charts"></div>
                <div class='clear margin'></div>
                <div id="saison_charts"></div>
                <div class='clear margin'></div>
          </div>
          <div class='slide view2 display_none'>
          <table id="led">
            <tr>
               <th><?php echo LANG_color; ?></th>
               <th><?php echo LANG_pui; ?></th>
               <th><?php echo LANG_start; ?></th>
               <th></th>
               <th><?php echo LANG_dur; ?></th>
               <th></th>
            </tr>
            <tr>
              <td>
                <span><?php echo LANG_hyb; ?></span>
              </td>
              <td>
                <div class="led"  id="led1">0%</div>
                <div class="ledslider " id="1"></div>
              </td>
              <td>
                  <div class="rangeslider  r1" name="1"></div><div class="h43"></div>
              </td>
              <td>
                  <div id="time1">00:00</div>
              </td>
              <td>
                  <div class="lengthslider  l1" name="1"></div><div class="h43"></div>
              </td>
              <td>
                  <div id="length1">00:00</div>
              </td>
            </tr>
            <tr>
              <td>
                 <span><?php echo LANG_hyb; ?></span>
              </td>
              <td>
                <div class="led" id="led2">0%</div>
                <div class="ledslider " id="2"></div>
              </td>
              <td>
                  <div class="rangeslider  r2" name="2"></div><div class="h43"></div>
              </td>
              <td>
                  <div id="time2">00:00</div>
              </td>
              <td>
                  <div class="lengthslider  l2" name="2"></div><div class="h43"></div>
              </td>
              <td>
                  <div id="length2">00:00</div>
              </td>
            </tr>
            <tr>
              <td>
                 <span ><?php echo LANG_hyb; ?></span>
              </td>
              <td>
                <div class="led" id="led3">0%</div>
                <div class="ledslider " id="3"></div>
              </td>
              <td>
                  <div class="rangeslider  r3" name="3"></div><div class="h43"></div>
              </td>
              <td>
                  <div id="time3">00:00</div>
              </td>
              <td>
                  <div class="lengthslider  l3" name="3"></div><div class="h43"></div>
              </td>
              <td>
                  <div id="length3">00:00</div>
              </td>
            </tr>
            <tr>
              <td>
                 <span><?php echo LANG_white; ?></span>
              </td>
              <td>
                <div class="led" id="led4">0%</div>
                <div class="ledslider " id="4"></div>
              </td>
              <td>
                  <div class="rangeslider  r4" name="4"></div><div class="h43"></div>
              </td>
              <td>
                  <div id="time4">00:00</div>
              </td>
              <td>
                  <div class="lengthslider  l4" name="4"></div><div class="h43"></div>
              </td>
              <td>
                  <div id="length4">00:00</div>
              </td>
            </tr>
            <tr>
              <td>
                 <span><?php echo LANG_white; ?></span>
              </td>
              <td>
                <div class="led" id="led5">0%</div>
                <div class="ledslider " id="5"></div>
              </td>
              <td>
                  <div class="rangeslider  r5" name="5"></div><div class="h43"></div>
              </td>
              <td>
                  <div id="time5">00:00</div>
              </td>
              <td>
                  <div class="lengthslider  l5" name="5"></div><div class="h43"></div>
              </td>
              <td>
                  <div id="length5">00:00</div>
              </td>
            </tr>
            <tr>
              <td>
                 <span><?php echo LANG_blue ?></span>
              </td>
              <td>
                <div class="led" id="led6">0%</div>
                <div class="ledslider " id="6"></div>
              </td>
              <td>
                  <div class="rangeslider  r6" name="6"></div><div class="h43"></div>
              </td>
              <td>
                  <div id="time6">00:00</div>
              </td>
              <td>
                  <div class="lengthslider  l6" name="6"></div><div class="h43"></div>
              </td>
              <td>
                  <div id="length6">00:00</div>
              </td>
            </tr>
           </table>
           <div class="opts"><div class="upper pointer manoff man red"><img src="images/icons/switcher.png"> <?php echo LANG_man_off; ?></div><div class="upper pointer manon display_none man green"><img src="images/icons/switcher.png"> <?php echo LANG_man_on; ?></div> <div class="upper pointer stormoff storms red"><img src="images/icons/power.png"> <?php echo LANG_sim_or_off; ?></div><div class="upper pointer stormon display_none storms green" ><img src="images/icons/power.png"> <?php echo LANG_sim_or_on; ?></div> <div class="upper pointer cloudoff clouds red"><img src="images/icons/cloud.png"> <?php echo LANG_sim_c_off; ?></div><div class="upper pointer cloudon display_none clouds green"><img src="images/icons/cloud.png"> <?php echo LANG_sim_c_on; ?></div></div>
       </div>
           <div class='slide view3 display_none'>
             <div class="opts">
                    <div class='upper'><h2><span id="time"><?php echo LANG_oscillo_title; ?></span></h2></div>
          <table>
            <tr>
               <th><?php echo LANG_state; ?></th>
               <th><?php echo LANG_angle; ?></th>
               <th></th>
               <th><?php echo LANG_vitesse; ?></th>
               <th></th>
            </tr>
            <tr>
              <td>
                  <div class="upper pointer svoon btn2 green display_none"><img src="images/icons/transfer.png"> <?php echo LANG_svo_on; ?></div><div class="upper pointer svooff btn2 red"><img src="images/icons/transfer.png"> <?php echo LANG_svo_off; ?></div>
              </td>
              <td>
                  <div class="angle a1" name="1"></div><div class="h43"></div>
              </td>
              <td>
                  <div id="angle1">10°</div>
              </td>
              <td>
                  <div class="speed s1" name="1"></div><div class="h43"></div>
              </td>
              <td>
                  <div id="speed1">1</div>
              </td>
            </tr>
            <tr>
              <td>
              <div class="upper pointer svo2on btn2 green display_none"><img src="images/icons/transfer.png"> <?php echo LANG_svo2_on; ?></div><div class="upper pointer svo2off btn2 red"><img src="images/icons/transfer.png"> <?php echo LANG_svo2_off; ?></div>
              </td>
              <td>
                  <div class="angle a2" name="2"></div><div class="h43"></div>
              </td>
              <td>
                  <div id="angle2">10°</div>
              </td>
              <td>
                  <div class="speed s2" name="2"></div><div class="h43"></div>
              </td>
              <td>
                  <div id="speed2">1</div>
              </td>
            </tr>

          </table>

                    <div class='clear margin'></div>
                    <div class='upper'><h2><span id="alarm"><?php echo LANG_al; ?></span></h2></div>
          <table>
            <tr>
               <th><?php echo LANG_state; ?></th>
               <th><?php echo LANG_min; ?></th>
               <th></th>
               <th><?php echo LANG_max; ?></th>
            </tr>
            <tr>
              <td>
                    <div class="upper pointer alarmon btn2 green display_none"><img src="images/icons/alarm.png"> <?php echo LANG_al_on; ?></div>
                    <div class="upper pointer alarmoff btn2 red"><img src="images/icons/alarm.png"> <?php echo LANG_al_off; ?></div>
              </td>
              <td>
                  <div><span id="al1"><?php if( CONFIG_TEMPS == 1 ) { echo '22'; }else{ echo '71,6'; }?></span><?php if( CONFIG_TEMPS == 1 ) { echo '°C'; }else{ echo '°F'; }?></div>
              </td>
              <td>
                  <div class="alm alm1" name="1"></div><div class="h43"></div>
              </td>
              <td>
                  <div><span id="al12"><?php if( CONFIG_TEMPS == 1 ) { echo '30'; }else{ echo '104'; }?></span><?php if( CONFIG_TEMPS == 1 ) { echo '°C'; }else{ echo '°F'; }?></div>
              </td>
            </tr>
            <tr>
              <td>
                    <div class="upper pointer alarm2on btn2 green display_none"><img src="images/icons/alarm.png"> <?php echo LANG_al2_on; ?></div>
                    <div class="upper pointer alarm2off btn2 red"><img src="images/icons/alarm.png"> <?php echo LANG_al2_off; ?></div>
              </td>
              <td>
                  <div><span id="al2"><?php if( CONFIG_TEMPS == 1 ) { echo '22'; }else{ echo '71,6'; }?></span><?php if( CONFIG_TEMPS == 1 ) { echo '°C'; }else{ echo '°F'; }?></div>
              </td>
              <td>
                  <div class="alm alm2" name="2"></div><div class="h43"></div>
              </td>
              <td>
                  <div><span id="al22"><?php if( CONFIG_TEMPS == 1 ) { echo '30'; }else{ echo '104'; }?></span><?php if( CONFIG_TEMPS == 1 ) { echo '°C'; }else{ echo '°F'; }?></div>
              </td>
            </tr>

          </table>

                    <div class='upper'><h2><span id="autosc"><?php echo LANG_asc_title; ?></span></h2></div>
                    <div class="upper pointer acon btn2 green"><img src="images/icons/cloud.png"> <?php echo LANG_ac_on; ?></div>
                    <div class="upper pointer acoff btn2 display_none red"><img src="images/icons/cloud.png"> <?php echo LANG_ac_off; ?></div>
                    <div class="upper pointer ason btn2 green"><img src="images/icons/power.png"> <?php echo LANG_as_on; ?></div>
                    <div class="upper pointer asoff btn2 display_none red"><img src="images/icons/power.png"> <?php echo LANG_as_off; ?></div>

                    <div class='clear margin'></div>
                    <div class='upper'><h2><span id="restart"><?php echo LANG_other_title; ?></span></h2></div>
                    <div class="upper pointer reset btn2" id="1"><img src="images/icons/logoff.png"> <?php echo LANG_restart; ?></div>
             </div>
          </div>
	</div>
       </div>
       </div>
       <span id="conft" class="display_none"><?php if( CONFIG_TEMPS == 1 ) { echo 'c'; }else{ echo 'f'; }?></span>
    </body>

</html>


