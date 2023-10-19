const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>

<div id="demo">
<h1>Welcome to PBU Analytics</h1>
</div><div>
	Reaction Time : <span id="RT">0</span><br>
  Visual Memory Level : <span id="VT">0</span><br>
  Sequence Memory Level : <span id="ST">0</span><br>
</div><script>
function sendData(data) {
  var xhttp = new XMLHttpRequest();
  if(data.length >= 5 && data.length <= 9){
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("VT").innerHTML = this.responseText;
      }
    };
    xhttp.open("POST", "/index", true); // Specify just the endpoint URL
    xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    data = data % 10000;
    var params = "VT=" + data; // Create the POST data
  }
  else if(data.length >= 10){
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("ST").innerHTML = this.responseText;
      }
    };
    xhttp.open("POST", "/index", true); // Specify just the endpoint URL
    xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    data = data % 100000000;
    var params = "ST=" + data; // Create the POST data
  }
  else{
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("RT").innerHTML = this.responseText;
      }
    };
    xhttp.open("POST", "/index", true); // Specify just the endpoint URL
    xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    var params = "RT=" + data; // Create the POST data
  }
  xhttp.send(params); // Send the POST request with the data
}

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  sendData();
}, 2000); //2000mSeconds update rate

</script>
</body>
</html>
)=====";
