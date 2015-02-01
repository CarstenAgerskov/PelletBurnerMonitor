e = 0.1;
lowEffectLedPos = [25,84,0];
stbyTestLedPos= [25,73,0];
driftLed = [25,100,0];
photoResistorR = 6/2;
baseThickness = 2;
ledR = 1;
ledW = 6;
l = 130;
b = 130;
hole1=[6,4,0];
hole2=[6,46,0];
$fn=80;

module photoResistorTube() {
  tubeDepth = 9;
  difference() {
    cylinder(r=photoResistorR+baseThickness,h=tubeDepth);
    cylinder(r=photoResistorR+e,h=tubeDepth-1);
    translate([0,0,0.5+tubeDepth])cube([2,photoResistorR*2,tubeDepth],center=true);    
  }
}

module ledSeperator() {
    translate([0,baseThickness/2+photoResistorR+e,9/2]) cube([ledW,baseThickness,9],center=true);
    translate([0,-baseThickness/2-photoResistorR,9/2]) cube([ledW,baseThickness,9],center=true);
}

module allTubes() {
  translate(lowEffectLedPos) rotate([0,-45,0]) photoResistorTube();  
  translate(stbyTestLedPos) rotate([0,-45,0]) photoResistorTube();
  translate(driftLed) rotate([0,-45,0]) photoResistorTube();
}

module allSeperators() {
  translate(lowEffectLedPos) ledSeperator();  
  translate(stbyTestLedPos) ledSeperator();
  translate(driftLed) ledSeperator();
}

module pellxEnclosure() {
  cornerDelta = 18;
  h = 9;
  thickness = 3;
  translate([0,0,-4]) cube([l,b,4]); //base
  translate([0,0,h]) cube([l,b,4]); // top
  translate([0,0,1]) cube([l,thickness,h]); // lower side
  translate([0,b-thickness,1]) cube([l,thickness,h]); // high side
  translate([0,0,1]) cube([thickness,b,h]); // left side
  translate([l-thickness,0,1]) cube([thickness,b,h]); // right side

  // Corners
  cube([cornerDelta,cornerDelta,h]);
  translate([0,b-cornerDelta,0]) cube([cornerDelta,cornerDelta,h]);
  translate([l-cornerDelta,b-cornerDelta,0]) cube([cornerDelta,cornerDelta,h]);
  translate([l-cornerDelta,0,0]) cube([cornerDelta,cornerDelta,h]);
}

module cutOut() {
  frame = 7;
  cornerR = 50;
  translate([frame+cornerR,frame+cornerR,-1]) minkowski() {
    cube([l-2*frame-2*cornerR, b-2*frame-2*cornerR,baseThickness]);
    cylinder(r=cornerR,h=2);
  }

}

module controlPanelInsert() {
  frame = 7;
  difference() {
    cube([l,b,baseThickness]); // Baseplate
    cutOut();
  }
}

module standoff(height) {
  mountingHoleRadius = 3.2 / 2;
  difference() {
    cylinder(r=1 + mountingHoleRadius, h = height);
    cylinder(r=mountingHoleRadius, h = height);    
  }
}

module fixPlate() {
  difference() {
    cube([13.5,50,baseThickness]);
    translate(hole1) hull() standoff(3.5);
    translate(hole2) hull() standoff(3.5);
    translate([0,7,0]) cube([3,36,baseThickness]);
  }
  translate(hole1) standoff(3.5);
  translate(hole2) standoff(3.5);
}

module clamp() {
  difference() {
    union() {
      cube([6,50,baseThickness]);
      translate([0,7,baseThickness]) cube([6,36,baseThickness]); 
    }
    translate([-3,0,0]) translate(hole1) cylinder(r=2,h=baseThickness);
    translate([-3,0,0]) translate(hole2) cylinder(r=2,h=baseThickness);
    hull() {
      translate([0.001,9,0]) cube([6,32,0.001]);
      translate([6,9,0]) cube([6,32,2*baseThickness]);
    }
  }
}



difference() {
  union() {
    difference() {
      controlPanelInsert();
      translate([3,61,0]) hull() fixPlate();
      translate([0,61+7,0]) cube([3,36,baseThickness]);
    }
    translate([-3,0,1]) allTubes();
    translate([-1,0,0]) allSeperators();
    translate([3,61,0]) fixPlate();
    translate([50,50,0]) clamp();
  }
  pellxEnclosure();
}
