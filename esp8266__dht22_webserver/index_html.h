
const char index_html[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="it">

<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css"
    integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.3.1/dist/css/bootstrap.min.css"
    integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
  <title>B-Grow</title>
  <style>
    fieldset {
      padding: 8px;
      margin-bottom: 8px;
    }

    .measure {
      font-size: 2em;
      padding-bottom: 4px;
      ;
    }
  </style>
</head>

<body>
  <div class="container-fluid">
    <div class="row">
      <div class="col-sm-8">
        <div class="card mb-3">
          <div class="card-header bg-info text-white">Situazione in tempo reale</div>
          <div class="card-body">
            <p>
              Riscladamento attivo per %heatingActiveForSeconds% secondi (inattivit&aacute; minima %heatingIdleForSeconds% secondi).
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
              <i class="fas fa-fire" style="color:#ff1100;"></i>
              <span class="dht-labels">Riscaldamento</span>
              <span id="rele1">%RELE1%</span><small id="rele1idle">%RELE1IDLE%</small>
            </p>
            <p class="measure">
              <i class="fas fa-lightbulb" style="color:#ffbb00;"></i>
              <span class="dht-labels">Illuminazione</span>
              <span id="rele2">%RELE2%</span>
            </p>

          </div>
        </div>
      </div>
      <div class="col-sm-4">
        <div class="card mb-3">
          <div class="card-header bg-warning">Impostazioni</div>
          <div class="card-body">
            <form action="/settings" method="post">
              <div id="accordion" class="mb-4">
                <div class="card">
                  <div class="card-header" id="lightning">
                    <h5 class="mb-0">
                      <a class="btn btn-link" data-toggle="collapse" data-target="#collapseLightning"
                        aria-expanded="true" aria-controls="collapseLightning" type="button">
                        Illuminazione
                      </a>
                    </h5>
                  </div>

                  <div id="collapseLightning" class="collapse" aria-labelledby="lightning" data-parent="#accordion">
                    <div class="card-body">
                      <div class="form-group">
                        <label for="lightManual">Attivazione</label>
                        <select class="form-control" id="lightManual" name="lightManual">
                          <option value="0">Automatico</option>
                          <option value="1">Manuale</option>
                        </select>
                      </div>
                      <div class="form-row">
                        <div class="form-group col-md-6">
                          <label for="lightManualOnTime">Accensione</label>
                          <input type="time" class="form-control" id="lightManualOnTime" name="lightManualOnTime"
                            aria-describedby="lightManualOnTimeHelp" value="%lightManualOnTime%">
                          <small id="lightManualOnTimeHelp" class="form-text text-muted">L'orario di accensione delle
                            luci.</small>
                        </div>
                        <div class="form-group col-md-6">
                          <label for="lightManualOffTime">Spegnimento</label>
                          <input type="time" class="form-control" id="lightManualOffTime" name="lightManualOffTime"
                            aria-describedby="lightManualOffTimeHelp">
                          <small id="lightManualOffTimeHelp" class="form-text text-muted">L'orario di spegnimento delle
                            luci.</small>
                        </div>
                      </div>
                    </div>
                  </div>
                </div>
                <div class="card">
                  <div class="card-header" id="temperature">
                    <h5 class="mb-0">
                      <a class="btn btn-link" data-toggle="collapse" data-target="#collapseTemperature"
                        aria-expanded="true" aria-controls="collapseTemperature" type="button">
                        Temperatura
                      </a>
                    </h5>
                  </div>
                  <div id="collapseTemperature" class="collapse" aria-labelledby="temperature" data-parent="#accordion">
                    <div class="card-body">
                      <div class="form-group">
                        <label for="tempActive">Gestione attiva</label>
                        <select class="form-control" id="tempActive" name="tempActive">
                          <option value="0">Disattivata</option>
                          <option value="1">Attiva</option>
                        </select>
                      </div>
                      <div class="form-row">
                        <div class="form-group col-md-6">
                          <label for="tempMin">Temp. minima</label>
                          <input type="number" class="form-control" id="tempMin" name="tempMin" value="%temperatureMin%"
                            aria-describedby="tempMinHelp">
                          <small id="tempMinHelp" class="form-text text-muted">Al di sotto di questa temperatura si
                            attiva
                            il
                            riscaldamento.</small>
                        </div>
                        <div class="form-group col-md-6">
                          <label for="tempMax">Temp. massima</label>
                          <input type="number" class="form-control" id="tempMax" name="tempMax" value="%temperatureMax%"
                            aria-describedby="tempMaxHelp">
                          <small id="tempMaxHelp" class="form-text text-muted">Al di sopra di questa temperatura si
                            attiva
                            il
                            raffreddamento.</small>
                        </div>
                      </div>
                      <div class="form-row">
                        <div class="form-group col-md-6">
                          <label for="tempMinNight">Temp. minima (notte)</label>
                          <input type="number" class="form-control" id="tempMinNight" name="tempMinNight" value="%temperatureMinNight%"
                            aria-describedby="tempMinHelp">
                          <small id="tempMinHelp" class="form-text text-muted">Al di sotto di questa temperatura si
                            attiva
                            il
                            riscaldamento.</small>
                        </div>
                        <div class="form-group col-md-6">
                          <label for="tempMaxNight">Temp. massima (notte)</label>
                          <input type="number" class="form-control" id="tempMaxNight" name="tempMaxNight" value="%temperatureMaxNight%"
                            aria-describedby="tempMaxHelp">
                          <small id="tempMaxHelp" class="form-text text-muted">Al di sopra di questa temperatura si
                            attiva
                            il
                            raffreddamento.</small>
                        </div>
                      </div>
                    </div>
                  </div>
                </div>
                <div class="card">
                  <div class="card-header" id="umidity">
                    <h5 class="mb-0">
                      <a class="btn btn-link" data-toggle="collapse" data-target="#collapseUmidity" aria-expanded="true"
                        aria-controls="collapseUmidity" type="button">
                        Umidita
                      </a>
                    </h5>
                  </div>
                  <div id="collapseUmidity" class="collapse" aria-labelledby="umidity" data-parent="#accordion">
                    <div class="card-body">
                      <div class="form-check">
                        <input type="checkbox" class="form-check-input" id="umidActive" name="umidActive">
                        <label class="form-check-label" for="rele2Manual">Gestione attiva</label>
                      </div>
                      <div class="form-row">
                        <div class="form-group col-md-6">
                          <label for="umidMin">Umid. minima</label>
                          <input type="number" class="form-control" id="umidMin" name="umidMin"
                            aria-describedby="umidMinHelp">
                          <small id="umidMinHelp" class="form-text text-muted">Al di sotto di questa umidità si attiva
                            l'umidificatore.</small>
                        </div>
                        <div class="form-group col-md-6">
                          <label for="umidMax">Umid. massima</label>
                          <input type="number" class="form-control" id="umidMax" name="umidMax"
                            aria-describedby="umidMaxHelp">
                          <small id="umidMaxHelp" class="form-text text-muted">Al di sopra di questa umidità si forza
                            l'attivatore dell'estrattore d'aria.</small>
                        </div>
                      </div>
                    </div>
                  </div>
                </div>
              </div>
              <div class="text-right">
                <button type="submit" value="Submit" class="btn btn-danger">Salva configurazione</button>
              </div>
            </form>
          </div>
        </div>
      </div>
    </div>
  </div>

  <script>
    setInterval(function () {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("temperature").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "/temperature", true);
      xhttp.send();
    }, 1000);

    setInterval(function () {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("humidity").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "/humidity", true);
      xhttp.send();
    }, 1000);

    setInterval(function () {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("rele1").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "/rele1", true);
      xhttp.send();
    }, 1000);

    setInterval(function () {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("rele1idle").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "/rele1idle", true);
      xhttp.send();
    }, 1000);

    setInterval(function () {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("rele2").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "/rele2", true);
      xhttp.send();
    }, 1000);

    function selectElement(id, valueToSelect) {
      let element = document.getElementById(id);
      element.value = valueToSelect;
    }
    selectElement('lightManual', '%lightManual%');
    selectElement('lightManualOnTime', '%lightManualOnTime%');
    selectElement('lightManualOffTime', '%lightManualOffTime%');
    selectElement('tempActive', '%tempActive%');
  </script>

  <script src="https://code.jquery.com/jquery-3.2.1.slim.min.js"
    integrity="sha384-KJ3o2DKtIkvYIK3UENzmM7KCkRr/rE9/Qpg6aAZGJwFDMVNA/GpGFF93hXpG5KkN"
    crossorigin="anonymous"></script>
  >
  <script src="https://cdn.jsdelivr.net/npm/bootstrap@4.0.0/dist/js/bootstrap.min.js"
    integrity="sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl"
    crossorigin="anonymous"></script>
</body>

</html>)rawliteral";