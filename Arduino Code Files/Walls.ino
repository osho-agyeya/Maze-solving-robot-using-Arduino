int lmotorf = 10;
int lmotorr = 9;
int rmotorf = 11;
int rmotorr = 12;

int trigPin2 = 2;  //front sensor
int echoPin2 = 3;

int trigPin3 = 4;  //right sensor
int echoPin3 = 5;

int trigPin1 = 6;   //left sensor
int echoPin1 = 7;

boolean a=true;

void setup() {
  Serial.begin (9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  
  pinMode(lmotorf, OUTPUT);
  pinMode(lmotorr, OUTPUT);
  pinMode(rmotorf, OUTPUT);
  pinMode(rmotorr, OUTPUT);
  
}

int firstsensor(){ // This function is for left sensor
  int duration1, distance1;
  digitalWrite (trigPin1, HIGH);
  delayMicroseconds (10);
  digitalWrite (trigPin1, LOW);
  duration1 = pulseIn (echoPin1, HIGH);
  distance1 = (duration1/2) / 29.1;

      Serial.print("1st Sensor: ");
      Serial.print(distance1);  
      Serial.print("cm    ");
  return distance1;
  
}
int secondsensor(){ // This function is for front sensor.
    int duration2, distance2;
    digitalWrite (trigPin2, HIGH);
    delayMicroseconds (10);
    digitalWrite (trigPin2, LOW);
    duration2 = pulseIn (echoPin2, HIGH);
    distance2 = (duration2/2) / 29.1;
  
      Serial.print("2nd Sensor: "); 
      Serial.print(distance2);  
      Serial.print("cm    ");
   return distance2;
  
}
int thirdsensor(){ // This function is for right sensor.
    int duration3, distance3;
    digitalWrite (trigPin3, HIGH);
    delayMicroseconds (10);
    digitalWrite (trigPin3, LOW);
    duration3 = pulseIn (echoPin3, HIGH);
    distance3 = (duration3/2) / 29.1;

      Serial.print("3rd Sensor: ");   
      Serial.print(distance3);  
      Serial.print("cm     ");
   return distance3;
   
}

void loop() {


  if(a){
    delay(2000);
    a=false;
  }

  firstsensor();
  secondsensor();
  thirdsensor();
    
  Serial.println("\n");
  
  if (firstsensor()>30){              //left sensor
    turnLeft();
    goStraight();
    stop();
  }else{
    if (secondsensor()>30){               //front sensor
      goStraight();
      stop();
    }else{
      if(thirdsensor()>30){             //right sensor
        turnRight();
        goStraight();
        stop();
      }else{
        turnLeft();
        stop();
        turnLeft();
        stop();
        goStraight();
        stop();
      }
    }
  }

}

void turnLeft(){
    digitalWrite (lmotorf, LOW);
    digitalWrite (lmotorr, HIGH);
    digitalWrite (rmotorf, HIGH);
    digitalWrite (rmotorr, LOW);
    delay(161);
}

void goStraight(){
    digitalWrite (lmotorf, HIGH);
    digitalWrite (lmotorr, LOW);
    digitalWrite (rmotorf, HIGH);
    digitalWrite (rmotorr, LOW);
    delay(460);
}

void turnRight(){

    digitalWrite (lmotorf, HIGH);
    digitalWrite (lmotorr, LOW);
    digitalWrite (rmotorf, LOW);
    digitalWrite (rmotorr, HIGH);
    delay(161);
}

void stop() {
    digitalWrite (lmotorf, LOW);
    digitalWrite (lmotorr, LOW);
    digitalWrite (rmotorf, LOW);
    digitalWrite (rmotorr, LOW);
    delay(500);
}
