use <SimpleStandoff.scad>

RasPiHoles=[[0,0,0],[58,0,0],[58,49,0],[0,49,0]];
RasPiHolesOffset=[3.5,3.5,0];
RasPiVolume=[85,56,17.5];

module rasPiStandoff(standoffH) {
  for ( i = [0 : 3] )
  {
	  translate(RasPiHolesOffset) translate(RasPiHoles[i]) simpleStandoff(standoffH);
  }
}

module rasPi(standoffH) {
  translate([139,-42.5,standoffH-2.5]) import("/home/carsten/Documents/My3DPrinterFiles/stl/B+_Model_v4.stl"); // Model from http://www.thingiverse.com/thing:402498/#files
//	cube(RasPiVolume);
}

rasPiStandoff(3, $fn=80);

rasPi(3);

