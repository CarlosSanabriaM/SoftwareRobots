
module footleft(){
    
  // Base redondeada
  minkowski(){
    translate([-22.5,-27,-2])
      cube([45,54,5]);
    cylinder(d=9,h=1, $fn = 30);
  }

  // Pilar del extremo
  translate([-23,0,9])
    cube(size = [5, 5, 14], center = true);

  // Pilar central
  translate([6,2.4,9])
    cube(size = [5, 10.2, 14], center = true);
}