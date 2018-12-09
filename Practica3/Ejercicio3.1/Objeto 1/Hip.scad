Num_Caras_Cilindros = 30;

PosX_cuadrado = 21.5;
PosY_cuadrado = 0;
PosZ_cuadrado = 9;
X_size_cuadrado = 13;
Y_size_cuadrado = 24;
Z_size_cuadrado = 50;

Diam_max_circulos = 3;
PosX_max_circulos = 33.5;
PosY_max_circulos = 0;
PosZ_max_circulos = 10;
H_max_circulos = 20;

PosX_min_circulos = 21.3;
PosY_min_circulos = 13.8;
PosZ_min_circulos = 10;
Diam_min_circulos = 1.5;
H_min_circulos = 20;

module hip(){
  difference() {

    // Base redondeada
    minkowski(){
      translate([-35.5,-15,0.4])
        cube([71,30,3.2]);
      cylinder(d=9,h=1, $fn = Num_Caras_Cilindros);
    }
    
    // Nota: para entender las indicaciones utilizar la vista picada (top)

    // Agujero cuadrado izquierda
    translate([-PosX_cuadrado,PosY_cuadrado,PosZ_cuadrado])
      cube(size = [X_size_cuadrado, Y_size_cuadrado, Z_size_cuadrado], center = true);

    // Agujero cuadrado derecha
    translate([PosX_cuadrado,-PosY_cuadrado,PosZ_cuadrado])
      cube(size = [X_size_cuadrado, Y_size_cuadrado, Z_size_cuadrado], center = true);

    // Agujero circular centro
    translate([0,-0,-10])
      cylinder(d=Diam_max_circulos,h=H_max_circulos, $fn = Num_Caras_Cilindros);

    // Agujero circular izquierda
    translate([-PosX_max_circulos,PosY_max_circulos,-PosZ_max_circulos])
      cylinder(d=Diam_max_circulos,h=H_max_circulos, $fn = Num_Caras_Cilindros);

    // Agujero circular derecha
    translate([PosX_max_circulos,PosY_max_circulos,-PosZ_max_circulos])
      cylinder(d=Diam_max_circulos,h=H_max_circulos, $fn = Num_Caras_Cilindros);

    // Agujero circular arriba izquierda
    translate([-PosX_min_circulos,PosY_min_circulos,-PosZ_min_circulos])
      cylinder(d=Diam_min_circulos,h=H_min_circulos, $fn = Num_Caras_Cilindros);

    // Agujero circular arriba derecha
    translate([PosX_min_circulos,PosY_min_circulos,-PosZ_min_circulos])
      cylinder(d=Diam_min_circulos,h=H_min_circulos, $fn = Num_Caras_Cilindros);
    
    // Agujero circular abajo izquierda
    translate([-PosX_min_circulos,-PosY_min_circulos,-PosZ_min_circulos])
      cylinder(d=Diam_min_circulos,h=H_min_circulos, $fn = Num_Caras_Cilindros);

    // Agujero circular abajo derecha
    translate([PosX_min_circulos,-PosY_min_circulos,-PosZ_min_circulos])
      cylinder(d=Diam_min_circulos,h=H_min_circulos, $fn = Num_Caras_Cilindros);

  }
}