<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Turtle Graphics</title>
    <style>
        canvas {
            /* Remove the border property */
        }
    </style>
    <script>
        function drawTurtle() {
            var canvas = document.getElementById("turtleCanvas");
            var ctx = canvas.getContext("2d");

            var x = 150;
            var y = 150;
            var angle = 0;

            for (var i = 0; i < 100; i++) {
                var steps = Math.floor(Math.random() * 100);
                var angleChange = Math.floor(Math.random() * 360);
                angle += angleChange;

                var newX = x + steps * Math.cos(angle * Math.PI / 180);
                var newY = y + steps * Math.sin(angle * Math.PI / 180);

                ctx.beginPath();
                ctx.moveTo(x, y);
                ctx.lineTo(newX, newY);
                ctx.stroke();

                x = newX;
                y = newY;
            }
        }
    </script>
</head>
<body onload="drawTurtle()">
    <canvas id="turtleCanvas" width="300" height="300"></canvas>
</body>
</html>
