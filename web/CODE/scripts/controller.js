/*
+---------------------------------------------------------------------------
|   > Copyright (c) 2013
|   > Bourgeois Jérôme
+---------------------------------------------------------------------------
|   > Controller JS
|   > Module written by Bourgeois Jérôme
|   > mail.bjerome@gmail.com
----------------------------------------------
|   > Date started: Aug 2, 2012
|   > Release updated: Jan 28, 2013 - 10:07 PM
+---------------------------------------------
|   > Module Version Number: 0.0.0.5
+---------------------------------------------
|   > Web Reef Controller
+--------------------------------------------------------------------------
*/
      //temps
      date = 0;
      temp1 = 0;
      temp2 = 0;
      controllerDate = 0;
      var scr = "functions.php";

$(function(){

      //init led value
      led1 = 0;

     $(".info").html("Connection au controlleur arduino...").fadeIn("slow");
     setTimeout( function(){ $(".info").fadeOut("slow"); }, 2000 );

     //Value On load
     $.get(scr+"?do=udp&func=6&val=0&to=0", function(data)
     {
          var Response = data.split("|");

          if ( Response[0] == "true" )
          {
              //Slider LEDS PERCENTAGE
              var i = 1;

              while ( i < 7 )
              {
                  if ( Response[i+1] == '00' ) { Response[i+1] = 0; }
                  $("#"+i).slider({ value: Response[i+1] });
                  $("#led"+i).html( Response[i+1]+"%" );
                  i++;
              }

              //Slider LEDS START TIME

              var j = 7;
              var i = 1;

              while ( j < 13 )
              {
                  var hours = Math.floor(Response[j+1] / 60);
                  var minutes = Math.floor( ( Response[j+1] - ( hours * 60 ) ) );

                  if(hours<10) hours = '0'+hours;
                  if(minutes<10) minutes = '0'+minutes;

                  $(".r"+i).slider({ value: Response[j+1] });
                  $("#time"+i).html( hours+':'+minutes );           

                  j++;
                  i++;
              }

              //Slider LEDS LENGTH

              var j = 14;

              while ( j < 20 )
              {
                  if ( i == 7 ) { i = 1; }

                  var hours = Math.floor(Response[j] / 60);
                  var minutes = Math.floor( ( Response[j] - ( hours * 60 ) ) );

                  if(hours<10) hours = '0'+hours;
                  if(minutes<10) minutes = '0'+minutes;

                  $(".l"+i).slider({ value: Response[j] });
                  $("#length"+i).html( hours+':'+minutes );                 

                  j++;
                  i++;
              }

              //time
              $("#time").html( "<h2>Date du Controlleur: "+Response[22]+"</h2>" ); 

              $("#timedo").hide(); 
              $("#showdotime").show();
              $("#showdotime").click(function() { 

              $("#timedo").show();

              var d = new Date();
              var n = d.getUTCDay(); 

              $('#timedo').datetimepicker({
	           addSliderAccess: true,
	           sliderAccessArgs: { touchonly: false },
                  timeFormat: 'HH:mm:ss',
                  dateFormat: 'dd:mm:yy:'+n+':',
	           onClose: function(dateText, inst) {
                       $("#timedo").hide();
		         if ($('#timedo').val() != '') {
                        var ss = dateText.replace(/\s+/g,"");
                        var elem = ss.split(':');
                        controllerDate = elem[6]+','+elem[5]+','+elem[4]+','+elem[3]+','+elem[1]+','+elem[0];
                        year = elem[2] - 2000;
                     }
	           },
              });

                    jQuery.datepicker._gotoToday('#timedo');

                    $("#timedo").datetimepicker( "show" );

                    $(document).on('click','.ui-datepicker-close',function() { 
                        $.post(scr+"?do=udp&func=12&val=0&to="+year+"&date="+controllerDate);
                    });
              });

              //storm
              if ( Response[20] == 1 )
              {
                   $( ".stormon" ).show();
                   $( ".stormoff" ).hide();
              }  

              //cloud
              if ( Response[21] == 1 )
              {
                   $( ".cloudon" ).show();
                   $( ".cloudoff" ).hide();
              }  

              //manuel
              if ( Response[23] == 1 )
              {
                   $( ".manon" ).show();
                   $( ".manoff" ).hide();
              } 

              //servo 1 on/off
              if ( Response[24] == 1 )
              {
                   $( ".svoon" ).show();
                   $( ".svooff" ).hide();
              } 

              //servo 2 on/off
              if ( Response[25] == 1 )
              {
                   $( ".svo2on" ).show();
                   $( ".svo2off" ).hide();
              } 

              //auto mode cloud on/off
              if ( Response[32] == 0 )
              {
                   $( ".acoff" ).show();
                   $( ".acon" ).hide();
              } 

              //auto mode storm on/off
              if ( Response[33] == 0 )
              {
                   $( ".asoff" ).show();
                   $( ".ason" ).hide();
              } 

              //alarm on/off
              if ( Response[34] == 1 )
              {
                   $( ".alarmon" ).show();
                   $( ".alarmoff" ).hide();
              } 

              //alarm 2 on/off
              if ( Response[35] == 1 )
              {
                   $( ".alarm2on" ).show();
                   $( ".alarm2off" ).hide();
              } 

              //alarm temps
              var i = 1;

              while ( i < 3 )
              {
                  if ( i == 1 )
                  {
                       $( ".alm"+i ).slider({ values: [Response[i+36],Response[i+35]] });
                       $("#al"+i).html( Response[i+36] );
                       $("#al"+i+"2").html( Response[i+35] );
                  }
                  else
                  {
                       $( ".alm"+i ).slider({ values: [Response[i+37],Response[i+36]] });
                       $("#al"+i).html( Response[i+37] );
                       $("#al"+i+"2").html( Response[i+36] );
                  }
                  i++;
              }

              //servo angle
              var i = 1;

              while ( i < 3 )
              {
                  $(".a"+i).slider({ value: Response[i+25] });
                  $("#angle"+i).html( Response[i+25]+"°" );
                  i++;
              }

              //servo speed
              var i = 1;

              while ( i < 3 )
              {
                  $(".s"+i).slider({ value: Response[i+27] });
                  $("#speed"+i).html( Response[i+27] );
                  i++;
              }

              $( ".tp2" ).html(Response[30]);
              $( ".tp1" ).html(Response[31]);
              $(".info").hide();
              $(".success").html(Response[1]).fadeIn("slow");
              setTimeout( function(){ $(".success").fadeOut("slow"); }, 2000 );
          }
          else
          {
              $(".info").hide();
              $(".error").html(Response[1]).fadeIn("slow");
              setTimeout( function(){ $(".error").fadeOut("slow"); }, 4000 );
          }
     });

             //Slider LEDS PERCENTAGE
             $( ".ledslider" ).slider({range: 'min'}, { max: 100 },{ value: led1 },{step : 1});

             $( ".ledslider" ).delay(10).bind( "slide", function() 
             {  
                 slideTouch = 1;
                 var sid = $(this).attr('id');
                 var val = $( "#"+sid ).slider( "option", "value" );
                 //Show value
                 $("#led"+sid).html(val+"%");
             });

             //New val
             $( ".ledslider" ).delay(100).bind( "slidestop", function() {
                var func = 1;
                var sid = $(this).attr('id');
                var val = $( "#"+sid ).slider( "option", "value" );
                $.post(scr+"?do=udp&func=1&val="+val+"&to="+sid);
                $("#led"+sid).html(val+"%");  
            });

    /** LED PROGR HOURS / MINS **/
    //Slider
    $( ".rangeslider" ).slider({range: 'min'},{ max: 1440 },{ value: 0 },{step : 1});

    $( ".rangeslider" ).delay(100).bind( "slide", function() {

            slideTouch = 1;
            var sid = $(this).attr('name');
            var val = $( ".r"+sid ).slider( "option", "value" );

            var hours = Math.floor(val / 60);
            var minutes = Math.floor( ( val - ( hours * 60 ) ) );

            if(hours<10) hours = '0'+hours;
            if(minutes<10) minutes = '0'+minutes;

            $('#time'+sid).html(hours+':'+minutes);
    });

    $( ".rangeslider" ).delay(100).bind( "slidestop", function() {

            var sid = $(this).attr('name');
            var val = $( ".r"+sid ).slider( "option", "value" );

            var hours = Math.floor(val / 60);
            var minutes = Math.floor( ( val - ( hours * 60 ) ) );

            if(hours<10) hours = '0'+hours;
            if(minutes<10) minutes = '0'+minutes;

            $('#time'+sid).html(hours+':'+minutes);
            $.post(scr+"?do=udp&func=2&val="+val+"&to="+sid);
    });

    /** LED PROGR LENGTH **/
    $( ".lengthslider" ).slider({range: 'min'},{ max: 1440 },{ value: 0 },{step : 1});

    $( ".lengthslider" ).delay(100).bind( "slide", function() {

            slideTouch = 1;
            var sid = $(this).attr('name');
            var val = $( ".l"+sid ).slider( "option", "value" );

            var hours = Math.floor(val / 60);
            var minutes = Math.floor( ( val - ( hours * 60 ) ) );

            if(hours<10) hours = '0'+hours;
            if(minutes<10) minutes = '0'+minutes;

            $('#length'+sid).html(hours+':'+minutes);
    });

    $( ".lengthslider" ).delay(100).bind( "slidestop", function() {

            var sid = $(this).attr('name');
            var val = $( ".l"+sid ).slider( "option", "value" );

            var hours = Math.floor(val / 60);
            var minutes = Math.floor( ( val - ( hours * 60 ) ) );
            if(hours<10) hours = '0'+hours;
            if(minutes<10) minutes = '0'+minutes;

            $('#length'+sid).html(hours+':'+minutes);
            $.post(scr+"?do=udp&func=3&val="+val+"&to="+sid);
    });

    /** SERVO PROGR ANGLE **/
    $( ".angle" ).slider({range: 'min'},{ min: 10 }, { max: 180 },{ value: 0 },{step : 1});

    $( ".angle" ).delay(100).bind( "slide", function() {

            slideTouch = 1;
            var sid = $(this).attr('name');
            var val = $( ".a"+sid ).slider( "option", "value" );

            $('#angle'+sid).html(val+'°');
    });

    $( ".angle" ).delay(100).bind( "slidestop", function() {

            var sid = $(this).attr('name');
            var val = $( ".a"+sid ).slider( "option", "value" );

            $('#angle'+sid).html(val+'°');
            $.post(scr+"?do=udp&func=10&val="+val+"&to="+sid);
    });

    /** SERVO PROGR SPEED **/
    $( ".speed" ).slider({range: 'min'},{ min: 1 },{ max: 10 },{ value: 0 },{step : 1});

    $( ".speed" ).delay(100).bind( "slide", function() {

            slideTouch = 1;
            var sid = $(this).attr('name');
            var val = $( ".s"+sid ).slider( "option", "value" );

            $('#speed'+sid).html(val);
    });

    $( ".speed" ).delay(100).bind( "slidestop", function() {

            var sid = $(this).attr('name');
            var val = $( ".s"+sid ).slider( "option", "value" );

            $('#speed'+sid).html(val);
            $.post(scr+"?do=udp&func=11&val="+val+"&to="+sid);
    });

             //Slider ALARM
              var vr = $( "#conft" ).text() == 'c' ? new Array("22","30") : new Array("71.6","86");
              var mx = $( "#conft" ).text() == 'c' ? 40 : 104;
              var st = $( "#conft" ).text() == 'c' ? 1 : 0.1;

             $( ".alm" ).slider({range: true}, {min: 0}, { max: mx },{ values: vr },{step : st});

             $( ".alm" ).delay(100).bind( "slide", function(event, ui) 
             {  
                 slideTouch = 1;
                 var sid = $(this).attr('name');
                 var val = $( ".alm"+sid ).slider( "values", 0 );
                 var val2 = $( ".alm"+sid ).slider( "values", 1 );

                 //Show value
                 $("#al"+sid).html(val);
                 $("#al"+sid+"2").html(val2);
             });

             $( ".alm" ).delay(100).bind( "slidestop", function(event, ui) 
             {  
                 var sid = $(this).attr('name');
                 var val = $( ".alm"+sid ).slider( "values", 0 );
                 var val2 = $( ".alm"+sid ).slider( "values", 1 );

                 $('#al'+sid).html(val);
                 $("#al"+sid+"2").html(val2);
                 $.post(scr+"?do=udp&func=15&val="+val+"&to="+sid);
                 $.post(scr+"?do=udp&func=15&val="+val2+"&to="+sid+"2");
             });

    $( ".stormoff" ).click(function(){
          $( ".stormon" ).show();
          $( ".stormoff" ).hide();
          $.post(scr+"?do=udp&func=4&val=1&to=0");
    });

    $( ".stormon" ).click(function(){
          $( ".stormoff" ).show();
          $( ".stormon" ).hide();
          $.post(scr+"?do=udp&func=4&val=2&to=0");
    });

    $( ".cloudoff" ).click(function(){
          $( ".cloudon" ).show();
          $( ".cloudoff" ).hide();

          $.post(scr+"?do=udp&func=5&val=1&to=0");
    });
    $( ".cloudon" ).click(function(){
          $( ".cloudoff" ).show();
          $( ".cloudon" ).hide();
          $.post(scr+"?do=udp&func=5&val=2&to=0");
    });

    $( ".manoff" ).click(function(){
          $( ".manon" ).show();
          $( ".manoff" ).hide();
          $.post(scr+"?do=udp&func=7&val=1&to=0");
    });
    $( ".manon" ).click(function(){
          $( ".manoff" ).show();
          $( ".manon" ).hide();
          $.post(scr+"?do=udp&func=7&val=2&to=0");
    });

    $( ".svooff" ).click(function(){
          $( ".svoon" ).show();
          $( ".svooff" ).hide();
          $.post(scr+"?do=udp&func=9&val=1&to=1");
    });
    $( ".svoon" ).click(function(){
          $( ".svooff" ).show();
          $( ".svoon" ).hide();
          $.post(scr+"?do=udp&func=9&val=0&to=1");
    });

    $( ".svo2off" ).click(function(){
          $( ".svo2on" ).show();
          $( ".svo2off" ).hide();
          $.post(scr+"?do=udp&func=9&val=1&to=2");
    });
    $( ".svo2on" ).click(function(){
          $( ".svo2off" ).show();
          $( ".svo2on" ).hide();
          $.post(scr+"?do=udp&func=9&val=0&to=2");
    });

    $( ".acoff" ).click(function(){
          $( ".acon" ).show();
          $( ".acoff" ).hide();
          $.post(scr+"?do=udp&func=13&val=1&to=1");
    });
    $( ".acon" ).click(function(){
          $( ".acoff" ).show();
          $( ".acon" ).hide();
          $.post(scr+"?do=udp&func=13&val=0&to=1");
    });

    $( ".asoff" ).click(function(){
          $( ".ason" ).show();
          $( ".asoff" ).hide();
          $.post(scr+"?do=udp&func=13&val=1&to=2");
    });
    $( ".ason" ).click(function(){
          $( ".asoff" ).show();
          $( ".ason" ).hide();
          $.post(scr+"?do=udp&func=13&val=0&to=2");
    });

    $( ".alarmoff" ).click(function(){
          $( ".alarmon" ).show();
          $( ".alarmoff" ).hide();
          $.post(scr+"?do=udp&func=14&val=1&to=1");
    });
    $( ".alarmon" ).click(function(){
          $( ".alarmoff" ).show();
          $( ".alarmon" ).hide();
          $.post(scr+"?do=udp&func=14&val=0&to=1");
    });

    $( ".alarm2off" ).click(function(){
          $( ".alarm2on" ).show();
          $( ".alarm2off" ).hide();
          $.post(scr+"?do=udp&func=14&val=1&to=2");
    });
    $( ".alarm2on" ).click(function(){
          $( ".alarm2off" ).show();
          $( ".alarm2on" ).hide();
          $.post(scr+"?do=udp&func=14&val=0&to=2");
    });
    
    $( ".reset" ).click(function(){
          $.post(scr+"?do=udp&func=8&val=1&to=0");
    });

    /** iOS slider **/
    $( ".pan" ).click(function(){
           
           var id = $(this).attr("pan");
           $(".slide").hide("fast");
           $(".view"+id).show("fast");
    });
});