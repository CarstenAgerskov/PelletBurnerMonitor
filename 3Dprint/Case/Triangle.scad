module triangle(x,y,h) {
	linear_extrude(height = h) polygon(points=[[0,0],[x,0],[0,y],[0,0]]);
}

module roundedTriangle(x,y,h,r) {
	translate([r,r,0]) minkowski() {
		triangle(x-r,y-r,h-1);
		cylinder(r=r,h=1);
	}
}
