Num_Caras_Cilindro = 30;
H_cilindros_traseros = 55;
Diam_cilindros_small = 1;

module legPartv3(){
    
  // Parte trasera
  difference(){

    translate([-17,27.5,5])
      cube ([40,3,10], center = true);

    translate([-17,30,5])
      rotate([90,0,0])
        cylinder (h = H_cilindros_traseros, d = 4, center = true, $fn = Num_Caras_Cilindro);
    
    translate([-8,30,5])
      rotate([90,0,0])
        cylinder (h = H_cilindros_traseros, d = Diam_cilindros_small, center = true, $fn = Num_Caras_Cilindro);
    
    translate([-26,30,5])
      rotate([90,0,0])
        cylinder (h = H_cilindros_traseros, d = Diam_cilindros_small, center = true, $fn = Num_Caras_Cilindro);
  }  

  // Brazo derecho
  difference(){
    rotate([0,0,90])
      translate([17,-1.5,5])
        cube ([24,3,10], center = true);
    
    translate([19,18,5])
      rotate([0,90,0])
        cylinder (h = 55, d = Diam_cilindros_small, center = true, $fn = Num_Caras_Cilindro);
    
    translate([27,12,5])
      rotate([0,90,0])
        cylinder (h = 60, d = Diam_cilindros_small, center = true, $fn = Num_Caras_Cilindro); 

    translate([27,5,5])
      rotate([0,90,0])
        cylinder (h = 60, d = 5, center = true, $fn = Num_Caras_Cilindro); 
  } 
    
  // Cilindro Derecha
  difference(){ 
    rotate([0,90,0])
      translate([-5,5,0])
        cylinder(d=10,h=3, $fn = Num_Caras_Cilindro);
    
    translate([-5,5,5])
      rotate([0,90,0])
        cylinder (h = 55, d = 5, center = true, $fn = Num_Caras_Cilindro); 
  }

  // Brazo Izquierdo
  rotate([0,0,90])
    translate([17,35.5,5])
      cube ([24,3,10], center = true);
  
  // Cilindro Izquierda
  rotate([0,90,0])
    translate([-5,5.5,-37])
      cylinder(d=10,h=3, $fn = 20);

  // Engranaje Derecha
  rotate([0,90,0])
    translate([-5,5,-37])
      cylinder(d=4.5,h=7.5, $fn = 20);

}