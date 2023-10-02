int pirPin = 2;  // Pino de entrada do sensor PIR
int ldrPin = A0; // Pino analógico ao qual o sensor de fotoresistor está conectado
int redLED = 10;
int yellowLED = 9;
int greenLED = 8;
int buzzerPin = 7; // Pino do buzzer
int servoPin1 = 5; // Pino do primeiro motor servo
int servoPin2 = 6; // Pino do segundo motor servo

Servo meuServo1; // Crie um objeto Servo para o primeiro servo
Servo meuServo2; // Crie um objeto Servo para o segundo servo

int notas[] = {262, 330, 392, 523, 0, 392, 523, 660};
int melodia[] = {0, 3, 2, 0, 4, 2, 3, 0};

void setup()
{
    pinMode(pirPin, INPUT);
    pinMode(ldrPin, INPUT);
    pinMode(redLED, OUTPUT);
    pinMode(yellowLED, OUTPUT);
    pinMode(greenLED, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    meuServo1.attach(servoPin1); // Anexar o primeiro servo ao pino
    meuServo2.attach(servoPin2); // Anexar o segundo servo ao pino
    meuServo1.write(90);         // Definir a posição inicial do primeiro servo (90 graus)
    meuServo2.write(90);         // Definir a posição inicial do segundo servo (90 graus)
    Serial.begin(9600);          // Inicialize a comunicação serial para depuração
}

void loop()
{
    int motionDetected = digitalRead(pirPin); // Verifique o sensor PIR
    int luminosidade = analogRead(ldrPin);    // Leia a luminosidade do sensor de fotoresistor

    int valorPotenciometro = analogRead(A1); // Leia o valor do potenciômetro

    if (motionDetected == HIGH && luminosidade < 500)
    {
        // Se o movimento for detectado e a luminosidade for baixa, acenda a luz vermelha, desligue a luz verde
        // e toque a melodia
        digitalWrite(redLED, HIGH);
        digitalWrite(yellowLED, HIGH);
        digitalWrite(yellowLED, LOW);
        digitalWrite(greenLED, LOW);
        tocarMelodia();

        // Balança o primeiro servo de um lado para o outro (aproximadamente 45 graus)
        for (int i = 0; i < 6; i++)
        {
            meuServo1.write(45);
            delay(500);
            meuServo1.write(135);
            delay(500);
        }

        meuServo1.write(90);                                       // Retorna o primeiro servo à posição inicial
        meuServo2.write(map(valorPotenciometro, 0, 1023, 0, 180)); // Atualize a posição do segundo servo com base no potenciômetro
        Serial.println("Movimento detectado!");
    }
    else
    {
        // Caso contrário, acenda a luz verde, desligue a luz vermelha
        // e pare o som
        digitalWrite(redLED, LOW);
        digitalWrite(yellowLED, HIGH);
        delay(200);
        digitalWrite(yellowLED, LOW);
        digitalWrite(greenLED, HIGH);
        delay(300);
        digitalWrite(greenLED, LOW);
        noTone(buzzerPin);                                         // Pare o som do buzzer
        meuServo1.write(90);                                       // Gire o primeiro servo de volta para a posição de 90 graus
        meuServo2.write(map(valorPotenciometro, 0, 1023, 0, 180)); // Atualize a posição do segundo servo com base no potenciômetro
        Serial.println("Nenhum movimento detectado ou luminosidade alta.");
    }

    delay(100); // Verifique a cada 0,1 segundos
}

void tocarMelodia()
{
    int duracao = 500;

    for (int i = 0; i < sizeof(melodia) / sizeof(melodia[0]); i++)
    {
        if (melodia[i] == -1)
        {
            noTone(buzzerPin);
        }
        else
        {
            tone(buzzerPin, notas[melodia[i]], duracao);
        }
        delay(duracao * 0.5);
    }
}
