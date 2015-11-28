import QtQuick 2.0

// A HistoryGraph egy Canvas, mivel arra lehet rajzolni.
Canvas{
    // Kintről kapott tulajdonságai
    property var graphTimestamps;
    property var graphVelocities;
    property var graphAccelerations

    // Paint signal esetén újra kell rajzolni a grafikon
    onPaint: {
        // "drawing context", amire rajzolunk
        var context = getContext("2d");

        // Kitöltés fehérre
        context.fillStyle = "white"
        context.fillRect(0, 0, width, height);

        // koordináta tengelyek berajzolása
        drawHorizontalLine(context, 0.0, "rgba(0,0,0,1)", 5.0)
        drawVerticalLine(context, 0.0, "rgba(0,0,0,1)", 5.0)

        // adatsorok kirajzolása
        drawDataset(context, graphVelocities, "rgba(110,220,110,1)", 5.0);
        drawDataset(context, graphAccelerations, "rgba(220,110,110,1)", 5.0);
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

    function drawDataset(context, datarow, strokeStyle, verticalScaler)
    {
        var offset = height/2;

        context.beginPath();
        context.lineWidth = 3;
        context.strokeStyle = strokeStyle;
        context.moveTo(0, offset-datarow[0]);
        // A vektoron végigmenve behúzzuk a grafikon szakaszait.
        for(var i=0; i<graphVelocities.length; i++)
        {
            context.lineTo(10*i, offset - verticalScaler * datarow[i]);
        }
        contex.stroke();
    }

    // TODO körvonal húzása adott sugárral (?)

}

