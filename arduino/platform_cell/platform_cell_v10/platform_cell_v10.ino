// Motor Y es el de los leds
// Motor X es el del enfoque del portamuestras (eje z del portamuestras)

#define Y_DIR_PIN 61 // poner n�mero del pin de direcci�n
#define Y_STEP_PIN 60 // poner n�mero de pin de conexi�n al motor
#define Y_ENABLE_PIN 56 // poner n�mero de pin de poner en marcha 
#define FIN_CARRERA_1 3 // poner n�mero de pin de final de carrera 1
#define FIN_CARRERA_2 2 // poner n�mero de pin de final de carrera 2
#define FIN_CARRERA_3 14 // poner n�mero de pin de final de carrera 3
#define X_DIR_PIN 55 // poner n�mero del pin de direcci�n
#define X_STEP_PIN 54 // poner n�mero de pin de conexi�n al motor
#define X_ENABLE_PIN 38 // poner n�mero de pin de poner en marcha 
#define TRIGGER_FILTRO_LINEAL 16 // poner n�mero de pin con el que leer el disparo de cada movimiento del filtro lineal
#define TRIGGER_CAMARA 17// poner n�mero de pin en que lanzamos un disparo TTL para accionar la c�mara

int stepDelay = 100; // tiempo de parada para controlar la velocidad


boolean pos_1 = false;
boolean pos_2 = false;
boolean pos_3 = false;
boolean pos_final = false;

boolean primer_arranque = true;
boolean primera_conexion = true;

int final_carrera_1 = 1;
int final_carrera_2 = 1;
int final_carrera_3 = 1;

  
int cont_led = 0;
int cont_filtro = 0;
int trigger = 0;


int cont = 0;


void setup() 
{
	// Marcar los pines como salida
	pinMode(Y_STEP_PIN, OUTPUT);
	pinMode(Y_DIR_PIN, OUTPUT);
	pinMode(Y_ENABLE_PIN, OUTPUT);
	digitalWrite(Y_ENABLE_PIN, LOW);
	
	pinMode(FIN_CARRERA_1, INPUT_PULLUP);
	pinMode(FIN_CARRERA_2, INPUT_PULLUP);
	pinMode(FIN_CARRERA_3, INPUT_PULLUP);
	
	pinMode(X_STEP_PIN, OUTPUT);
	pinMode(X_DIR_PIN, OUTPUT);
	pinMode(X_ENABLE_PIN, OUTPUT);
	digitalWrite(X_ENABLE_PIN, LOW);
		
	pinMode(TRIGGER_FILTRO_LINEAL, INPUT);
	pinMode(TRIGGER_CAMARA, OUTPUT);
	
	Serial.begin(115200);
}

void loop() 
{
	if (primera_conexion == true) // cuando encendemos el puerto serie mandamos la comprobacion de que es nuestro programa el que busca
		{
			delayMicroseconds(500);
			Serial.print("a"); // codigo ascii 97
			primera_conexion = false;
		}
	if ((primer_arranque == true) && (primera_conexion == false)) // cuando encendemos el programa nos vamos a la posicion del final de carrera n�1
		{
			posicion_1();
			primer_arranque = false;
			pos_final = true;
		}
	if ((primer_arranque == false) && (primera_conexion == false))	
  	{
  	  leer_serial();
  	  que_hacer();
  	  cont_led = 0;
  	  cont_filtro = 0;
  	  if (pos_final == true)
  	  {
  		  Serial.print("1");
  		  pos_final = false;
  	  }
  	}
}

//-------------------------------------------------------------------------------------------------------------------------------

void que_hacer() // lee del puerto serie y decide que hacer segun la información que recibe
{	
	switch (cont_led)
	{
		case 1:
		posicion_3();
		comprobar_salto();
		pos_final = true;
		break;
		
		case 2:
		posicion_2();
		comprobar_salto();
		pos_final = true;
		break;
		
		case 3:
		posicion_2();
		comprobar_salto();
		posicion_3();
		comprobar_salto();
		pos_final = true;
		break;
		
		case 4:
		posicion_1();
		comprobar_salto();
		pos_final = true;
		break;
		
		case 5:
		posicion_1();
		comprobar_salto();
		posicion_3();
		comprobar_salto();
		pos_final = true;
		break;
		
		case 6:
		posicion_1();
		comprobar_salto();
		posicion_2();
		comprobar_salto();
		pos_final = true;
		break;
		
		case 7:
		posicion_1();
		comprobar_salto();
		posicion_2();
		comprobar_salto();
		posicion_3();
		comprobar_salto();
		pos_final = true;
		break;

		case 8:
		mover_enfoque();
		break;
		
		case 9:
		autofocus();
		break;
				
	}
}

//---------------------------------------------------------------------------------------------------------------------------------

// Funciones para el control de la parte de los cubos de las leds

void comprobar_salto() // comprobamos si ha habido alg�n salto del filtro lineal y lo contabilizamos
{
  int contador = 0;

  while (contador < cont_filtro)
  {
    trigger = digitalRead(TRIGGER_FILTRO_LINEAL);
    if (trigger == 1)
    {
      contador++;
      delay(250);
    }
  }
}

void leer_serial()
{
	while (Serial.available()>0)
	{
		cont_led = Serial.parseInt();
		cont_filtro = Serial.parseInt();
		if (Serial.read() == '\n')
		{
			return;
		}
	}
}

void posicion_1()
{
	final_carrera_1 = digitalRead(FIN_CARRERA_1);
	while ((final_carrera_1 == 1) && (pos_1 == false)) // vamos a ir hasta la posicion del led 1
	{
		digitalWrite(Y_DIR_PIN, HIGH);
		digitalWrite(Y_STEP_PIN, LOW);
		delayMicroseconds(stepDelay);
		digitalWrite(Y_STEP_PIN, HIGH);
		delayMicroseconds(stepDelay);
		final_carrera_1 = digitalRead(FIN_CARRERA_1);
	}
	final_carrera_1 = digitalRead(FIN_CARRERA_1);
	if (final_carrera_1 == 0)
	{
		pos_1 = true;
		pos_2 = false;
		pos_3 = false;
	}
}

void posicion_2()
{
	final_carrera_2 = digitalRead(FIN_CARRERA_2);
	
	while ((final_carrera_2 == 1) && (pos_2 == false) && (pos_1 == true))
	{
		digitalWrite(Y_DIR_PIN, LOW);
		digitalWrite(Y_STEP_PIN, LOW);
		delayMicroseconds(stepDelay);
		digitalWrite(Y_STEP_PIN, HIGH);
		delayMicroseconds(stepDelay);
		final_carrera_2 = digitalRead(FIN_CARRERA_2);
	}
	while ((final_carrera_2 == 1) && (pos_2 == false) && (pos_3 == true))
	{
		digitalWrite(Y_DIR_PIN, HIGH);
		digitalWrite(Y_STEP_PIN, LOW);
		delayMicroseconds(stepDelay);
		digitalWrite(Y_STEP_PIN, HIGH);
		delayMicroseconds(stepDelay);
		final_carrera_2 = digitalRead(FIN_CARRERA_2);
	}
	if ((final_carrera_2 == 0) && (pos_3 == true))
	{
		for (int x=0; x<3200; x++)
		{
			digitalWrite(Y_DIR_PIN, HIGH);
			digitalWrite(Y_STEP_PIN, LOW);
			delayMicroseconds(stepDelay);
			digitalWrite(Y_STEP_PIN, HIGH);
			delayMicroseconds(stepDelay);
		}
		delay(100);
		final_carrera_2 = digitalRead(FIN_CARRERA_2);
		while ((final_carrera_2 == 1) && (pos_2 == false))
		{
			digitalWrite(Y_DIR_PIN, LOW);
			digitalWrite(Y_STEP_PIN, LOW);
			delayMicroseconds(stepDelay);
			digitalWrite(Y_STEP_PIN, HIGH);
			delayMicroseconds(stepDelay);
			final_carrera_2 = digitalRead(FIN_CARRERA_2);
		}
	}
	final_carrera_2 = digitalRead(FIN_CARRERA_2);
	if (final_carrera_2 == 0)
	{
		pos_1 = false;
		pos_3 = false;
		pos_2 = true;
	}
}

void posicion_3()
{
	final_carrera_3 = digitalRead(FIN_CARRERA_3);
	while ((final_carrera_3 == 1) && (pos_3 == false))
	{
		digitalWrite(Y_DIR_PIN, LOW);
		digitalWrite(Y_STEP_PIN, LOW);
		delayMicroseconds(stepDelay);
		digitalWrite(Y_STEP_PIN, HIGH);
		delayMicroseconds(stepDelay);
		final_carrera_3 = digitalRead(FIN_CARRERA_3);
	}
	final_carrera_3 = digitalRead(FIN_CARRERA_3);
	if (final_carrera_3 == 0)
	{
		pos_1 = false;
		pos_2 = false;
		pos_3 = true;
	}
}

//---------------------------------------------------------------------------------------------------------------------------------
// funciones para el control del micrometro

void autofocus()
{

  //// para las poleas de 20 Y 20 dientes: 400 es una vuelta completa del micrometro y 1 vueltas del motor. trabajando a pasos completos, 16 micropasos por paso.
  //
//  for (int x=0; x< 200; x++)
//  {
//    mover_enfoque_continuo_abajo(16);
//  }
//  delay(1000);
//  for (int x = 0; x< 400; x++)
//  {
//    mover_enfoque_continuo_arriba(16);
//    // enviamos un disparo a la camara para que tome el dato y esperamos el tiempo que tarda en tomarla hay que preguntar a Iv�n
//    digitalWrite(TRIGGER_CAMARA, HIGH);
//    delay(100);
//    digitalWrite(TRIGGER_CAMARA, LOW);
//    delay(900);
//  }
//  for (int x = 0; x< 200; x++)
//  {
//    mover_enfoque_continuo_abajo(16);
//  }
//  // volvemos a la posici�n inicial
//  delay(1000);
//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // para las poleas de 20 Y 20 dientes: 800 es una vuelta completa del micrometro y 2 vueltas del motor. trabajando a pasos completos, 8 micropasos por paso.
  
  //for (int x=0; x< 800; x++)
  //{
    //mover_enfoque_continuo_abajo(8);
  //}
  //delay(1000);
  //for (int x = 0; x< 1600; x++)
  //{
    //mover_enfoque_continuo_arriba(8);
    //// enviamos un disparo a la camara para que tome el dato y esperamos el tiempo que tarda en tomarla hay que preguntar a Iv�n
    //digitalWrite(TRIGGER_CAMARA, HIGH);
    //delay(100);
    //digitalWrite(TRIGGER_CAMARA, LOW);
    //delay(900);
  //}
  //for (int x = 0; x< 800; x++)
  //{
    //mover_enfoque_continuo_abajo(8);
  //}
  //// volvemos a la posici�n inicial
  //delay(1000);

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------- 

   //para las poleas de 44 Y 32 dientes: 550 es una vuelta completa del micrometro y 1,375 vueltas del motor. trabajando a pasos completos, 16 micropasos por paso.
  
  
  for (int x=0; x< 550; x++) // bajamos una vuelta desde el centro ( el centro se posiciona a mano actualmente)
  {
    mover_enfoque_continuo_abajo(16);
  }
  delay(1000);
  for (int x = 0; x< 1100; x++) // subimos la vuelta que hemos bajado y 
  {
    mover_enfoque_continuo_arriba(16);
    // enviamos un disparo a la camara para que tome el dato y esperamos el tiempo que tarda en tomarla hay que preguntar a Iv�n
    digitalWrite(TRIGGER_CAMARA, HIGH);
    delay(100);
    digitalWrite(TRIGGER_CAMARA, LOW);
    delay(900);
  }
  for (int x = 0; x< 550; x++)
  {
    mover_enfoque_continuo_abajo(16);
  }
  
  // volvemos a la posici�n inicial
  delay(1000);

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // para las poleas de 44 Y 32 dientes: 1100 es una vuelta completa del micrometro y 2,75 vueltas del motor trabajando a 1/2 pasos, 8 micropasos por paso 
  
  //for (int x=0; x< 1100; x++) // bajamos una vuelta desde el centro ( el centro se posiciona a mano actualmente)
  //{
    //mover_enfoque_continuo_abajo(8);
  //}
  //delay(1000);
  //for (int x = 0; x< 2200; x++) // subimos la vuelta que hemos bajado y
  //{
    //mover_enfoque_continuo_arriba(8);
    //// enviamos un disparo a la camara para que tome el dato y esperamos el tiempo que tarda en tomarla hay que preguntar a Iv�n
    //digitalWrite(TRIGGER_CAMARA, HIGH);
    //delay(100);
    //digitalWrite(TRIGGER_CAMARA, LOW);
    //delay(900);
  //}
  //for (int x = 0; x< 1100; x++)
  //{
    //mover_enfoque_continuo_abajo(8);
  //}
  //
  //// volvemos a la posici�n inicial
  //delay(1000);
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // para las poleas de 44 Y 20 dientes: , 880 es una vuelta completa del micrometro y 2,2 vueltas del motor trabajando a paso completo, 16 micropasos por paso

  //for (int x=0; x< 880; x++) // bajamos una vuelta desde el centro ( el centro se posiciona a mano actualmente)
  //{
    //mover_enfoque_continuo_abajo(16);
  //}
  //delay(1000);
  //for (int x = 0; x< 1760; x++) // subimos la vuelta que hemos bajado y
  //{
    //mover_enfoque_continuo_arriba(16);
    //// enviamos un disparo a la camara para que tome el dato y esperamos el tiempo que tarda en tomarla hay que preguntar a Iv�n
    //digitalWrite(TRIGGER_CAMARA, HIGH);
    //delay(100);
    //digitalWrite(TRIGGER_CAMARA, LOW);
    //delay(900);
  //}
  //for (int x = 0; x< 880; x++)
  //{
    //mover_enfoque_continuo_abajo(16);
  //}
  //
  //// volvemos a la posici�n inicial
  //delay(1000);

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // para las poleas de 44 Y 20 dientes: , 1760 es una vuelta completa del micrometro y 4,4 vueltas del motor. trabajando a 1/2 pasos, 8 micropasos por paso

  //for (int x=0; x< 1760; x++) // bajamos una vuelta desde el centro ( el centro se posiciona a mano actualmente)
  //{
    //mover_enfoque_continuo_abajo(8);
  //}
  //delay(1000);
  //for (int x = 0; x< 3520; x++) // subimos la vuelta que hemos bajado y
  //{
    //mover_enfoque_continuo_arriba(8);
    //// enviamos un disparo a la camara para que tome el dato y esperamos el tiempo que tarda en tomarla hay que preguntar a Iv�n
    //digitalWrite(TRIGGER_CAMARA, HIGH);
    //delay(100);
    //digitalWrite(TRIGGER_CAMARA, LOW);
    //delay(900);
  //}
  //for (int x = 0; x< 1760; x++)
  //{
    //mover_enfoque_continuo_abajo(8);
  //}
  //
  //// volvemos a la posici�n inicial
  //delay(1000);

Serial.print("1");  
}

void mover_enfoque_continuo_arriba(int pasos)
{
  //el motor tiene 400 pasos por vuelta y el driver est� cnfigurado a 1/16 o 6400 micrpaos por vuelta.
  //Si ponemos los micropasos a 8 estamos trabajando al equivalente a 1/2 pasos 800 pasos por vuelta
  //int pasos = 8;
  //Si ponemos los micropasos a 16 estamos trabajando al equivalente a full steep 400 pasos por vuelta
  //int pasos = 16; 
  
  
  boolean dir = LOW;
  while (pasos > 0)
  {
    digitalWrite(X_DIR_PIN, dir);
    digitalWrite(X_STEP_PIN, LOW);
    delayMicroseconds(stepDelay);
    digitalWrite(X_STEP_PIN, HIGH);
    delayMicroseconds(stepDelay);
    pasos--;
  }
}

void mover_enfoque_continuo_abajo(int pasos)
{
  //el motor tiene 400 pasos por vuelta y el driver est� cnfigurado a 1/16 o 6400 micrpaos por vuelta. 
  //Si ponemos los micropasos a 8 estamos trabajando al equivalente a 1/2 pasos 800 pasos por vuelta
  //int pasos = 8;
  //Si ponemos los micropasos a 16 estamos trabajando al equivalente a full steep 400 pasos por vuelta
  //int pasos = 16;
  
  boolean dir = HIGH;
  while (pasos > 0)
  {
    digitalWrite(X_DIR_PIN, dir);
    digitalWrite(X_STEP_PIN, LOW);
    delayMicroseconds(stepDelay);
    digitalWrite(X_STEP_PIN, HIGH);
    delayMicroseconds(stepDelay);
    pasos--;
  }
}

void mover_enfoque()
{
  int pasos = 0;
  boolean dir= HIGH;
  switch (cont_filtro)
  {
    case 1:
      //mover_enfoque_continuo_abajo(80);  // 5 pasos 4,5º
      mover_enfoque_continuo_abajo(32);
      //pasos = 32; // 2.5 um por pulsación
      //dir = HIGH;
      break;
    case 2:
      //mover_enfoque_continuo_abajo(160); // 10 pasos 9º
      mover_enfoque_continuo_abajo(128);
      //pasos = 128; // 10um por pulsación
      //dir = HIGH;
      break;
    case 3:
      //mover_enfoque_continuo_arriba(80); // 5 pasos 4,5º
      mover_enfoque_continuo_arriba(32);
      //pasos = 32; // 2.5 um por pulsación
      //dir = LOW;
      break;
    case 4:
      //mover_enfoque_continuo_arriba(160); // 10 pasos 9º 
      mover_enfoque_continuo_arriba(128);
      //pasos = 128; // 10um por pulsación
      //dir = LOW;
      break;
  }
  
  while (pasos > 0)
  {
    digitalWrite(X_DIR_PIN, dir);
    digitalWrite(X_STEP_PIN, LOW);
    delayMicroseconds(stepDelay);
    digitalWrite(X_STEP_PIN, HIGH);
    delayMicroseconds(stepDelay);
    pasos--;
  }
Serial.print("1");  
}

