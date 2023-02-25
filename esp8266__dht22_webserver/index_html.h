
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p.measure { font-size: 3.0rem; }
    p.small { font-size: 0.8rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>GrowBox bHome</h2>
  <p class="small">
  Il riscladamento rimane attivo per %heatingActiveForSeconds% secondi ed ha un periodo di inattivit&aacute; minimo di %heatingIdleForSeconds% secondi (all'avvio sar&aacute; inattivo per %heatingIdleForSeconds% secondi).
  </p>
  <p class="small">
  Range temperatura: %temperatureMin% &deg;C / %temperatureMax% &deg;C (&plusmn; %temperatureTol% &deg;C).
  </p>
  <p class="measure">
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperatura</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p class="measure">
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Umidit&#224</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&#37;</sup>
  </p>
  <p class="measure">
    <i class="fas fa-fire" style="color:#ff6600;"></i> 
    <span class="dht-labels">Riscaldamento</span>
    <span id="rele1">%RELE1%</span>
  </p>
   <p class="measure">
    <i class="fas fa-fire" style="color:#ff6600;"></i> 
    <span class="dht-labels">Illuminazione</span>
    <span id="rele2">%RELE2%</span>
  </p>
  

</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("rele1").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/rele1", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("rele2").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/rele2", true);
  xhttp.send();
}, 1000 ) ;
</script>
</html>)rawliteral";