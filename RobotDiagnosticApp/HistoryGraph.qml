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
        drawHorizontalLine(context, 0.0, "rgba(0,0,0,1)", 1.0)
        drawVerticalLine(context, 0.0, "rgba(0,0,0,1)", 1.0)
        drawCircle(context, -50, 0, 50, 0, 280, "rgba(100,0,0,1)", 1.0);
        // adatsorok kirajzolása
        drawDataset(context, graphVelocities, "rgba(110,220,110,1)", 1.0);
        drawDataset(context, graphAccelerations, "rgba(220,110,110,1)", 1.0);


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
        /** TODO */
        //context.moveTo(0, offset-datarow[0]);
        // A vektoron végigmenve behúzzuk a grafikon szakaszait.
        /*for(var i=0; i<graphVelocities.length; i++)
        {
            context.lineTo(10*i, offset - verticalScaler * datarow[i]);
        }*/
        context.stroke();
    }

    // TODO körvonal húzása adott sugárral (?)
    function drawCircle(context, dataValue1, dataValue2, radius, startAngle, endAngle, strokeStyle, verticalScaler)
    {
        var offsetw = width/2;
        var offseth = height/2;

        var startAngRad = startAngle*Math.PI/180;
        var endAngRad = endAngle*Math.PI/180;

        context.beginPath();
        context.lineWidth = 3;
        context.strokeStyle = strokeStyle;
        context.arc(offsetw + verticalScaler * dataValue1, offseth - verticalScaler * dataValue2,
                    radius, startAngRad, endAngRad, false)
        context.stroke();
    }

}

