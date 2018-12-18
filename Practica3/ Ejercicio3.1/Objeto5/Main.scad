use <new_board_mount.scad>
use <spacers.scad>
use <cover.scad>
union(){
    //Parte abajo
    translate([50,-50,0])
    parteBaja();
    //Parte media
    translate([50,50,10])
    parteMedia();
    //Parte superior
    rotate([0,0,50])
    translate([0,0,20])
    parteSuperior();
    
    //Tornillos
    translate([41,0,5])
    pieza();
    translate([-41,0,5])
    pieza();
    translate([0,-41,5])
    pieza();
    translate([0,41,5])
    pieza();
    
    //Helices
    rotate([0,180,0])
    translate([-10,-98,0])
    helice();
    
    rotate([0,180,90])
    translate([-10,-98,0])
    helice();
    
    rotate([0,180,180])
    translate([-10,-98,0])
    helice();
    
    rotate([0,180,270])
    translate([-10,-98,0])
    helice();
}