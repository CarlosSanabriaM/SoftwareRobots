radioCilindro = 48;
radioCilindrosAgujero= 3;
dimBarra = [10,180,10];
dimVacioBarra = [8,170,8];
module cilindroUno(){
    translate([-50,-50,0])
        cylinder(h=1, r=radioCilindro, center= true);
}
module parteBaja(){
    difference(){
          cilindroDos();
            cilindrosGrandesAgujero(-50,10,0);
            cilindrosGrandesAgujero(-90,50,0);
            cilindrosGrandesAgujero(-10, 50,0);
            cilindrosGrandesAgujero(-50, 90,0);
            cilindrosGrandesAgujero(-50, 38,0);
            cilindrosGrandesAgujero(-38,50,0);
            cilindrosGrandesAgujero(-62, 50,0);
            cilindrosGrandesAgujero(-50, 62,0);
            //Pequeños
            cilindrosPequeAgujero(-50,20,0);
            cilindrosPequeAgujero(-80,50,0);
            cilindrosPequeAgujero(-20,50,0);
            cilindrosPequeAgujero(-50,80,0);
            
            cilindrosPequeAgujero(-28,28,0);
            cilindrosPequeAgujero(-72,28,0);
            cilindrosPequeAgujero(-28,70,0);
            cilindrosPequeAgujero(-72,72,0);
        }
}
module tornillo(){
    cylinder(h=15, d = 4, center = true, $fn=15);
}

parteBaja();
parteMedia();
helice();
module parteMedia(){
    difference(){
        cilindroUno();
        cilindrosGrandesAgujero(-50,-10,0);
        cilindrosGrandesAgujero(-90,-50,0);
        cilindrosGrandesAgujero(-10,-50,0);
        cilindrosGrandesAgujero(-50,-90,0);
        cilindrosGrandesAgujero(-50,-38,0);
        cilindrosGrandesAgujero(-38,-50,0);
        cilindrosGrandesAgujero(-62,-50,0);
        cilindrosGrandesAgujero(-50,-62,0);
    }
}
module cilindroDos(){
        translate([-50,50,0]){
            cylinder(r=radioCilindro);
        }
}
module cilindrosGrandesAgujero(x,y,z){
    translate([x,y,z]){
        cylinder(r=radioCilindrosAgujero,h = 50, center=true);
    }
}

module cilindrosPequeAgujero(x,y,z){
    translate([x,y,z]){
        cylinder(r=2,h = 80, center=true);
    }
}

module barras(){
    difference(){
        translate([5,-90,0])
        cube(dimBarra);
        translate([6,90,1])
        cube(dimVacioBarra);
        translate([10,80,0])
        cilindrosPequeAgujero(); 
        translate([10,55,0])
        cilindrosPequeAgujero(); 
    }
}
module hexagono(){
    translate([10,-78,0])
    rotate([0,0,90])
    cylinder(r=15,h = 2, center=true, $fn=6);
}
module helice(){
    difference(){
        union(){
            union(){
                barras();
                hexagono();
            }
            translate([10,-55,10])
            cylinder(r1=5,r=0,h=40);
            //Tornillos
            translate([10,-70,5])
            tornillo();
            translate([10,-85,5])
            tornillo();
        }
        translate([10,-70,0])
        cilindrosPequeAgujero();
        translate([10,-85,0])
        cilindrosPequeAgujero();
    }
}