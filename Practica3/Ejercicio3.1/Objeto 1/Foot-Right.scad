
module footright(){
  
  // Base redondeada
  minkowski(){
    translate([-30,-27,-2])
      cube([44.5,54,5]);
    cylinder(d=9,h=1, $fn = 30);
  }

  // Pilar extremo
  translate([15,0,9])
    cube(size = [5, 5, 14], center = true);

  // Pilar central
  translate([-13.5,2.4,9])
    cube(size = [5, 10.2, 14], center = true);
}