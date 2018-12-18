vertice = [
                  [60, 59, 0],
                  [60, 59, 30],
                  [60, -59, 30],
                  [60, -59, 0],
                  [-60, 59, 30],
                  [-60, 59, 0],
                  [-60, -59, 30],
                  [-60, -59, 0],
                  //parte delantera
                  [4, -70, 30],
                  [4, -70, 0],
                  [-4, -70, 30],
                  [-4, -70, 0]

    ];
verticeDos = [
                  [58, 58, 1],
                  [58, 58, 30],
                  [58, -58, 30],
                  [58, -58, 1],
                  [-58, 58, 30],
                  [-58, 58, 1],
                  [-58, -58, 30],
                  [-58, -58, 1],
                  //parte delantera
                  [4, -69, 30],
                  [4, -69, 0],
                  [-4, -69, 30],
                  [-4, -69, 0],

    ];
radioAgujeroGrande = 10;
radioAgujeroPeque = 2;
module agujeroGrande(){
        cylinder(r=radioAgujeroGrande, h=30, center=true, $fn=50);
}

module agujeroPeque(){
    cylinder(r=radioAgujeroPeque, h=30, center=true, $fn=50);
}

//Hueco
module base(){
    difference(){
        hull(){
          for (pos = vertice)
            translate(pos)
              cylinder(r = 10, center = true, $fn =50);
        }
        hull(){
          for (pos = verticeDos)
            translate(pos)
              cylinder(r = 8, center = true, $fn = 50);
        }
        translate([60,-5,0])
            rectangulo(5, 50, 10);
        translate([-60,-5,0])
            rectangulo(5, 50, 10);
        rotate([180,0,0])
            translate([45,-58,-20])
                rectangulo(15, 30, 10);
        //Agujeros
        //Izquierda
        rotate([90,0,0])
        translate([-20, 15, 80])
            agujeroGrande();
        rotate([90,0,0])
        translate([-48, 15, 80])
            agujeroGrande();
        //Derecha
        rotate([90,0,0])
        translate([20, 15, 80])
            agujeroGrande();
        rotate([90,0,0])
        translate([48, 15, 80])
            agujeroGrande();
        //Base
        translate([6, -60, 10])
            agujeroPeque();
        translate([-6, -60, 10])
            agujeroPeque();
        //Atras
        translate([-20, 50, 1])
             agujeroPeque();
        translate([20, 50, 1])
             agujeroPeque();
    }
}
pieza();
//Pieza
module pieza(){
    union(){
        base();
        translate([-63, -55, 18])
            columna(28);
        translate([63, -55, 18])
            columna(28);
        translate([-13, 60, 10])
            columna(15);
        translate([13, 60, 10])
            columna(15);
    }
}

//Columnas
module columna(h){
    difference(){
        rotate([0,0,45])
        cylinder(r=5, h=h, center=true, $fn=4);
        cylinder(r=2, h=40, center=true, $fn=50);
    }
}
module rectangulo(x,y,z){
    cube([x, y, z],center=true); 
}
