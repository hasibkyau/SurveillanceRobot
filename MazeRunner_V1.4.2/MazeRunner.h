String myPATH(int endPoint) {
  if(1==endPoint){return "F";}
  else if(2==endPoint){return "FLF";}
  else if(3==endPoint){return "FFFF";}
  else if(4==endPoint){return "FFFFLF";}
  else if(5==endPoint){return "FRRF";}
  else if(6==endPoint){return "FFF";}
  else if(7==endPoint){return "FFFFRF";}
  else if(8==endPoint){return "FFFFRFLF";}
  else if(9==endPoint){return "FRFF";}
  else if(10==endPoint){return "FRLF";}
  else if(11==endPoint){return "FRLFF";}
  else if(12==endPoint){return "FFFFRFFF";}
  else return "0";
}


String myHTML(int cp, int sp, int ep, String PATH) {
  String ptr = "<!DOCTYPE html>";
  ptr += "<html><head>";
  ptr += "<title>Maze Robot</title>";

  ptr += "<link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/bootstrap@4.3.1/dist/css/bootstrap.min.css' integrity='sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T' crossorigin='anonymous'>";
  ptr += "<script src='https://code.jquery.com/jquery-3.3.1.slim.min.js' integrity='sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo' crossorigin='anonymous'></script>";
  ptr += "<script src='https://cdn.jsdelivr.net/npm/popper.js@1.14.7/dist/umd/popper.min.js' integrity='sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1' crossorigin='anonymous'></script>";
  ptr += "<script src='https://cdn.jsdelivr.net/npm/bootstrap@4.3.1/dist/js/bootstrap.min.js' integrity='sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM' crossorigin='anonymous'></script>";


  ptr += "<meta name=' viewport ' content=' width = device - width, initial - scale = 1 '>";
  ptr += "<style>";
  ptr += ".button{\n";
  ptr += "background-color: green;";
  ptr += "border: none;";
  ptr += "color: white;";
  ptr += "padding: 15px 32px;";
  ptr += "text-align: center;";
  ptr += "text-decoration: none;";
  ptr += "display: flex;";
  ptr += "font-size: 16px;";
  ptr += "margin: 4px 2px;";
  ptr += "cursor: pointer;";
  ptr += "}\n";
  ptr += "</style>";
  ptr += "</head><body>";

  ptr += "<div class='container'>";
  ptr += "<h2 class='m-3'>Autonomous Maze Solving Robot</h2>";


  ptr += "<div class='row'>";

  ptr += "<div class='col'>";
  ptr += "</form><br>";
  ptr += "<form action='/start'>";
  ptr += "<button type='submit' class='btn btn-success'>Start</button>";
  ptr += " </form>";
  ptr += "</div>";

  ptr += "<div class='col'>";
  ptr += "</form><br>";
  ptr += "<form action='/stop'>";
  ptr += "<button type='submit' class='btn btn-danger'>Stop</button>";
  ptr += " </form>";
  ptr += "</div>";

  ptr += "</div>";

  ptr += "<div class='row'>";

  ptr += "<div class='col'>";
  ptr += "<span class = 'input-group-text'>Current Point:";
  ptr += int(cp);
  ptr += "</ span>";
  ptr += "</div>";

  ptr += "<div class='col'>";
  ptr += "<span class = 'input-group-text'>Start Point:";
  ptr += int(sp);
  ptr += "</ span>";
  ptr += "</div>";


  ptr += "</div>";

  ptr += "<div class='row'>";

  ptr += "<div class='col'>";
  ptr += "<span class = 'input-group-text'>Destination Point:";
  ptr += int(ep);
  ptr += "</ span>";
  ptr += "</div>";

  ptr += "</div>";

  ptr += "<div class='row'>";
  ptr += "<div class='col'>";
  ptr += "<form action='/decrement'>";
  ptr += "<button type='submit' class='btn btn-warning'>Prev.</button>";
  ptr += "</form><br>";
  ptr += "</div>";

  ptr += "<div class='col'>";
  ptr += "<form action='/increment'>";
  ptr += "<button  type='submit' class='btn btn-primary'>Next</button>";
  ptr += " </form>";
  ptr += "</div>";
  ptr += "</div>";

  ptr += "<div class='row'>";

  ptr += "<div class='col'>";
  ptr += "<span class = 'input-group-text'>Shortest Path:";
  ptr += PATH;
  ptr += "</ span>";
  ptr += "</div>";

  ptr += "</div>";

  ptr += "</div>";

  ptr += "</body></html>";
  return ptr;
}


// HTML web page to handle 3 input fields (input1, input2, input3)
String sendHTML() {
  const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
.button1 {
  background-color: green;
  border: none;
  color: white;
  padding: 15px 32px;
  text-align: center;
  text-decoration: none;
  display: flex;
  font-size: 16px;
  margin: 4px 2px;
  cursor: pointer;
}
.button2 {
  background-color: red;
  border: none;
  color: white;
  padding: 15px 32px;
  text-align: center;
  text-decoration: none;
  display: flex;
  font-size: 16px;
  margin: 4px 2px;
  cursor: pointer;
}
.button3 {
  background-color: blue;
  border: none;
  color: white;
  padding: 15px 32px;
  text-align: center;
  text-decoration: none;
  display: flex;
  font-size: 16px;
  margin: 4px 2px;
  cursor: pointer;
}
</style>

  </head><body>
<h2>Autonomous Maze Solving Robot</h2>
  <form action="/decrement">
    <button type="submit" class="button2">( - )</button>
  </form><br>
  <form action="/increment">
    <button type="submit" class="button1">( + )</button>
  </form>
  </form><br>
  <form action="/start">
    <button type="submit" class="button3">Start</button>
  </form>
  </form><br>
  <form action="/stop">
    <button type="submit" class="button3">Stop</button>
  </form>
</body></html>)rawliteral";
  return index_html;
}
