import QtQuick

Image {
    id: mediaButton

    signal clicked

    property string shapeStr: "<circle cx='12' cy='12' r='8'/>"

    property string color: "gray"
    property string colorHover: "lighgray"
    property string colorPressed: "white"
    property string activeColor: mouseArea.pressed ? colorPressed : (mouseArea.containsMouse ? colorHover : color)

    property string iconBtn: "data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2002000/svg' viewBox='0 0 24 24' fill='" + activeColor + "'>" + shapeStr + "</svg>"

    height: parent.height * 0.6
    width: parent.width * 0.2

    source: iconBtn

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            mediaButton.clicked();
        }
    }
}
