// Custom servo motor contorl function
void turnRight(){
      digitalWrite(servo,HIGH);
      delayMicroseconds(2600);
      digitalWrite(servo, LOW);             
}

void turnLeft(){
       digitalWrite(servo,HIGH);
       delayMicroseconds(500);
       digitalWrite(servo, LOW);
}

void moveCenter(){
    digitalWrite(servo,HIGH);
    delayMicroseconds(1610);
    digitalWrite(servo, LOW);
}
