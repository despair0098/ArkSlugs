void trainAction(){
    int num = randomInt() % 100;
    num = abs(num);
    energy = energy - 20;
    if(energy < 0){
      energy = 0;  
    }
    if(energy < 50 && num < 30){
      fail = true;
      hp += 10;
    } else if(energy > 50 && num < 10){
      rare = true;
      hp += 30;
      atk += 10;  
    } else {
      hp += 20;
      atk += 5;  
    }   
}

void playAction(){
  int num = abs(randomInt() % 100);
  num = abs(num);
  hp += 5;
  atk += 5;  
  if(num < 20){
    energy += 10;
    rare = true;  
  } else {
    energy += 5;  
  }

  if(energy > 100){
    energy = 100;  
  }  
}

void feedAction(){
  energy += 50;
  if(energy > 100){
    energy = 100;  
  }  
}
