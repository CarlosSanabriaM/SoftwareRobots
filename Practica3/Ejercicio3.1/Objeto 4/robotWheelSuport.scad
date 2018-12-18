radioCirculo = 25;
radioAgujero = 7;

//Cilindro grande
module cilindroGrande(){
    cylinder(r=radioCirculo, h=3, $fn=100);
}

piezaRueda();

module piezaRueda(){
    difference(){
        union(){
            rueda();
            tuerca();
        }
        //falta agujero
        rotate([0,90,0])
        translate([0,0,-3])
        cylinder(r=1, h=80, $fn=100, center = true);
    }
}
module rueda(){
    difference(){
        rotate([0,90,0])
        cilindroGrande();
        //izquierda
        rotate([0,90,0])
        translate([0,14,0])
        agujero(); 
        //derecha
        rotate([0,90,0])
        translate([0,-14,0])
        agujero();
        //arriba
        rotate([0,90,0])
        translate([-14,0,0])
        agujero();
        //abajo
        rotate([0,90,0])
        translate([14,0,0])
        agujero();

    }
}
module agujero(){
    cylinder(r=radioAgujero, h=10, $fn=100);
}
tuerca();
module tuerca(){
    difference(){
        rotate([0,90,0])
        translate([0,0,-1])
        cylinder(r=3, h=2, $fn=100);
        
    }
}