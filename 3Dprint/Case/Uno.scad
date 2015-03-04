use <ArduinoMountingLibrary/arduino.scad> // From http://www.thingiverse.com/thing:64008/#files, which uses pins.scad http://www.thingiverse.com/thing:10541/#files 
use <SimpleStandoff.scad>

unoHoles = [[0,1.25,0],[15.25,51.75,0],[43,51.75,0],[48.25,0,0]];

unoOffset=[2.5,14.25,0];
unoVolume=[54,69,13];

module unoStandoff(standoffH) {
  for ( i = [0 : 3] )
  {
	  translate(unoOffset) translate(unoHoles[i]) simpleStandoff(standoffH);
  }
}

module uno() {
  arduino();
//	cube(unoVolume);
}

unoStandoff(3,$fn=80);
translate([0,0,3]) uno();

