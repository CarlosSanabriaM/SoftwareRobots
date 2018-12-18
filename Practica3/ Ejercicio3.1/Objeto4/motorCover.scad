cuboBase();
module cuboBase(){
    union(){
        cube(size = [40,20,5], center = true);
        apendice();
        translate([-17.0*2,-12.7*2,0])
        apendice();
    }
}
    //Apendices
module apendice(){
    difference(){
        translate([17.0,12.9,0])
        cube(size = [6,6,5], center = true);
        translate([17.0,12.9,-4])
        agujero();
    }
}

module agujero(){
    cylinder(r=2,h=20,$fn=100);
}