
// Medidas Agujeros
H_Cilindro = 20;
D_Cilindro = 5;
Num_Caras_Cilindro = 30;

// Dimensiones cortes diagonales
SizeX_Corte_Diagonal = 10;
SizeY_Corte_Diagonal = 10;
SizeZ_Corte_Diagonal = 35;

module sujecion_rueda(){
  union(){
    difference(){
      // Soporte Central
      rotate([0,0,90])
        translate([20,-2,10])
          cube ([40,3,20], center = true);

      // Agujero central
      translate([0,20,10])
        rotate([0,90,0])
          cylinder (h = H_Cilindro, d = D_Cilindro, center = true, $fn = Num_Caras_Cilindro);
   
      // Corte diagonal derecha
      translate([0,42,10])
        rotate([30,0,0])
          cube([SizeX_Corte_Diagonal,SizeY_Corte_Diagonal,SizeZ_Corte_Diagonal], center = true);

      //Cortar diagonal izquierda   
      translate([0,-2,10])
        rotate([60,0,0])
          rotate([90,0,0])
            cube([SizeX_Corte_Diagonal,SizeY_Corte_Diagonal,SizeZ_Corte_Diagonal], center = true);
    }

    difference(){
      // Soporte Base
      rotate([90,0,0])
        translate([10,1.5,-20])
          cube ([20,3,39], center = true);
    
      // Agujero base izquierda
      translate([10,10,0])
        rotate([0,0,90])
          cylinder (h = H_Cilindro, d = D_Cilindro, center = true, $fn = Num_Caras_Cilindro);
     
      // Agujero base derecha
      translate([10,30,0])
        rotate([0,0,90])
          cylinder (h = H_Cilindro, d = D_Cilindro, center = true, $fn = Num_Caras_Cilindro); 
    }
  }
}
