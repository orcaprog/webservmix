
<?php
if (!isset($_SERVER['PHP_AUTH_USER'])) {
    header('WWW-Authenticate: Basic realm="My Realm"');
    header('HTTP/1.0 401 Unauthorized');
    echo 'Text to send if user hits Cancel button';
    exit;
} else {
    echo "<p>Hello {$_SERVER['PHP_AUTH_USER']}.</p>";
    echo "<p>You entered {$_SERVER['PHP_AUTH_PW']} as your password.</p>";
}
?>
    <?php
    if(isset($_POST["submit"]))
    {
        $username=$_POST["username"];
        $password=$_POST["password"];
        if($username == "molchi" && $password == "123")
        {
            echo "Your Information is correct";
        }
        else
        {
            echo "user name or password  is not  correct";
        }
    }
    ?>