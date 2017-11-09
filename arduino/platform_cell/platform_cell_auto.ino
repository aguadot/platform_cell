// Motor Y es el de los leds
// Motor X es el del enfoque del portamuestras (eje z del portamuestras)

#define Y_DIR_PIN 61 // poner número del pin de dirección
#define Y_STEP_PIN 60 // poner número de pin de conexión al motor
#define Y_ENABLE_PIN 56 // poner número de pin de poner en marcha 
#define FIN_CARRERA_1 3 // poner número de pin de final de carrera 1
#define FIN_CARRERA_2 2 // poner número de pin de final de carrera 2
#define FIN_CARRERA_3 14 // poner número de pin de final de carrera 3
#define X_DIR_PIN 54 // poner número del pin de dirección
#define X_STEP_PIN 55 // poner número de pin de conexión al motor
#define X_ENABLE_PIN 38 // poner número de pin de poner en marcha 
#define TRIGGER_FILTRO_LINEAL 16 // poner número de pin con el que leer el disparo de cada movimiento del filtro lineal


int stepDelay = 100; // tiempo de parada para controlar la velocidad

int num_trigger = 5; // poner el número de posiciones que tiene el filtro lineal para poder controlar cuando acaba el movimiento

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
		
	pinMode(TRIGGER_FILTRO_LINEAL, INPUT);
	//digitalWrite(TRIGGER_FILTRO_LINEAL,LOW);
	
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
if (primer_arranque == true) // cuando encendemos el programa nos vamos a la posicion del final de carrera nº1
	{
		posicion_1();
		primer_arranque = false;
		pos_final = true;
	}
		
	leer_serial();
	mover_led();
	cont_led = 0;
	cont_filtro = 0;
	if (pos_final == true)
	{
		Serial.print("1");
		pos_final = false;
	}
}

void comprobar_salto() // comprobamos si ha habido algún salto del filtro lineal y lo contabilizamos
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

void mover_led()
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
	}
}

void leer_serial() // nos carga los datos de la secuencia de movimiento y los saltos del filtro en sus variables
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

void motor_enfoque()
{
	// falta saber cada cuantos pasos necesitamos mover el enfoque y como controlar el punto de inicio
}