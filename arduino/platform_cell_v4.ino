

#define Y_DIR_PIN 61 // poner número del pin de dirección
#define Y_STEP_PIN 60 // poner número de pin de conexión al motor
#define Y_ENABLE_PIN 56 // poner número de pin de poner en marcha 
#define FIN_CARRERA_1 3 // poner número de pin de final de carrera 1
#define FIN_CARRERA_2 2 // poner número de pin de final de carrera 2
#define FIN_CARRERA_3 14 // poner número de pin de final de carrera 3

const int steps = 200;

int stepDelay; // tiempo de parada para controlar la velocidad

boolean pos_1 = false; // cuando pongamos un reset de inicio la pondremos a false
boolean pos_2 = false;
boolean pos_3 = false;

boolean primer_arranque = true; // para controlar cada vez que se encienda ir al home

int final_carrera_1 = 1;
int final_carrera_2 = 1;
int final_carrera_3 = 1;

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
	
	Serial.begin(9600);

}

void loop() 
{
	if (primer_arranque == true) // cuando encendemos el programa nos vamos a la posicion del final de carrera nº1
	{
		final_carrera_1 = digitalRead(FIN_CARRERA_1);
		while (final_carrera_1 == 1) // vamos a ir hasta la posicion del led 1
		{
			digitalWrite(Y_DIR_PIN, HIGH);
			digitalWrite(Y_STEP_PIN, LOW);
			delay(1);
			digitalWrite(Y_STEP_PIN, HIGH);
			final_carrera_1 = digitalRead(FIN_CARRERA_1);
		}
		primer_arranque = false;
		pos_1 = true;
	}

	if (Serial.available()>0)
	{
		int option = Serial.read();
		if (option == '1') // poner el led 1 en posición
		{
			if ((pos_1 == false) && (pos_2 == true)) // retroceder hasta pos 1 desde pos 2
			{
				for (int x=0; x<6400; x++)
				{
					final_carrera_1 = digitalRead(FIN_CARRERA_1); // comprobamos que no llegamos 
					if (final_carrera_1 == 1)
					{
						digitalWrite(Y_DIR_PIN, HIGH);
						digitalWrite(Y_STEP_PIN, LOW);
						delay(1);
						digitalWrite(Y_STEP_PIN, HIGH);
					}
					else break;
				}
			}
			else if ((pos_1 == false) && (pos_3 == true)) // retroceder hasta la pos 1 desde pos 3
			{
				for (int x=0; x<12800; x++)
				{
					final_carrera_1 = digitalRead(FIN_CARRERA_1);
					if (final_carrera_1 == 1)
					{
						digitalWrite(Y_DIR_PIN, HIGH);
						digitalWrite(Y_STEP_PIN, LOW);
						delay(1);
						digitalWrite(Y_STEP_PIN, HIGH);
					}
					else break;
				}
			}
			// Si no hemos llegado al final de carrera nos vamos hasta él
			final_carrera_1 = digitalRead(FIN_CARRERA_1);
			while ((final_carrera_1 == 1) && (pos_1 == false))
			{
				digitalWrite(Y_DIR_PIN, HIGH);
				digitalWrite(Y_STEP_PIN, LOW);
				delay(1);
				digitalWrite(Y_STEP_PIN, HIGH);
				final_carrera_1 = digitalRead(FIN_CARRERA_1);
			}
			final_carrera_1 = digitalRead(FIN_CARRERA_1);
			if (final_carrera_1 == 0)
			{
				pos_2 = false;
				pos_3 = false;
				pos_1 = true;
				Serial.println("ok_1");
			}
			else
			{
				Serial.println("error_1");
			}
		}
		
		else if (option == '2') // poner el led 2 en posición
		{
			if ((pos_2 == false) && (pos_1 == true)) // avanzar hasta pos 2 porque está en la pos 1
			{	
				for (int x=0; x<6400; x++)
				{
					final_carrera_2 = digitalRead(FIN_CARRERA_2);
					if (final_carrera_2 == 1)
					{
						digitalWrite(Y_DIR_PIN, LOW);
						digitalWrite(Y_STEP_PIN, LOW);
						delay(1);
						digitalWrite(Y_STEP_PIN, HIGH);
					}
					else break;
				}				
			}
			else if ((pos_2== false) && (pos_3 == true)) // retroceder hasta pos 2 porque está en la pos 3
			{	
				for (int x=0; x<6400; x++)
				{
					final_carrera_2 = digitalRead(FIN_CARRERA_2);
					if (final_carrera_2 == 1)
					{
						digitalWrite(Y_DIR_PIN, HIGH);
						digitalWrite(Y_STEP_PIN, LOW);
						delay(1);
						digitalWrite(Y_STEP_PIN, HIGH);
					}
					else break;
				}
			}
			// Si no hemos llegado al final de carrera nos vamos hasta él
			final_carrera_2 = digitalRead(FIN_CARRERA_2);
			while ((final_carrera_2 == 1) && (pos_2 == false))
			{
				digitalWrite(Y_DIR_PIN, HIGH);
				digitalWrite(Y_STEP_PIN, LOW);
				delay(1);
				digitalWrite(Y_STEP_PIN, HIGH);
				final_carrera_2 = digitalRead(FIN_CARRERA_2);
			}
			final_carrera_2 = digitalRead(FIN_CARRERA_2);
			if (final_carrera_2 == 0) 
			{	
				pos_1 = false;
				pos_3 = false;
				pos_2 = true;
				Serial.println("ok_2");
			}
			else
			{
				Serial.println("error_2");
			}
		}
		else if (option == '3') // poner el led 3 en posición
		{
			if ((pos_3 == false) && (pos_1 == true))// avanzar hasta pos 3 desde pos 1
			{
				for (int x=0; x<12800; x++)
				{
					final_carrera_3 = digitalRead(FIN_CARRERA_3);
					if (final_carrera_3 == 1)
					{
						digitalWrite(Y_DIR_PIN, LOW);
						digitalWrite(Y_STEP_PIN, LOW);
						delay(1);
						digitalWrite(Y_STEP_PIN, HIGH);
					}
					else break;
				}
			}
			else if ((pos_3 == false) && (pos_2 == true)) // avanzar hasta pos 3 desde pos 2
			{
				for (int x=0; x<6400; x++)
				{
					final_carrera_3 = digitalRead(FIN_CARRERA_3);
					if (final_carrera_3 ==1)
					{
						digitalWrite(Y_DIR_PIN, LOW);
						digitalWrite(Y_STEP_PIN, LOW);
						delay(1);
						digitalWrite(Y_STEP_PIN, HIGH);
					}
					else break;
				}	
			}
			// Si no hemos llegado al final de carrera nos vamos hasta él
			final_carrera_3 = digitalRead(FIN_CARRERA_3);
			while ((final_carrera_3 == 1) && (pos_3 == false))
			{
				digitalWrite(Y_DIR_PIN, HIGH);
				digitalWrite(Y_STEP_PIN, LOW);
				delay(1);
				digitalWrite(Y_STEP_PIN, HIGH);
				final_carrera_3 = digitalRead(FIN_CARRERA_2);
			}
			final_carrera_3 = digitalRead(FIN_CARRERA_3);
			if (final_carrera_3 == 0)
			{
				pos_1 = false;
				pos_2 = false;
				pos_3 = true;
				Serial.println("ok_3");
			}
			else
			{
				Serial.println("error_3");
			}
	 	}
	}
}
	