import QtQuick 2.0

Canvas{

    property var graphTimestamps;
    property var graphVelocities;
    property var graphAccelerations;

    onPaint: {
        var context = getContext("2d");

        context.fillStyle ="white"
        context.fillRect(0, 0, width, height);

        drawVerticalLine(context, 0.0, "rgba(0,0,0,1)", 1.0)
        drawHorizontalLine(context, 0.0, "rgba(0,0,0,1)", 1.0)
        drawCircle(context, 70, "rgba(0,0,0,1)")

    }

    // Vízszintes vonal rajzolása
    function drawHorizontalLine(context, dataValue, strokeStyle, verticalScaler)
    {
        var offset = height/2;

        context.beginPath();
        context.lineWidth = 1;
        context.strokeStyle = strokeStyle;
        // Mozgás a vonal elejére
        context.moveTo(0, offset - verticalScaler * dataValue);
        // Mozgás a vonal végére
        context.lineTo(width, offset - verticalScaler * dataValue);
        context.stroke();
    }

    // Függőleges vonal rajzolása
    function drawVerticalLine(context, dataValue, strokeStyle, horizontalScaler)
    {
        var offset = width/2;

        context.beginPath();
        context.lineWidth = 1;
        context.strokeStyle = strokeStyle;
        // Mozgás a vonal elejére
        context.moveTo(offset - horizontalScaler * dataValue, 0);
        // Mozgás a vonal végére
        context.lineTo(offset - horizontalScaler * dataValue, height);
        context.stroke();
    }

    function drawCircle(context, radius, strokeStyle)
    {
        var offsetw = width/2;
        var offseth = height/2;
        context.beginPath();
        context.lineWidth = 1;
        context.strokeStyle = strokeStyle;

        // kör kirajzolása
        context.arc(offsetw, offsetw, radius, 0, 360, false);
        context.stroke();
    }


}


