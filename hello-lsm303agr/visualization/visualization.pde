/*   Arduino Compass
 *
 *  by Dejan Nedelkovski,
 *  www.HowToMechatronics.com
 *
 */

import processing.serial.*;
import java.awt.event.KeyEvent;
import java.io.IOException;

Serial myPort;
PImage imgCompass;
PImage imgCompassArrow;
PImage background;

String data="";
float heading;

void setup() {
  size (1920, 1080, P3D);
  smooth();
  imgCompass = loadImage("Compass.png");
  imgCompassArrow = loadImage("CompassArrow.png");
  background = loadImage("Background.png");

  myPort = new Serial(this, "/dev/ttyACM0", 115200); // starts the serial communication
  myPort.bufferUntil('\n');
}

void draw() {

  image(background, 0, 0); // Loads the Background image

  pushMatrix();
  translate(width/2, height/2, 0); // Translates the coordinate system into the center of the screen, so that the rotation happen right in the center
  rotateZ(radians(-heading)); // Rotates the Compass around Z - Axis
  image(imgCompass, -960, -540); // Loads the Compass image and as the coordinate system is relocated we need need to set the image at -960x, -540y (half the screen size)
  popMatrix(); // Brings coordinate system is back to the original position 0,0,0

  image(imgCompassArrow, 0, 0); // Loads the CompassArrow image which is not affected by the rotateZ() function because of the popMatrix() function
  textSize(30);
  text("Heading: " + heading, 40, 40); // Prints the value of the heading on the screen

  delay(40);
}

// starts reading data from the Serial Port
void serialEvent (Serial myPort) {

  try {
    data = myPort.readStringUntil('\n');// reads the data from the Serial Port and puts it into the String variable "data".
    print(data);

    String[] list = split(data, ':');
    if (list[0].equals("Heading") == true) {
      heading = float(list[1]);
    }
  }
  catch(RuntimeException e) {
    e.printStackTrace();
  }
}
