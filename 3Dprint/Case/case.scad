use <SimpleStandoff.scad>
use <RasPi.scad>
use <Uno.scad> 
use <Triangle.scad>
use <CableReleif.scad>
$fn=80;
mountScrewR=2.2;
baseH=2;
standoffH=5;
elevation1=15;
base=[100,110,baseH];
baseTranslate=[0,0,0];
sideH=47;
e=0.2;
lidH=3;

module basePlate() {
 cube(base);
}

module sides() {
  translate([0,base[1]-baseH,0]) cube([base[0],baseH,sideH]);
  cube([baseH,base[1],sideH]);
  translate([base[0]-baseH,0,0]) cube([baseH,base[1],sideH]);
}


module mount() {
  cylinder(r=mountScrewR,h=baseH);
  translate([80,0,0]) cylinder(r=mountScrewR,h=baseH);

}

module pi() {
	rotate([0,0,-90]) translate([-85,0,0]) {
		rasPiStandoff(standoffH);
//		rasPi(standoffH); // Remove this line before print
	}
}

module unoModule() {
	difference() {
		unoStandoff(standoffH+elevation1);
		translate([30,0,BaseH]) cube([30,30,standoffH+elevation1+1]);
	}
//	translate([0,0,standoffH+elevation1]) uno(); // remove before print
}

module usbCut() {
  cube([10,15,9]);
}

module switchCut() {
  cylinder(r=7/2+1.5*e,h=10);
}

module ledCut() {
  cylinder(r=5/2+e,h=10);
}

module cableCut() {
  cube([10,15,4]);
}

module terminalHolder() {
  cylinder(r=3/2,h=12);
}

module boards() {
	pi();
	translate([-6,23,0]) unoModule();
}

module lidSupport(h) {
  translate([baseH,0,h]) rotate([-90,0,0]) triangle(2,2,base[1]);
  translate([base[0]-baseH,0,h]) rotate([-90,90,0]) triangle(2,2,base[1]);
  translate([baseH,0,h+lidH+e]) rotate([-90,0,0]) triangle(2,2,base[1]);
  translate([base[0]-baseH,0,h+lidH+e]) rotate([-90,90,0]) triangle(2,2,base[1]);
  translate([0,0,h]) cube([baseH,base[1],lidH+e]);
  translate([base[0]-baseH,0,h]) cube([baseH,base[1],lidH+e]);
}

module lid() {
  translate([0,-baseH,deltaS]) cube([base[0],baseH,lidH]);
  deltaS=0.8;
  difference() {
    translate([0,0,deltaS]) cube(base+[0,0,lidH-base[2]-deltaS]);
    translate([-deltaS/2,-1,0]) minkowski() {
      lidSupport(0);
		cube([deltaS,2,deltaS]);
    }
  }
}

module case() {
  translate(base+baseTranslate) translate([-62,-100,0]) boards();

difference() {
  sides();
  #translate([-1,79,3]) usbCut();
  #translate([-1,60,10]) rotate([0,90,0]) switchCut();
  #translate([base[0]-9,82,37]) cableCut();
  #translate([-1,100,25]) rotate([0,90,0]) ledCut();
}

difference() {
  translate(baseTranslate) basePlate();
  translate([10,102,0]) mount();
//  translate([22,22,0]) cylinder(r=17,h=baseH);
  translate([22,75,0]) cylinder(r=17,h=baseH);
  translate([64,80,0]) cylinder(r=17,h=baseH);
  translate([66,25,0]) cylinder(r=17,h=baseH);
}

lidSupport(sideH);

translate([15,25,baseH]) terminalHolder();

translate([7.5,5,baseH]) cableReleifBase(cableD=6);

translate([5,-10,0]) cableReleifHolder(cableD=6);
}

// Depending on the size of the print bed, it is usually neccesary to print the two parts below seperately
case();
translate([-base[0]-10,0,0]) lid();
