void setup() {
  // put your setup code here, to run once:
  
  
  
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //arrLng is total number of seconds minus 1 that you want to use for average out
  int arrLng = 5;
  
  double tmpArr[arrLng];
  double sum = 0;
  double avg = 0;
  double b = 0;

   
    Serial.print("For loop Start: ");
  Serial.println("");  
  
  
  for(int d = 0; d < arrLng; d++){

    
    tmpArr[d] = b; 
    
    Serial.print("b = ");
    Serial.print(b);
    Serial.println("");
    b++;
    delay(1000);   
  
  }

  for(int a = 0; a < arrLng; a++){
      sum += tmpArr[a];
      }

      avg = sum / (arrLng);

    Serial.print("Average: ");
      Serial.print(avg);
      Serial.println("");

    
}
