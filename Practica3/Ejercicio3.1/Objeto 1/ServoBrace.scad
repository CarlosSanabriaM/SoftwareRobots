RotX_pared_interna = 90;
RotY_pared_interna = 0;
RotZ_pared_interna = 0;
    
TranX_pared_interna = 10;
TranY_pared_interna = 14;
TranZ_pared_interna = 12;
    
SizeX_pared_interna = 20;
SizeY_pared_interna = 2;
SizeZ_pared_interna = 23;

module servoBrace(){
  union(){

    difference(){
      // Pilar central frontal
      translate([0,0,12])
        cube ([30,2,23], center = true);
        
      // Agujero central
      translate([0,0,12])
        rotate([90,0,0])
          cylinder (h = 55, d = 3, center = true, $fn = 30);
    }
    
    difference(){
      // Pilar izquierdo frontal
      translate([-32,0,12])
        cube ([10,2,23], center = true);

      // Agujero izquierdo
      translate([-33.6,0,12])
        rotate([90,0,0])
          cylinder (h = 55, d = 3, center = true, $fn = 30);
    }
    
    difference(){
      // Pilar derecho frontal
      translate([32,0,12])
        cube ([10,2,23], center = true);

      // Agujero derecho
      translate([33.4,0,12])
        rotate([90,0,0])
          cylinder (h = 55, d = 3, center = true, $fn = 30);
    }
 
    // Pilar izquierdo trasero
    translate([-21,19,12])
      cube ([16,2,23], center = true);
    
    // Pilar derecho trasero
    translate([21,19,12])
      cube ([16,2,23], center = true);

    // Paredes Internas
    
    rotate(RotX_pared_interna,RotY_pared_interna,RotZ_pared_interna)
      translate([TranX_pared_interna,TranY_pared_interna,TranZ_pared_interna])
        cube ([SizeX_pared_interna,SizeY_pared_interna,SizeZ_pared_interna], center = true);
 
    rotate(RotX_pared_interna,RotY_pared_interna,RotZ_pared_interna)
      translate([TranX_pared_interna,-TranY_pared_interna,TranZ_pared_interna])
        cube ([SizeX_pared_interna,SizeY_pared_interna,SizeZ_pared_interna], center = true);
 
    rotate(RotX_pared_interna,RotY_pared_interna,RotZ_pared_interna)
      translate([TranX_pared_interna,TranY_pared_interna*2,TranZ_pared_interna])
        cube ([SizeX_pared_interna,SizeY_pared_interna,SizeZ_pared_interna], center = true);
 
    rotate(RotX_pared_interna,RotY_pared_interna,RotZ_pared_interna)
      translate([TranX_pared_interna,-TranY_pared_interna*2,TranZ_pared_interna])
        cube ([SizeX_pared_interna,SizeY_pared_interna,SizeZ_pared_interna], center = true);

  }
}
