// Medidas Patas laterales
anchoPata = 16.5;
largoPata = 11;
altoPata = 4;

desvioXPata = 12;
desvioYPata = 0.7;
desvioZPata = 0;

// Medidas Pilares laterales
anchoPilar = 3;
largoPilar = 41;
altoPilar = 20;

desvioXPilar = 12;
desvioYPilar = 0.7;
desvioZPilar = 0;

// Medidas PT
// PT: Pilar trasero
anchoPT = 30;
largoPT = 3;
altoPT = 20;

desvioXPT = 0;
desvioYPT = 40.2;
desvioZPT = 10;

// Patas Laterales
translate([desvioXPata,desvioYPata,desvioZPata])
  cube([anchoPata,largoPata,altoPata]);
  
translate([-(desvioXPata+anchoPata),desvioYPata,desvioZPata])
  cube([anchoPata,largoPata,altoPata]);

// Pilares Laterales
translate([desvioXPilar,desvioYPilar,desvioZPilar])
  cube([anchoPilar,largoPilar,altoPilar]);
  
translate([-(desvioXPilar+anchoPilar),desvioYPilar,desvioZPilar])
  cube([anchoPilar,largoPilar,altoPilar]);

// Pilar Trasero
translate([desvioXPT,desvioYPT,desvioZPT])
  cube([anchoPT,largoPT,altoPT], center=true);
