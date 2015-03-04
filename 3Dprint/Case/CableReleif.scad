use <SimpleStandoff.scad>

standoffD=3.2+2;
xT = 2.6;
holeD=3.9;
e=0.2;

module cable(cableD) {
  rotate([90,0,0]) cylinder(r=cableD/2,h=10);
}

module cableReleifBase(cableD) {
  difference() {
    union() {
      simpleStandoff(cableD);
      translate([cableD+standoffD+4,0,0]) simpleStandoff(cableD);
      translate([standoffD/2-e/2,-standoffD/2,0]) cube([cableD+4+e,standoffD,cableD]);
    }
    hull() {
      translate([(cableD+standoffD+4)/2,5,cableD/2+1]) cable(cableD);
      translate([(cableD+standoffD+4)/2,5,cableD/2+10]) cable(cableD);
    }
  }
}

module cableReleifHolder(cableD) {
  difference() {
    translate([-1,-1,0]) cube([cableD+2*standoffD+6,standoffD+2,2]);
    #translate([standoffD/2,standoffD/2,-1]) cylinder(r=holeD/2,h=10);
    #translate([cableD+2*standoffD+4-standoffD/2,standoffD/2,-1]) cylinder(r=holeD/2,h=10);
  }
}

cableReleifBase(cableD=6);

translate([-standoffD/2,10,0]) cableReleifHolder(cableD=6);







