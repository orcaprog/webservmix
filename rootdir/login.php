<!DOCTYPE html>
<html>
    <head>

    </head>
    <body>
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
        <form action="" method="POST">
            User Name :<input type="text" name="username">
        </br>
            Pass word:<input type="text" name="password">
        </br>
        <input type="submit" name="submit" value="Log in">
        </form>
    </body>
</html>