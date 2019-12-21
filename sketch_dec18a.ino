#include <Fuzzy.h>
int inMotorA1=2, inMotorA2=3;
int inMotorB1=4, inMotorB2=5;
long duracion,distancia;
float velocidadeMotor1=10;
float velocidadeMotor2=11;
#define Pecho 6
#define Ptrig 7

Fuzzy* fuzzy = new Fuzzy();

 FuzzySet* demcerca = new FuzzySet (0, 10, 13, 15);      //Conjunto difuso Distancia demasiado cerca
 FuzzySet* muycerca = new FuzzySet  (13, 30, 40, 50);    //Conjunto difuso Distancia muy cerca
 FuzzySet* cerca = new FuzzySet   (43, 62, 76, 90);     //Conjunto difuso Distancia cerca
 FuzzySet* lejos = new FuzzySet  (80, 120, 180, 360);     //Conjunto difuso Distancia promedio  
 

 FuzzySet* Alto = new FuzzySet(0, 0, 0, 0);                // Velocidad para detener     
 FuzzySet* Suave = new FuzzySet(5, 15, 25, 50);         // Velocidad suave
 FuzzySet* Media = new FuzzySet(45,60, 80, 100);    // Velocidad medio
 FuzzySet* Rapida = new FuzzySet(95, 120, 130, 200);  // Velocidad rápido 

void setup() {
  
  FuzzyInput* distance = new FuzzyInput(1);
  distance->addFuzzySet(demcerca); //Agrega fuzzyset demasiado cerca a distancia
  distance->addFuzzySet(muycerca); //Agrega fuzzyset muy cerca a distancia
  distance->addFuzzySet(cerca);    //Agrega fuzzyset cerca a distancia
  distance->addFuzzySet(lejos);    //Agrega fuzzyset lejos a distancia
  fuzzy->addFuzzyInput(distance);  //Agrega entrada difusa al objeto difuso


  //Crear composición de entrada difusa vel
  FuzzyOutput* vel = new FuzzyOutput(1);
  vel->addFuzzySet(Alto); // Agregar fuzzyset paro
  vel->addFuzzySet(Suave);// Agregar fuzzyset lento
  vel->addFuzzySet(Media);// Agregar fuzzyset medio
  vel->addFuzzySet(Rapida);//Agregar fuzzyset rapido
  fuzzy->addFuzzyOutput(vel);// Agrega entrada difusa al objeto difuso


  FuzzyRuleAntecedent* ifdistancedemcerca = new FuzzyRuleAntecedent(); // Inicializando antecedente de la expresión
  ifdistancedemcerca->joinSingle(demcerca); // Agregando FuzzySet correspondiente al antecedente
  FuzzyRuleConsequent* paro = new FuzzyRuleConsequent(); // Inicializando consecuencia de la expresión
  paro->addOutput(Alto); // Agregando FuzzySet correspondiente a la consecuencia
  // Instantiating a FuzzyRule object
  FuzzyRule* fuzzyRule01 = new FuzzyRule(1, ifdistancedemcerca, paro);  
  fuzzy->addFuzzyRule(fuzzyRule01); // Agrega regla difusa al objeto difuso.
  
  FuzzyRuleAntecedent* ifdistancemuycerca = new FuzzyRuleAntecedent(); // Inicializando antecedente de la expresión
  ifdistancemuycerca->joinSingle(muycerca); // Agregando FuzzySet correspondiente al antecedente
  FuzzyRuleConsequent* muydespacio = new FuzzyRuleConsequent(); // Inicializando consecuencia de la expresión
  muydespacio->addOutput(Suave); // Agregando FuzzySet correspondiente a la consecuencia
  // Instantiating a FuzzyRule object
  FuzzyRule* fuzzyRule02 = new FuzzyRule(2, ifdistancemuycerca, muydespacio);  
  fuzzy->addFuzzyRule(fuzzyRule02); // Agrega regla difusa al objeto difuso.

  FuzzyRuleAntecedent* ifdistancecerca = new FuzzyRuleAntecedent(); // Inicializando antecedente de la expresión
  ifdistancecerca->joinSingle(cerca); // Agregando FuzzySet correspondiente al antecedente
  FuzzyRuleConsequent* despacio = new FuzzyRuleConsequent(); // Inicializando consecuencia de la expresión
  despacio->addOutput(Media); // Agregando FuzzySet correspondiente a la consecuencia
  // Instantiating a FuzzyRule object
  FuzzyRule* fuzzyRule03 = new FuzzyRule(3, ifdistancecerca, despacio);  
  fuzzy->addFuzzyRule(fuzzyRule03); // Agrega regla difusa al objeto difuso


  FuzzyRuleAntecedent* ifdistancelejos= new FuzzyRuleAntecedent(); // Inicializando antecedente de la expresión
  ifdistancelejos->joinSingle(lejos); // Agregando FuzzySet correspondiente al antecedente
  FuzzyRuleConsequent* rapido = new FuzzyRuleConsequent(); // Inicializando consecuencia de la expresión
  rapido->addOutput(Rapida); // Agregando FuzzySet correspondiente a la consecuencia
  // Instantiating a FuzzyRule object
  FuzzyRule* fuzzyRule04 = new FuzzyRule(4, ifdistancelejos, rapido);  
  fuzzy->addFuzzyRule(fuzzyRule04); // Agrega regla difusa al objeto difuso

  pinMode (Pecho, INPUT);
  pinMode (Ptrig, OUTPUT);
  pinMode(inMotorA1,OUTPUT);
  pinMode(inMotorB1,OUTPUT);
  pinMode(inMotorA2,OUTPUT);
  pinMode(inMotorB2,OUTPUT);

  pinMode(velocidadeMotor1,OUTPUT);
  pinMode(velocidadeMotor2,OUTPUT);
  Serial.begin(9600);


}

void loop(){
  digitalWrite(Ptrig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ptrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Ptrig, LOW);
  duracion = pulseIn(Pecho, HIGH);
  distancia = (duracion/2)/29;
  
    fuzzy->setInput(1, distancia);
    
    fuzzy->fuzzify();
    
    float v = fuzzy->defuzzify(1);

    analogWrite(velocidadeMotor1,v);
    analogWrite(velocidadeMotor2,v);

    digitalWrite(inMotorA1,LOW);
    digitalWrite(inMotorA2,HIGH);
    digitalWrite(inMotorB1,HIGH);
    digitalWrite(inMotorB2,LOW);
    
    Serial.print("Distancia = ");
    Serial.print(distancia);
    Serial.print(" Velocidade = ");
    Serial.println(v);
    delay(100);
}
