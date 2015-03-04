module simpleStandoff(height) {
  mountingHoleDiameter = 3.2;
  difference() {
    cylinder(r=1 + mountingHoleDiameter/2, h = height);
    translate([0,0,-1]) cylinder(r=mountingHoleDiameter/2, h = height+2);    
  }
}


simpleStandoff(3, $fn=80);
