
module battrack(){

  //Rectangulo central
  translate([0,0,10])
    difference() {
      translate([0,21,20])
        cube(size = [30, 41, 36], center = true);

      translate([0,0,0])
        cube(size = [12, 39, 40], center = false);
    
      translate([-12,0,0])
        cube(size = [12, 39, 40], center = false);
     
      translate([0,0,0])
        cube(size = [20, 42, 17], center = false);
     
      translate([-15,0,0])
        cube(size = [20, 42, 17], center = false);
    }
    
  // Pata derecha  
  translate([0,0,10])
    difference() {
      translate([14,1,0])
        cube(size = [14, 11, 22], center = false);
     
      translate([10,0,0])
        cube(size = [20, 40, 18], center = false);
    }    
 
  // Pata Izquierdo
  translate([0,0,10])
    difference() {
      translate([-28,1,0])
        cube(size = [14, 11, 22], center = false);
     
      translate([-35,0,0])
        cube(size = [26, 13, 18], center = false);    
    }
}  
   
     
     