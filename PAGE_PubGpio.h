const char PAGE_PubGpio[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <title>Publish GPIO</title>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="favicon.ico">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
  <div class="container">
    <div class="form">
      <form id="pubgpio" action="/postpubgpio">
        <table>
          <tr>
            <th colspan="2">Publish GPIO</th>
          </tr>
          <tr>
            <td><label for="gpio2">GPIO 2</label></td>
            <td><input type="text" id="gpio2" name="gpio2" placeholder="Topic" /></td>
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
  <script type="text/javascript" src="js/pubgpio.js"></script>
</body>
</html>
)=====";
