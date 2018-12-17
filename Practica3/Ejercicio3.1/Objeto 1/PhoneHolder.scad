PosX_soporte_movil = 34;
PosY_soporte_movil = 8;
PosZ_soporte_movil = 20;

RotX_soporte_movil = 0;
RotY_soporte_movil = 0;
RotZ_soporte_movil = 90;

SizeX_soporte_movil = 15;
SizeY_soporte_movil = 2;
SizeZ_soporte_movil = 40;

module phoneHolder(){
    
  // Soporte movil central vertical trasero
  translate([0,0,55])
    cube ([70,2,107], center = true);
 
  // Soporte movil izquierda   
  translate([-PosX_soporte_movil,-PosY_soporte_movil,PosZ_soporte_movil])
    rotate([RotX_soporte_movil,RotY_soporte_movil,RotZ_soporte_movil])
      cube ([SizeX_soporte_movil,SizeY_soporte_movil,SizeZ_soporte_movil], center = true);
    
  // Soporte movil derecha
  translate([PosX_soporte_movil,-PosY_soporte_movil,PosZ_soporte_movil])
    rotate([RotX_soporte_movil,RotY_soporte_movil,RotZ_soporte_movil])
      cube ([SizeX_soporte_movil,SizeY_soporte_movil,SizeZ_soporte_movil], center = true);
     
  // Soporte movil central horizontal     
  translate([0,-6,5])
    cube ([70,14,10], center = true);

  // Soporte movil central vertical frontal
  translate([0,-14,7])
    cube ([70,2,15], center = true);
    
  // Soporte trasero vertical
  translate([0,3,45])
    cube ([50,2,20], center = true);

  // Soporte trasero horizontal
  rotate([90,0,0])
    translate([0,36,-10])
      cube ([50,5,15], center = true); 

  difference(){
    // Soporte trasero vertical izquierdo
    translate([-25,10,40])
      rotate([0,0,90])
        cube ([18,2,30], center = true);

    translate([-25,20,45])
      rotate([110,0,0])
        cube([10,29,10], center = true); 
      
    translate([-25,12,28])
      rotate([20,0,0])
        cube([10,20,10], center = true);
  }

  difference(){
    // Soporte trasero vertical derecho
    translate([25,10,40])
      rotate([0,0,90])
        cube ([18,2,30], center = true);
      
    translate([25,20,45])
      rotate([110,0,0])
        cube([10,29,10], center = true); 

    translate([25,12,28])
      rotate([20,0,0])
        cube([10,20,10], center = true);  
  }
}     