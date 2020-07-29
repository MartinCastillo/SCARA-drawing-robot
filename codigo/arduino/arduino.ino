// Este archivo implementa la cinemática inversa de un brazo robtico SCARA
// En lenguaje Arduino, basado en el codigo de https://appliedgo.net/roboticarm/

// algunos datos:
// largo1: largo de la primera sección del brazo
// largo2: largo de la segunda seccin
// angulo1: angulo del primer de la base
// angulo2: angulo de la articulacion

// largo1 y largo2 son números en centímetros
#include <Servo.h>
Servo servo1;
Servo servo2;
Servo servo3;
#include<math.h>
double largo1 = 10;  // esto se debe cambiar por el largo real
double largo2 = 10; // esto tambien

// valor de pi
double Pi = 3.14159;
// Funcion LeyDelCoseno
// esta función trigonometrica implementa la ley del coseno (trigonometria)
// entradas:
// A: angulo del primer lado
// B: angulo del segundo lado
// C: angulo del lado que está al frente del ángulo
// retorna: angulo que está al frente del lado C
//(A * A + B * B - C * C) / (2 * A * B);

///----------------------------------------------------0--------------------------
bool parser(String& mensaje, float& coord_x, float& coord_y)
{
  // primero revisamos si el formato cumple el formato pedido
  // la primera condicion confirma si el mensaje parte con una 'x'
  // la segunda condicion revisa si termina con un 'f'
  // la ultima asegura que haya una 'y' dentro del mensaje
  if (mensaje.startsWith("x") && mensaje.endsWith("f") && mensaje.indexOf("y") > 0)
  {
    // si es valido, queremos extraer los numeros
    // para eso, obtenemos los indices de los caracteres que van antes y despues de los numeros
    int index_x = mensaje.indexOf("x");
    int index_y = mensaje.indexOf("y");
    int index_f = mensaje.indexOf("f");

    // ahora que tenemos los indices de los caracteres, queremos extraer el substring (o subcadena) donde está el numero
    // la función substring(i,j) extrae el string desde el caracter en la posición i, hasta la j-1
    // por eso extraemos el string que va desde el i+1 hasta el j, para solamente extraer el numero que esta ENTRE los caracteres
    String str_coordenada_x = mensaje.substring(index_x + 1, index_y);
    String str_coordenada_y = mensaje.substring(index_y + 1, index_f);

    // ahora los pasamos a ints, porque no se pueden pasar a otro tipo de numero
    // en ese sentido, si necesitas pasarle algo más preciso, envía un int que represente milímetros,
    // y despues los divides por 10 para obtener un float en centímetros que le pasas a la función de la cinemática inversa
    // eso es lo que hacemos abajo
    int coordenada_x = str_coordenada_x.toInt();
    int coordenada_y = str_coordenada_y.toInt();

    // ahora las pasamos a centimetros y las guardamos en las variables que le pasamos a la funcion
    coord_x = coordenada_x / 10.0;
    coord_y = coordenada_y / 10.0;

    return true;
  }
  else {
    // sino, se retorna falso
    return false;
  }
}
///----------------------------------------------------0--------------------------

// Funcion Distancia
// Calcula la distancia de un punto al origen (eje del robot) con el teorema de Pitagoras
// x: distancia en el eje x
// y: distancia en el eje y

double distancia(double x, double y)//R
{
  return sqrt(x * x + y * y);
}

// esta funcion convierte angulos en radianes a grados
double radianesAGrados(double radianes)
{
  return radianes * 180.0 / Pi;
}

double tetha(double x, double y) { //tetha
  return  atan2(y , x); /////////////////////////////////////por revisar
}

// Funcion angulosMotores
// Esta funcion calcula lso valores de los motores (a1, a2) para mover el brazo a la posición x,y
// x: posicion objetivo (eje x)
// y: posición objetivo (eje y)
// a1: valor donde se guardara el resultado del angulo 1. Angulo en grados
// a2: valor donde se guardara el resultado del angulo 2. Angulo en grados
void angulosMotores(double x, double y, double& a1, double& a2) {
  double R = distancia(x, y);
  if (R > largo1 + largo2) {
    Serial.print("no llega el brazo");
  }
  double th = tetha(x, y);
  double D2 = (R * R - largo1 * largo1 - largo2 * largo2) / (2 * largo1 * largo2);
  double a2Radianes = acos(D2);
  
  double a1Radianes = th - atan2((largo2 * sin(a2Radianes)) , (largo1 + largo2 * cos(a2Radianes)));
  // ahora guardamos los valores en grados en a1 y a2
  a1 = radianesAGrados(a1Radianes);
  a2 = radianesAGrados(a2Radianes);
}

double moveraPunto(double X, double Y) {
  double angulo1 = 0.00;
  double angulo2 = 0.00;
  angulosMotores(X, Y, angulo1, angulo2);
  angulo1 = angulo1 + 90;
  //Serial.print("angulo 1: ");
  //Serial.print(angulo1);
  //Serial.print(" , angulo 2: ");
  Serial.println(angulo2);
  servo1.write(angulo1);
  servo2.write(angulo2);
}

double dibujaPunto(double X, double Y) {
  servo3.write(30);
  delay(100);
  moveraPunto(X, Y);
  delay(300);
  servo3.write(90);
  delay(500);
  servo3.write(30);
  //Envia mensaje a pc para enviar el siguente punto
  Serial.print('a');
}

double dibujaLineaA(double X, double Y) {//en proceso/////////////////
  servo3.write(90);
  delay(500);
  moveraPunto(X, Y);
  delay(300);
  servo3.write(30);
}

void setup()
{
  Serial.begin(9600);
  servo1.attach(3);
  servo2.attach(5);
  servo3.attach(6);
  servo1.write(90);
  servo2.write(0);
  servo3.write(30);
}

void loop()
{
  if (Serial.read() > 0) {
   String msg = Serial.readString();
   float coord_x = 0.0;
   float coord_y = 0.0;
   bool msg_valido = parser(msg , coord_x, coord_y);//cor x y cord y valen ahoa las coordenadas del mensage
   dibujaPunto(coord_x, coord_y);
  }
}
