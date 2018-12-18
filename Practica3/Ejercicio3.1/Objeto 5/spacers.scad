alturaCilindros = 18;
dmCilindros = 4;
distancia = 10;

//Pieza
module pieza(){
    cylinder(h= alturaCilindros*2, d = dmCilindros, center = true, $fn=15);
}
//Repeticiones
for (i = [1 : 2])
 {      
     difference(){
         translate([-55+(i*distancia), -8, 12])
         pieza();
          translate([-55+(i*distancia), -8 , 12])
            cylinder(h= alturaCilindros*2, d =      dmCilindros-1, center = true, $fn=15);
     }
 }

 for (i = [1 : 2])
 {
     difference(){
        translate([-55+(i*distancia), -20 , 12])
        pieza();
        translate([-55+(i*distancia), -20 , 12])
        cylinder(h= alturaCilindros*2, d =      dmCilindros-1, center = true, $fn=15);
     }
 }   
 

