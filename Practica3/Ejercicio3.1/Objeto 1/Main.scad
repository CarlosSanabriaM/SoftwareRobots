use <BattRack.scad> 
use <Foot-Left.scad>
use <Foot-Right.scad>
use <Hip.scad>
use <LegPart-v3.scad>
use <PhoneHolder.scad>
use <ServoBrace.scad>

// Pata izquierda
translate([40,0,0])
  footleft();

// Pata derecha
translate([-30,0,0])
  footright();
  
// Enganche pata izquierda
translate([30,20,5])
  rotate([90,0,90])
    legPartv3();

// Enganche pata derecha
translate([-40,20,5])
  rotate([90,0,90])
    legPartv3();

// Base
translate([0,0,40])
  hip();
  
// Base del movil
translate([0,0,40])
  phoneHolder();

// Pieza trasera
translate([0,18,48])
  battrack();

// Engache trasero para servos
translate([0,30,54])
  rotate([90,0,0])
    servoBrace();