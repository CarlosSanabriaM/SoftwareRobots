radioAgujero=1;
module rectangulo(largo, alto, grosor){
    rotate([90,0,0])
    cube([largo, alto, grosor],center=true); 
}
module holder(){
    union(){
        difference(){
            rectangulo(120,110,1);
            translate([-25,3,0])
                rectangulo(20,40,20);
            translate([-20,3,-35])
                rectangulo(8,15,20);
            translate([-13,3,34])
                rectangulo(13,15,20);
            //Grande
            translate([12,3,22])
                rectangulo(33,46,20);
            translate([37,3,-16])
                rectangulo(4,10,20);
            translate([35,3,19])
                rectangulo(8,40,20);
            //Horizontal
            translate([13,3,-32])
                rectangulo(52,15,20);
            //Agujeros grandes
            translate([4,3,-7])
                agujero(2);
            translate([-9,3,-18])
                agujero(2);
            translate([20,3,-18])
                agujero(2);
            translate([30,3,-7])
                agujero(2);
            translate([-35,3,40])
                agujero(2);
            translate([-35,3,28])
                agujero(2);
            //Izquierda
            translate([-45,3,15])
                agujero(2);
            translate([-45,3,-17])
                agujero(2);
            translate([-35,3,-40])
                agujero(2);
            translate([-35,3,-28])
                agujero(2);
            //Superiores
            translate([-12,3,-45])
                agujero(2);
            translate([20,3,-45])
                agujero(2);
            translate([30,3,-45])
                agujero(1.5);
            translate([39,3,-45])
                agujero(1.5);
            //Inferiores
            translate([-9,3,44])
                agujero(2);
            translate([35,3,44])
                agujero(2);
            //Derecha
            for(i = [1: 6]){
                translate([45,3,-39+(i*11)   ])
                    agujero(2);
            }
        }
    }
    anexo(54.5,0,-61.5);
    anexo(54.5,0,61.5);
}

module agujero(radioAgujero){
    rotate([90,0,0])
    cylinder(r=radioAgujero,h=10,$fn=50);
}
holder();
module anexo(x,y,z){
    difference(){
        translate([x,y,z])
        rectangulo(10,15,1);
        translate([x,5,z])
        agujero(2);
    }
}
    