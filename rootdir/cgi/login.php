<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Login Page</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f4;
            text-align: center;
            margin-top: 100px;
        }
        .container {
            width: 300px;
            padding: 20px;
            background-color: #fff;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
            border-radius: 5px;
            margin: 0 auto;
        }
    </style>
</head>
<body>
    <div class="container">
        <h2>Login Form</h2>
        <?php
        if(isset($_POST["submit"])) {
            $username = $_POST["username"];
            $password = $_POST["password"];
            if($username == "molchi" && $password == "123") {
                echo "<p style='color: green;'>Your information is correct. Welcome, $username!</p>";
            } else {
                echo "<p style='color: red;'>Username or password is incorrect. Please try again.</p>";
            }
        }
        ?>
        <form method="post">
            <label for="username">Username:</label>
            <input type="text" name="username" required><br>
            <label for="password">Password:</label>
            <input type="password" name="password" required><br>
            <button type="submit" name="submit">Login</button>
        </form>
    </div>
</body>
</html>