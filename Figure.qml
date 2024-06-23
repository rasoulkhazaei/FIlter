import QtQuick
import QtCharts
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs

ColumnLayout {
    id: root

    property real minX: figureCpp.minX
    property real maxX: figureCpp.maxX
    property real minY: figureCpp.minY
    property real maxY: figureCpp.maxY

    property real minY2: figureCpp.minY2
    property real maxY2: figureCpp.maxY2
    property real mouseX: 0
    property real mouseY1: 0
    property real mouseY2: 0

    property string customing: "nothing"

    enum Customising {
        Start,
        Stop,
        Level
    }

    property int custommingState: Figure.Customising.Start

    enum MouseType {
        Draging,
        Zooming,
        Nothing
    }

    Keys.onPressed: (event)=> {
                        if (event.key === Qt.Key_Left)
                        chartView.scrollLeft(10)
                        if (event.key === Qt.Key_Right)
                        chartView.scrollRight(10)
                        if (event.key === Qt.Key_Up)
                        chartView.scrollUp(10)
                        if (event.key === Qt.Key_Down)
                        chartView.scrollDown(10)
                    }
    RowLayout {
        ToolBar{
            RowLayout {
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                Layout.topMargin: 10
                spacing: 1

                ToolButton {
                    text: "Theme"
                    onClicked: themeMenu.open()
                    Menu {
                        id: themeMenu
                        Repeater {
                            model: ["Light", "BlueCerulean", "Dark", "BrownSand", "BlueNcs", "HighContrast", "BlueIcy", "Qt"]

                            MenuItem {
                                required property string modelData
                                required property int index
                                text: modelData
                                onClicked: {
                                    chartView.theme = index
                                }
                            }
                        }
                    }

                }

                ToolButton {
                    // Layout.leftMargin: 15
                    text: "X Unit:"
                    onClicked: unitMenu.open()
                    Menu {
                        id: unitMenu
                        Repeater {
                            model: ["Length(mm)", "Time(s)"]
                            MenuItem {
                                required property string modelData
                                text: modelData
                                onClicked: {
                                    fileManagerCpp.xAxis = modelData
                                    fileManagerCpp.justUpdateData(axialSeries, radialSeries)
                                }
                            }
                        }
                    }
                }

                ToolButton {
                    text: "GridLines"
                    onClicked: gridMenu.open()
                    Menu {
                        id: gridMenu
                        title: "Grid"
                        MenuItem {
                            checkable: true
                            text: "X Grid"
                            checked: true
                            onCheckedChanged: {
                                axisX.gridVisible = checked
                            }
                        }

                        MenuItem {
                            checkable: true
                            text: "Axial Grid"
                            checked: true
                            onCheckedChanged: {
                                axisY.gridVisible = checked
                            }
                        }

                        MenuItem {
                            checkable: true
                            text: "Radial Grid"
                            checked: true
                            onCheckedChanged: {
                                axisY2.gridVisible = checked
                            }
                        }
                    }
                }

                ToolButton {
                    text: "Customize"
                    onClicked: customMenu.open()
                    Menu {
                        id: customMenu
                        Repeater {
                            model: ["axial", "radial"]
                            MenuItem {
                                required property string modelData
                                text: modelData
                                onClicked: {
                                    root.customing = modelData
                                    customDialog.show()
                                    dragBtn.checked = false
                                    zoomBtn.checked = false
                                    fileManagerCpp.customising = modelData
                                    chartView.mouseBeh = Figure.MouseType.Nothing
                                }
                            }
                        }
                    }

                    Window {
                        id: customDialog
                        width: 400
                        height: 300
                        minimumWidth: 400
                        minimumHeight: 300
                        title: "Customising " + root.customing
                        flags: Qt.WindowCloseButtonHint
                        ColumnLayout {

                            anchors.fill: parent
                            RowLayout {
                                id: lyt
                                ColumnLayout {
                                    RadioButton {
                                        id: startRadio
                                        checked: true
                                        Layout.bottomMargin: 15
                                        onCheckedChanged: {
                                            if (checked) {
                                                root.custommingState = Figure.Customising.Start
                                            }
                                        }
                                    }
                                    RadioButton {
                                        id: stopRadio
                                        Layout.bottomMargin: 15
                                        onCheckedChanged: {
                                            if (checked) {
                                                root.custommingState = Figure.Customising.Stop
                                            }
                                        }
                                    }
                                    RadioButton {
                                        id: levelRadio
                                        Layout.bottomMargin: 15
                                        onCheckedChanged: {
                                            if (checked) {
                                                root.custommingState = Figure.Customising.Level
                                            }
                                        }
                                    }
                                }



                            }
                            Row {
                                spacing: 10
                                Layout.leftMargin: 20
                                Button {
                                    id: okBtn
                                    text: "Ok"
                                    // enabled: (Math.abs(fileManagerCpp.customStart - 0) >= Number.EPSILON ) &&
                                    //          (Math.abs(fileManagerCpp.customStop - 0) >= Number.EPSILON ) &&
                                    //          (Math.abs(fileManagerCpp.customLevel - 0) >= Number.EPSILON )
                                    onClicked: {
                                        fileManagerCpp.applyCustoming()
                                        fileManagerCpp.justUpdateData(axialSeries, radialSeries)
                                        cancelBtn.clicked()
                                    }
                                }

                                Button {
                                    id: cancelBtn
                                    text: "Cancel"
                                    onClicked: {
                                        fileManagerCpp.customStart.x = 0
                                        fileManagerCpp.customStart.y = 0
                                        fileManagerCpp.customStop.x = 0
                                        fileManagerCpp.customStop.y = 0
                                        fileManagerCpp.customLevel.x = 0
                                        fileManagerCpp.customLevel.y = 0
                                        root.customing = "nothing"
                                        customDialog.close()
                                    }
                                }
                            }
                        }
                        // standardButtons: Dialog.Save | Dialog.Cancel
                    }
                }

                ToolButton {
                    text: "Utility"
                    onClicked: utilityMenu.open()
                    Menu {
                        id: utilityMenu
                        MenuItem {
                            text: "hex to txt"
                            onClicked:  {
                                utilityDialog.open()
                            }
                        }
                    }

                    FileDialog {
                        id: fileDialog
                        onAccepted:  {
                            fileLoc.text = currentFile
                        }
                    }

                    FolderDialog {
                        id: folderDialog
                        onAccepted: {
                            folderLoc.text = currentFolder
                        }
                    }

                    Dialog {
                        id: utilityDialog
                        x: 200
                        y: 200
                        width: 500
                        height: 300
                        GridLayout {
                            anchors.fill: parent
                            columns: 3
                            Text {
                                text: "Choose File:"
                            }

                            TextField {
                                id: fileLoc
                                Layout.fillWidth: true
                                enabled: false
                            }

                            Button {
                                text: "Browse"
                                onClicked: {
                                    fileDialog.open()
                                }
                            }

                            Text {
                                text: "Folder To Save txt:"
                            }

                            TextField {
                                id: folderLoc
                                Layout.fillWidth: true
                                enabled: false
                            }

                            Button {
                                text: "Browse"
                                onClicked: {
                                    folderDialog.open()
                                }
                            }

                            Button {
                                text: "Ok"

                                onClicked: {
                                    fileManagerCpp.hex2txt(fileLoc.text, folderLoc.text)
                                    utilityDialog.close()
                                }
                            }

                            Button {
                                text: "Cancel"
                                onClicked: {
                                    utilityDialog.close()
                                }
                            }


                        }
                    }
                }
            }

        }
        Button {
            id: homeBtn
            display: AbstractButton.IconOnly
            hoverEnabled: true
            icon {
                // source: "Images/home.png"
                width: 28
                height: 28
            }
            ToolTip {
                parent: homeBtn
                text: "Reset original view"
                visible: homeBtn.hovered
                delay: Qt.styleHints.mousePressAndHoldInterval
                font.pointSize: 12
                y: homeBtn.y + homeBtn.height
            }

            // background: Rectangle {
            //     color: homeBtn.hovered ? "white" : "lightgray"
            //     border.color: "gray"
            //     border.width: homeBtn.hovered ? 1 : 0
            //     radius: homeBtn.hovered ? 3 : 0
            // }
            width: 32
            height: 32
            onClicked: {
                chartView.zoomReset()
                figureCpp.maxY = figureCpp.maxY1Home
                figureCpp.minY = figureCpp.minY1Home
                figureCpp.maxY2 = figureCpp.maxY2Home
                figureCpp.minY2 = figureCpp.minY2Home
                figureCpp.maxX = figureCpp.maxXHome
                figureCpp.minX = figureCpp.minXHome

            }
        }

        Button {
            id: dragBtn
            display: AbstractButton.IconOnly
            hoverEnabled: true
            icon {
                // source: "Images/drag.png"
                width: 28
                height: 28
            }
            // background: Rectangle {
            //     color: dragBtn.hovered ? "white" : dragBtn.checked ? "darkgray" : "lightgray"
            //     border.color: "gray"
            //     border.width: dragBtn.hovered || dragBtn.checked ? 1 : 0
            //     radius: dragBtn.hovered || dragBtn.checked ? 3 : 0
            // }
            ToolTip {
                parent: dragBtn
                text: "Left button pans, Right button zooms x/y fixes axis"
                visible: dragBtn.hovered
                delay: Qt.styleHints.mousePressAndHoldInterval
                font.pointSize: 12
                y: dragBtn.y + dragBtn.height
            }
            checkable: true
            checked: false
            onClicked: {
                if (!checked)
                    chartView.mouseBeh = Figure.MouseType.Nothing
                else {
                    chartView.mouseBeh = Figure.MouseType.Draging
                    if (zoomBtn.checked)
                        zoomBtn.checked = false
                }
            }

            width: 32
            height: 32
        }

        Button {
            id: zoomBtn
            display: AbstractButton.IconOnly
            hoverEnabled: true
            icon {
                // source: "Images/zoom.png"
                width: 28
                height: 28
            }

            // background: Rectangle {
            //     color: zoomBtn.hovered ? "white" : zoomBtn.checked ? "darkgray" : "lightgray"
            //     border.color: "gray"
            //     border.width: zoomBtn.hovered || zoomBtn.checked? 1 : 0
            //     radius: zoomBtn.hovered || zoomBtn.checked ? 3 : 0
            // }

            ToolTip {
                parent: zoomBtn
                text: "Zoom to rectangle"
                visible: zoomBtn.hovered
                delay: Qt.styleHints.mousePressAndHoldInterval
                font.pointSize: 12
                y: zoomBtn.y + zoomBtn.height
            }
            checkable: true
            checked: false
            onClicked: {
                if (!checked)
                    chartView.mouseBeh = Figure.MouseType.Nothing
                else {
                    chartView.mouseBeh = Figure.MouseType.Zooming
                    if (dragBtn.checked)
                        dragBtn.checked = false
                }

            }

            width: 32
            height: 32
        }



        Item {
            Layout.fillWidth: true
        }

        Label {
            text: "x: "
            color: "black"
            visible: dragArea.containsMouse || axialSeries.hovered
        }

        Label {
            id: xValue
            text: mouseX.toFixed(5)
            color: "black"
            visible: dragArea.containsMouse || axialSeries.hovered
        }


        Label {
            Layout.leftMargin: 10
            text: "axial: "
            color: "black"
            visible: dragArea.containsMouse || axialSeries.hovered
        }

        Label {
            id: yValue
            text: mouseY1.toFixed(5)
            color: "black"
            visible: dragArea.containsMouse || axialSeries.hovered
        }

        Label {
            Layout.leftMargin: 10
            text: "radial: "
            color: "black"
            visible: dragArea.containsMouse
        }

        Label {
            id: y2Value
            text: mouseY2.toFixed(5)
            color: "black"
            visible: dragArea.containsMouse
        }
    }

    ChartView {
        id: chartView
        Layout.fillHeight: true
        Layout.fillWidth: true
        margins.bottom: 0
        margins.top: 0
        margins.left: 0
        margins.right: 0




        property int mouseBeh: Figure.MouseType.Nothing
        property int zoomRatio: 30

        // property bool openGL: openGLSupported
        property list<int> pointsX: []
        property list<int> pointsY: []

        signal graphxyChanged(point: var, state: bool)
        animationOptions: ChartView.SeriesAnimations
        theme: ChartView.ChartThemeBlueCerulean


        plotArea: Qt.rect(0, 0, 0, 0)

        // onOpenGLChanged: {
        //     if (openGLSupported) {
        //         var series1 = series("signal 1")
        //         if (series1)
        //             series1.useOpenGL = openGL;
        //         var series2 = series("signal 2")
        //         if (series2)
        //             series2.useOpenGL = openGL;
        //     }
        // }
        legend.visible: true

        Rectangle {
            id: rubber
            color: "steelblue"
            opacity: 0.7
            border.color: "blue"
            border.width: 2

        }
        focus: true


        MouseArea {

            id: dragArea
            propagateComposedEvents: true
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            anchors.fill: parent
            property real oldX: 0
            property real oldY: 0
            visible: customing === "nothing" || levelRadio.checked
            cursorShape: chartView.mouseBeh === Figure.MouseType.Nothing ? Qt.ArrowCursor :
                                                                           chartView.mouseBeh === Figure.MouseType.Draging ?
                                                                               Qt.ClosedHandCursor : Qt.CrossCursor
            // onWheel: (w)=>{
            //              if (w.angleDelta.y > 0)
            //              chartView.zoom(1.1)
            //              else
            //              chartView.zoom(0.9)

            //          }
            onReleased: (mouse) => {
                            if (chartView.pointsY.length > 0 | chartView.pointsX > 0)
                            if(mouse.button === Qt.LeftButton)
                            chartView.zoomIn(rubber)
                            else if (mouse.button === Qt.RightButton)
                            chartView.zoom(rubber.width/chartView.width)
                            rubber.visible = false
                            mouse.accepted = false
                        }

            onPressed: (mouse) => {
                           oldX = mouse.x
                           oldY = mouse.y
                           chartView.pointsY = []
                           chartView.pointsX = []
                           if (levelRadio.checked) {
                               if (customing == "axial") {
                                   var p1 = chartView.mapToValue(Qt.point(mouse.x, mouse.y), axialSeries)
                                   fileManagerCpp.customLevel = p1
                                   print(p1)
                               }
                               if (customing == "radial") {
                                   var p2 = chartView.mapToValue(Qt.point(mouse.x, mouse.y), radialSeries)
                                   fileManagerCpp.customLevel = p2
                                   print(p2)
                               }
                           }
                       }

            hoverEnabled: true

            onPositionChanged: (mouse) => {
                                   var p1 = chartView.mapToValue(Qt.point(mouse.x, mouse.y), axialSeries)
                                   // var p2 = chartView.mapToValue(Qt.point(mouse.x, mouse.y), radialSeries)
                                   root.mouseX = p1.x
                                   root.mouseY1 = p1.y
                                   // root.mouseY2 = p2.y

                                   if (!pressed)
                                   return
                                   if (chartView.mouseBeh === Figure.MouseType.Zooming) {
                                       rubber.visible = true
                                       chartView.pointsX.push(mouse.x)
                                       chartView.pointsY.push(mouse.y)
                                       rubber.x = Math.min(chartView.pointsX[0], chartView.pointsX[chartView.pointsX.length-1])
                                       rubber.y = Math.min(chartView.pointsY[0], chartView.pointsY[chartView.pointsY.length-1])
                                       rubber.width = Math.abs(Math.max(chartView.pointsX[0], chartView.pointsX[chartView.pointsX.length-1]) - Math.min(chartView.pointsX[0], chartView.pointsX[chartView.pointsX.length-1]))
                                       rubber.height = Math.abs(Math.max(chartView.pointsY[0], chartView.pointsY[chartView.pointsY.length-1]) - Math.min(chartView.pointsY[0], chartView.pointsY[chartView.pointsY.length-1]))
                                   }
                                   else if (chartView.mouseBeh === Figure.MouseType.Draging) {
                                       if (dragArea.pressedButtons === Qt.LeftButton) {
                                           if (mouse.x > oldX)
                                           chartView.scrollLeft(mouse.x - oldX)
                                           else if (mouse.x < oldX)
                                           chartView.scrollRight(oldX - mouse.x)

                                           var y1factor = (figureCpp.maxY - figureCpp.minY) / chartView.height
                                           var y2factor = (figureCpp.maxY2 - figureCpp.minY2) / chartView.height
                                           // print(chartView.height)
                                           // print(mouse.y)
                                           if (mouse.x > chartView.width/2) {
                                               // chartView.scrollUp(mouse.y - oldY)
                                               figureCpp.minY2 +=  (mouse.y - oldY) * y2factor //axisY2.tickInterval
                                               figureCpp.maxY2 +=  (mouse.y - oldY) * y2factor//axisY2.tickInterval
                                           }
                                           else {
                                               // // chartView.scrollDown(oldY - mouse.y)
                                               figureCpp.minY += (mouse.y - oldY) * y1factor
                                               figureCpp.maxY += (mouse.y - oldY) * y1factor
                                           }

                                       }
                                       else if (dragArea.pressedButtons === Qt.RightButton) {
                                           if (mouse.y < oldY) {
                                               figureCpp.minY = figureCpp.minY + axisY.tickInterval/chartView.zoomRatio
                                               figureCpp.maxY = figureCpp.maxY - axisY.tickInterval/chartView.zoomRatio
                                           }
                                           else if (mouse.y > oldY) {
                                               figureCpp.minY = figureCpp.minY - axisY.tickInterval/chartView.zoomRatio
                                               figureCpp.maxY = figureCpp.maxY + axisY.tickInterval/chartView.zoomRatio
                                           }
                                           if (mouse.x > oldX) {
                                               figureCpp.minX = figureCpp.minX + axisX.tickInterval/chartView.zoomRatio
                                               figureCpp.maxX = figureCpp.maxX - axisX.tickInterval/chartView.zoomRatio
                                           }
                                           else if (mouse.x < oldX){
                                               figureCpp.minX = figureCpp.minX - axisX.tickInterval/chartView.zoomRatio
                                               figureCpp.maxX = figureCpp.maxX + axisX.tickInterval/chartView.zoomRatio
                                           }
                                       }
                                       oldX = mouse.x
                                       oldY = mouse.y
                                   }

                               }
        }

        ValuesAxis {
            id: axisY
            min: root.minY
            max: root.maxY

            labelFormat: "%3.0f mv"


            onRangeChanged: {
                var logg = Math.log((axisY.max - axisY.min)/1.7) / Math.LN10
                var dec = Math.floor(logg)
                axisY.labelFormat = "%3." + (dec < 0 ? Math.abs(dec) : 0) + "f mv"
                axisY.tickInterval = Math.pow(10, dec)
            }


            tickAnchor: 0
            tickInterval: 1
            tickType : ValueAxis.TicksDynamic
        }

        ValuesAxis {
            id: axisY2
            min: root.minY2
            max: root.maxY2
            labelFormat: "%3.0f mv"
            // gridVisible: false
            // gridLinePen: 1
            onRangeChanged: {
                var logg = Math.log((axisY2.max - axisY2.min)/1.7) / Math.LN10
                var dec = Math.floor(logg)
                axisY2.labelFormat = "%3." + (dec < 0 ? Math.abs(dec) : 0) + "f mv"
                axisY2.tickInterval = Math.pow(10, dec)
            }

            tickAnchor: 0
            tickInterval: 1
            tickType : ValueAxis.TicksDynamic
        }

        ValuesAxis {
            id: axisX
            min: root.minX
            max: root.maxX
            onRangeChanged: {
                var logg = Math.log((axisX.max - axisX.min)/1.7) / Math.LN10
                var dec = Math.floor(logg)
                axisX.labelFormat = "%3." + (dec < 0 ? Math.abs(dec) : 0) + "f"
                axisX.tickInterval = Math.pow(10, dec)
            }
            tickAnchor: 0
            tickInterval: 100
            tickType : ValueAxis.TicksDynamic
        }

        LineSeries {
            id: axialSeries
            axisX: axisX
            axisY: axisY
            name: "Axial"
            property bool hovered: false
            onHovered: (point , state)=> {
                           root.mouseX = point.x
                           root.mouseY1 = point.y

                           hovered = state
                       }

            onPressed:  (point) =>{
                            if (customing != "axial")
                            return
                            if (startRadio.checked) {
                                fileManagerCpp.customStart = point
                            }
                            else if (stopRadio.checked) {
                                fileManagerCpp.customStop = point
                            }

                        }

            // useOpenGL: chartView.openGL
        }


        Timer {
            id: refreshTimer
            interval: 1000
            running: true
            repeat: true
            onTriggered: {
                figureCpp.update(axialSeries)
            }
        }
    }
}
