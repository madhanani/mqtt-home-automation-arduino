const char PAGE_Index[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <title>Dashboard</title>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="favicon.ico">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
  <div class="container">
    <div class="form">
      <form id="config" action="/config">
        <table>
          <tr>
            <th colspan="2">General Settings</th>
          </tr>
          <tr>
            <td><label for="username">Username</label></td>
            <td><input type="text" id="username" name="username" placeholder="Username" /></td>
          </tr>
          <tr>
            <td><label for="password">Password</label></td>
            <td><input type="text" id="password" name="password" placeholder="Password" /></td>
          </tr>
          <tr>
            <td><label for="cookieName">Cookie Name</label></td>
            <td><input type="text" id="cookieName" name="cookieName" placeholder="Cookie Name" /></td>
          </tr>
          <tr>
            <td><label for="cookieValue">Cookie Value</label></td>
            <td><input type="text" id="cookieValue" name="cookieValue" placeholder="Cookie Value" /></td>
          </tr>
          <tr>
            <th colspan="2">WiFi Settings</th>
          </tr>
          <tr>
            <td><label for="ssidSTA">Name</label></td>
            <td><input type="text" id="ssidSTA" name="ssidSTA" placeholder="Name" /></td>
          </tr>
          <tr>
            <td><label for="passwordSTA">Password</label></td>
            <td><input type="text" id="passwordSTA" name="passwordSTA" placeholder="Password" /></td>
          </tr>
          <tr>
            <th colspan="2">Access Point Settings</th>
          </tr>
          <tr>
            <td><label for="ssidAP">Name</label></td>
            <td><input type="text" id="ssidAP" name="ssidAP" placeholder="Name" /></td>
          </tr>
          <tr>
            <td><label for="passwordAP">Password</label></td>
            <td><input type="text" id="passwordAP" name="passwordAP" placeholder="Password" /></td>
          </tr>
          <tr>
            <th colspan="2">MQTT Settings</th>
          </tr>
          <tr>
            <td><label for="mqttServer">Server</label></td>
            <td><input type="text" id="mqttServer" name="mqttServer" placeholder="192.168.0.199" /></td>
          </tr> 
          <tr>
            <td colspan="2"><button type="submit" class="button">Save</button></td>
          </tr> 
          <tr>
            <td colspan="2"><button id="logout" class="button">Logout</button></td>
          </tr>   
        </table>
      </form>
    </div>
  </div>
  <script type="text/javascript" src="js/jquery.min.js"></script>
  <script type="text/javascript" src="js/jcookie.js"></script>
  <script type="text/javascript" src="js/main.js"></script>
</body>
</html>
)=====";
