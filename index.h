const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>

<div id="demo">
<h1>Welcome to PBU Analytics</h1>
</div>

<div>
	Reaction Time : <span id="RT">0</span><br>
</div>

<script>
function sendData(time) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("RT").innerHTML = this.responseText;
    }
  };
  xhttp.open("POST", "/index", true); // Specify just the endpoint URL
  xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
  var params = "RT=" + time; // Create the POST data
  xhttp.send(params); // Send the POST request with the data
}

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  sendData();
}, 5000); //2000mSeconds update rate

</script>
</body>
</html>
)=====";
