use <20W.scad> 
use <4X5JP.scad>
use <1X2AB.scad>
use <DCGEARED.scad>

// Ruedas eje trasero
translate([20,82,20])
  rotate([90,0,0])
    rueda();

translate([20,-18,20])
  rotate([90,0,0])
    rueda();

//Ruedas eje delantero
translate([80,82,20])
  rotate([90,0,0])
    rueda();

translate([80,-18,20])
  rotate([90,0,0])
    rueda();

// Plancha
translate([0,-10,30])
  plancha();

//Sujecion rueda delantera izquierda
rotate([180,360,90])
  translate([-10,60,-30])
    sujecion_rueda();

//Sujecion rueda delantera derecha
rotate([180,360,90])
  rotate([0,0,180])
    translate([-70,-100,-30])
      sujecion_rueda();

// Soporte trasero izquierdo
rotate([90,0,0])
  translate([0,10,-32])
    trasera();

// Soporte trasero derecho
rotate([90,0,0])
  translate([0,10,-69])
    trasera();

//Eje trasero
translate([20,30,20])
  rotate([90,0,0])
    cylinder (h = 120, d = 5, center = true);

//Eje delantero
translate([80,30,20])
  rotate([90,0,0])
    cylinder (h = 120, d = 5, center = true);
