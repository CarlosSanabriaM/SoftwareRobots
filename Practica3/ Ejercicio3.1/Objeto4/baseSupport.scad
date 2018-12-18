r = 15; //-- Radio de las curvas
vertice = [
  [-8, -35, 0],
  [-8, -35, 40],
  [-8, -85, 40],
  [-8, -85, 0],
  [10, -35, 0],
  [10, -35, 40],
  [10, -85, 0],
  [10, -85, 40],
]; 
baseSupport();
module baseSupport(){
    difference(){
        hull(){
          for (pos = vertice)
            translate(pos)
              sphere(r = r, $fn = 50);
        }
        translate([-30,-110,-20])
            cube(size = [70,90,35], center = true/false, $fn=6);
    } 
}