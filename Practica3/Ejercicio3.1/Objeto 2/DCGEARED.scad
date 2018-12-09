
// Dimensiones cortes anchos
SizeX_corte_ancho = 3.5;
SizeY_corte_ancho = 70;
SizeZ_corte_ancho = 7;

// Dimensiones cortes estrechos
SizeX_corte_estrecho = 5;
SizeY_corte_estrecho = 70;
SizeZ_corte_estrecho = 4;

// Dimensiones cilindros
H_Cilindro = 30;
D_Cilindro = 5;
Num_Caras_Cilindro = 30;

module trasera(){
  difference(){
    //Central Izquierda
    rotate([0,90,0])
      translate([-20,10,20])
        cube ([40,20,40], center = true);
 
    // Corte ancho derecha
    rotate([90,0,0])
      translate([36.5,30,-10])
        cube ([SizeX_corte_ancho,SizeY_corte_ancho,SizeZ_corte_ancho], center = true); 
 
    // Corte ancho izquierda
    rotate([90,0,0])
      translate([3,30,-10])
        cube ([SizeX_corte_ancho,SizeY_corte_ancho,SizeZ_corte_ancho], center = true); 
      
    // Corte estrecho derecha
    rotate([90,0,0])
      translate([39,30,-10])
        cube ([SizeX_corte_estrecho,SizeY_corte_estrecho,SizeZ_corte_estrecho], center = true); 
    
    // Corte estrecho izquierda
    rotate([90,0,0])
      translate([1,30,-10])
        cube ([SizeX_corte_estrecho,SizeY_corte_estrecho,SizeZ_corte_estrecho], center = true); 

    // Corte central
    rotate([90,0,0])
      translate([20,40,-10])
        cube ([11,70,13], center = true); 
    
    // Agujero pequeño central
    translate([20,10,5])    
      cylinder (h = H_Cilindro, d = 3, center = true,$fn=Num_Caras_Cilindro);
    
    // Agujeros centrales
    translate([10,10,10])
      rotate([90,0,0])
        cylinder (h = H_Cilindro, d = D_Cilindro, center = true,$fn=Num_Caras_Cilindro);

    translate([30,10,10])
      rotate([90,0,0])
        cylinder (h = H_Cilindro, d = D_Cilindro, center = true,$fn=Num_Caras_Cilindro);

    translate([30,10,30])
      rotate([90,0,0])
        cylinder (h = H_Cilindro, d = D_Cilindro, center = true,$fn=Num_Caras_Cilindro);

    translate([10,10,30])
      rotate([90,0,0])
        cylinder (h = H_Cilindro, d = D_Cilindro, center = true,$fn=Num_Caras_Cilindro);
  }
}