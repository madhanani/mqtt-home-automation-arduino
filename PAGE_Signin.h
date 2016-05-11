const char PAGE_Signin[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <title>SignIn</title>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="favicon.ico">  
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
  <div class="container">
    <div class="form">
      <form id="signin" action="/signin">
        <table>
          <tr>
            <th colspan="2">Please Signin</th>
          </tr>
          <tr>
            <td><label for="username">Username</label></td>
            <td><input type="text" id="username" name="username" placeholder="Username" /></td>
          </tr>
          <tr>
            <td><label for="password">Password</label></td>
            <td><input type="password" id="password" name="password" placeholder="Password" /></td>
          </tr>
          <tr>
            <td colspan="2"><button type="submit" class="button">Signin</button></td>
          </tr>     
        </table>
      </form>
    </div>
  </div>
  <script type="text/javascript" src="js/jquery.min.js"></script>
  <script type="text/javascript" src="js/jcookie.js"></script>
  <script type="text/javascript" src="js/signin.js"></script>
</body>
</html>
)=====";

