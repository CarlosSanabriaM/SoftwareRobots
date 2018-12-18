use <base.scad>
use <baseSupport.scad>
use <boardHolder.scad>
use <motorCover.scad>
use <robotWheelSuport.scad>

union(){
    //Rueda izda
    translate([-65,0,10])
    piezaRueda();
    //Rueda derecha
    translate([60,0,10])
    piezaRueda();
    //Base
    pieza();
    //Soporte e la base
    rotate([180,0,0])
        translate([0,60,-15])
            baseSupport();
    //Ranuras
    rotate([90,180,90])
    translate([0,-30,0])
        holder();
}
