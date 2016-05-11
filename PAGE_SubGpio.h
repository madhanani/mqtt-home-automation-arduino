const char PAGE_SubGpio[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <title>Subscription GPIO</title>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="favicon.ico">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
  <div class="container">
    <div class="form">
      <form id="subgpio" action="/postsubgpio">
        <table>
          <tr>
            <th colspan="2">Subscription GPIO</th>
          </tr>
          <tr>
            <td><label for="gpio4">GPIO 4</label></td>
            <td><input type="text" id="gpio4" name="gpio4" placeholder="Topic" /></td>
          </tr>
          <tr>
            <td><label for="gpio5">GPIO 5</label></td>
            <td><input type="text" id="gpio5" name="gpio5" placeholder="Topic" /></td>
          </tr>
          <tr>
            <td><label for="gpio12">GPIO 12</label></td>
            <td><input type="text" id="gpio12" name="gpio12" placeholder="Topic" /></td>
          </tr>
          <tr>
            <td><label for="gpio13">GPIO 13</label></td>
            <td><input type="text" id="gpio13" name="gpio13" placeholder="Topic" /></td>
          </tr>
          <tr>
            <td><label for="gpio14">GPIO 14</label></td>
            <td><input type="text" id="gpio14" name="gpio14" placeholder="Topic" /></td>
          </tr>
          <tr>
            <td><label for="gpio15">GPIO 15</label></td>
            <td><input type="text" id="gpio15" name="gpio15" placeholder="Topic" /></td>
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
  <script type="text/javascript" src="js/subgpio.js"></script>
</body>
</html>
)=====";
