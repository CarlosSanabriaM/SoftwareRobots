// Parametros 
alturaCilindros = 0.5;
radioCilindros = 2;
r = 45;
h = 20;
center = false;
$fn = 30;

module parteSuperior(){
    //Ahuecar
    difference(){
        //unir semiesfera y cilindro
        union(){
            difference(){
                cylinder(h=0.2, r = 47, center = true, $fn=$fn);
                translate([-27, -30, 0])
                cilindro();
                translate([27, 30, 0])
                 cilindro();
                translate([27, -30, 0])
                 cilindro();
                translate([-27, 30, 0])
                 cilindro();
             }
                difference(){ 
                    //-- Cilindro con tapa superior redondeada
                    parteArriba(r = r, h = h, both = false, center = center, $fn = $fn);
                    translate([0,0,-35]){
                        cube([100,100,70], center = true); 
                    }
                }
        }
        translate([0,0,-15]){
        parteArriba(r=45, h=30, both=false, center=center, $fn=$fn);
        }
    }
}

parteSuperior();

module cilindro(){
    cylinder(h=alturaCilindros, r = radioCilindros, center = true);
}

module parteArriba(r = 4, 
                 h = 20, 
                 center = false, 
                 both = false, $fn = 30)
{
    hc = (both == true) ? h - 2 * r : h - r;
    //-- Calcular la traslacion para centrar el cilindro
    posc = (center == true) ? 0 : h/2;
      
    //-- Aplicar traslacion para el centrado
    translate([0, 0, posc]) 
     //-- Centrar la figura
    translate([0, 0, -h/2]) {
        //-- Cilindro + esfera
        cylinder(r = r, h = hc, $fn = $fn);
        translate([0, 0, hc]) 
        sphere(r = r, $fn = $fn);
     }
}
