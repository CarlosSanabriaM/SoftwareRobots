
// Medidas para los agujeros
Desp_EjeX = 10;
Desp_EjeY_inicial = 10;
Separacion_Entre_Agujeros = 20;
Desp_EjeZ = 10;
H_Cilindros = 20;
D_Cilindros = 5;
Num_caras_cilindros = 30;


// Para entender la nomenclatura de esta pieza utilizar la vista top

module plancha(){
  difference(){
      
    // Base
    cube([100,80,3]);

    // De izquierda a derecha
    for (i = [0 : 3]){

      // Primera columna
      translate([Desp_EjeX,Desp_EjeY_inicial+(Separacion_Entre_Agujeros*i),-Desp_EjeZ])
        cylinder(h= H_Cilindros, d = D_Cilindros, $fn=Num_caras_cilindros);
      
      // Segunda columna
      translate([Desp_EjeX*3,Desp_EjeY_inicial+(Separacion_Entre_Agujeros*i),-Desp_EjeZ])
        cylinder(h= H_Cilindros, d = D_Cilindros, $fn=Num_caras_cilindros);

      // Tercera columna
      translate([Desp_EjeX*5,Desp_EjeY_inicial+(Separacion_Entre_Agujeros*i),-Desp_EjeZ])    
        cylinder(h= H_Cilindros, d = D_Cilindros, $fn=Num_caras_cilindros);

      // Cuarta columna
      translate([Desp_EjeX*7,Desp_EjeY_inicial+(Separacion_Entre_Agujeros*i),-Desp_EjeZ])     
        cylinder(h= H_Cilindros, d = D_Cilindros, $fn=Num_caras_cilindros);

      // Quinta columna
      translate([Desp_EjeX*9,Desp_EjeY_inicial+(Separacion_Entre_Agujeros*i),-Desp_EjeZ])
        cylinder(h= H_Cilindros, d = D_Cilindros, $fn=Num_caras_cilindros);
    } 
  }
}
