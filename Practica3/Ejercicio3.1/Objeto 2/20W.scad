Num_Caras_Agujeros = 30;
H_Agujeros = 15;
D_Agujeros = 5;

module rueda(){
  translate([0,0,2])
    difference(){
      // Cilindro original
      translate([0,0,0])
        cylinder (h = 10, d = 40, center = true,$fn=24);

      // Cilindro superior
      translate([0,0,5])
        cylinder (h = 3, d = 35, center = true,$fn=Num_Caras_Agujeros);
 
      // Cilindro inferior
      translate([0,0,-5])
        cylinder (h = 3, d = 35, center = true,$fn=Num_Caras_Agujeros);

      // Agujeros \\
      // Agujero central
      translate([0,0,0])
        cylinder (h = H_Agujeros, d = D_Agujeros, center = true,$fn=Num_Caras_Agujeros);
 
      translate([10,0,0])
        cylinder (h = H_Agujeros, d = D_Agujeros, center = true,$fn=Num_Caras_Agujeros);
 
      translate([0,10,0])
        cylinder (h = H_Agujeros, d = D_Agujeros, center = true,$fn=Num_Caras_Agujeros);

      translate([0,-10,0])
        cylinder (h = H_Agujeros, d = D_Agujeros, center = true,$fn=Num_Caras_Agujeros);
 
      translate([-7,-7,0])
        cylinder (h = H_Agujeros, d = D_Agujeros, center = true,$fn=Num_Caras_Agujeros);
 
      translate([-7,7,0])
        cylinder (h = H_Agujeros, d = D_Agujeros, center = true,$fn=Num_Caras_Agujeros);
 
      translate([7,-7,0])
        cylinder (h = H_Agujeros, d = D_Agujeros, center = true,$fn=Num_Caras_Agujeros);
 
      translate([7,7,0])
        cylinder (h = H_Agujeros, d = D_Agujeros, center = true,$fn=Num_Caras_Agujeros);
 
      translate([-10,0,0])
        cylinder (h = H_Agujeros, d = D_Agujeros, center = true,$fn=Num_Caras_Agujeros);
    }
    
  difference(){
    translate([0,0,2.5])
      cylinder (h = 10, d = 10, center = true,$fn=Num_Caras_Agujeros);
      
    translate([0,0,2.5])
      cylinder (h = H_Agujeros, d = D_Agujeros, center = true,$fn=Num_Caras_Agujeros);
  }
}