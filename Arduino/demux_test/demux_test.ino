void setup() {
  // put your setup code here, to run once:
  #define S0 3
  #define S1 4
  #define S2 5
  #define S3 6

  #define SIG 7

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  pinMode(SIG, OUTPUT);

  digitalWrite(SIG,LOW);
    delay(1000);

}

void toggle_light(){
  delay(1000);
  digitalWrite(SIG, LOW);
  delay(1000);
}

void toggle_addr(int addr){
  digitalWrite(S0, bitRead(addr,0));
  digitalWrite(S1, bitRead(addr,1));
  digitalWrite(S2, bitRead(addr,2));
  digitalWrite(S3, bitRead(addr,3));
}


void loop() {

  //


  digitalWrite(SIG, HIGH);

  for(int i =0; i<1;i++){
    toggle_addr(i);
    delay(10);
    

  }

  //toggle_addr(0);



 // toggle_light();



}
