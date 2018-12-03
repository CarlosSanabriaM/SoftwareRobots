use <BattRack.scad> 
use <Foot-Left.scad>
use <Foot-Right.scad>
use <Hip.scad>
use <LegPart-v3.scad>
use <PhoneHolder.scad>
use <ServoBrace.scad>

//base del movil
translate([0,0,40])
  phoneHolder();

//pieza detras
translate([0,18,48])
  battrack();

//pata izquierda
translate([40,0,0])
  footleft();

//pata derecha
translate([-30,0,0])
  footright();

//base
translate([0,0,40])
  hip();
//pieza pata izquierda
translate([30,20,5])
  rotate([90,0,90])
    legPartv3();

//pieza pata derecha
translate([-40,20,5])
  rotate([90,0,90])
    legPartv3();

//pieza detras
translate([0,30,54])
  rotate([90,0,0])
    servoBrace();