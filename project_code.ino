#include <SPI.h>
#include <LiquidCrystal.h>
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int DC = 9;
int CS = A2;
// MOSI --> (SDA, COPI) --> D11
int RST = A3;
// SCLK --> (SCK, CLOCK) --> D13
int servo = A5; //pwm pin

int JS_X = A0;
int JS_Y = A1;
int JS_BTN = 8;

bool up = false;
bool down = false;
bool left = false;
bool right = false;
bool pressed = false;

bool PRINT = true;

bool feed = false;
bool play = false;
bool train = false;
bool feedMenu = false;
bool playMenu = false;
bool trainMenu = false;

bool battle = false;
bool decided = false;
bool win = false;

int turns = 0;
int j = 0;

int hp = 50;
int atk = 20;
int energy = 100;
char rank[5] = {'D', 'C', 'B', 'A', 'S'};

bool fail = false;
bool rare = false;

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);

} task;

int delay_gcd;
const unsigned short tasksNum = 7;
task tasks[tasksNum];

int counter = 0;
enum SM1_SERVO{SM1_INIT, LEFT, RIGHT};
int SM1_Tick(int state1) {
if(battle){
  switch (state1) { // State transitions
    case SM1_INIT:
       if(win && decided){
        counter = 0;
        state1 = LEFT; 
       } else if(!win && decided){
        counter = 0;
        state1 = RIGHT; 
       } else {
        state1 = SM1_INIT; 
       }
        break;
    case LEFT:
        if(counter <= 150){
          state1 = LEFT;
        } else {
          counter = 0;
          state1 = SM1_INIT;
          battle = false;
          turns++;
          j++;
          decided = false;
          PRINT = true;  
        }
        break;
    case RIGHT:
        if(counter <= 150){
          state1 = RIGHT;
        } else {
          counter = 0;
          state1 = SM1_INIT;
          battle = false;
          turns++;
          decided = false;
          PRINT = true;    
        }
        break;
  }
  switch (state1) { // State Action
    case SM1_INIT:
      moveCenter();
      Serial.println("Ar center");
      break;
    case LEFT: 
      turnLeft();
      counter++;
      Serial.println("Turning left");
      break;
    case RIGHT:
      turnRight();
      Serial.println("Turning right");
      counter++;
      break;
  }
} 
  return state1;
}


enum SM2_BATTLE { SM2_INIT, FATE};
int SM2_Tick(int state2) {
if(battle){
  switch (state2) { // State transitions
    case SM2_INIT:
          state2 = SM2_INIT;
  }
  switch (state2) { // State Action
    case SM2_INIT:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("BATTLING");
    if(!decided){
    int num = abs(randomInt() % 100);
    num = abs(num);
    Serial.println("I'm deciding");
    if(j == 0){
      if(hp > 100){
          win = true;
          Serial.println("I'm winning");
      } else {
          win = false;
          Serial.println("I'm losing"); 
      }
    }

    if(j == 1){
       if(hp > 150 && atk > 40){
          win = true; 
       } else if((hp < 150 || atk < 40) && num < 90){
          win = true;
       } else {
          win = false; 
       }
    }

    if(j == 2){
       if(hp > 250 && atk > 50){
          win = true; 
       } else if((hp < 250 || atk < 50) && num < 70){
          win = true;
       } else {
          win = false; 
       }
    }

    if(j == 3){
       if(hp > 300 && atk > 60){
          win = true; 
       } else if((hp < 300 || atk < 60) && num < 80){
          win = true;
       } else {
          win = false; 
       }
    }

    if(j == 4){
       if(hp > 400 && atk > 70){
          win = true; 
       } else if((hp < 400 || atk < 70) && num < 80){
          win = true;
       } else {
          win = false; 
       }
    }

    decided = true;
    }
      break;
  } 
}
  return state2;
}

enum SM3_ACTIONS { SM3_INIT, FEEDING, PLAYING, TRAINING};
int SM3_Tick(int state3) {
if(!battle){
  switch (state3) { // State transitions
    case SM3_INIT:
        state3 = SM3_INIT; 
     break;
  }
  switch (state3) { // State Action
    case SM3_INIT:
      if(pressed){ 
        if(feed){
          feedAction();
          feed = false;
        } else if(play){
          playAction();
          play = false;
        } else if(train){
          trainAction();
          train = false;
        }
      }
      break;
  }
} 
  return state3;
}

enum SM4_TFT { SM4_INIT};
int SM4_Tick(int state4) {
  switch (state4) { // State transitions
    case SM4_INIT:
        state4 = SM4_INIT;
      break;
  }
  switch (state4) { // State Action
    case SM4_INIT:
      if(PRINT){
          if(j <= 1){
            printSprite();
            PRINT = false;
          }

          if(j <= 3 && j >=2){
            printSprite2();
            PRINT = false;  
          }

          if(j == 4){
            printSprite2();
            PRINT = false;  
          }
      }
      break;
  } 
  return state4;
}

enum SM5_JOYSTICK { SM5_INIT};
int SM5_Tick(int state5) {
  int j_x = analogRead(JS_X);
  int j_y = analogRead(JS_Y);
      //Serial.print("X: "); Serial.println(j_x);
      //Serial.print(" Y: " ); Serial.println(j_y);
  switch (state5) { // State transitions
    case SM5_INIT:
      state5 = SM5_INIT;
      break;
  }
  switch (state5) { // State Action
    case SM5_INIT:
      if(j_x < 300){ // up
        down = false;
        up = true;
        right = false;
        left = false;
      } else if(j_x > 900){ // down
        down = true;
        up = false;
        right = false;
        left = false;
      } else if(j_y > 900){ // left
        down = false;
        up = false;
        right = false;
        left = true;
      } else if(j_y < 300){ // right
         down = false;
         up = false;
         right = true;
         left = false;
      } else {
          down = false;
         up = false;
         right = false;
         left = false;
      }
      break;
  } 
  return state5;
}

enum SM6_LCD { SM6_INIT, PLAY, TRAIN, STATS, RESULT, SHOW};
int SM6_Tick(int state6) {
  //Serial.print("Turn: ");
  //Serial.println(turns);
if(!battle){
  switch (state6) { // State transitions
    case SM6_INIT:
      if(right){
        state6 = PLAY;
        feedMenu = false;
      } else if(down){
        state6 = TRAIN;
        feedMenu = false;
      } else {
        state6 = SM6_INIT;
      }

      if(pressed){
        feed = true;
        state6 = RESULT;  
      }
      break;
    case PLAY:
      if(left){
        state6 = SM6_INIT;
        playMenu = false;
      } else if(down){
        state6 = STATS;
        playMenu = false;
      } else {
        state6 = PLAY;  
      }

      if(pressed){
        play = true;
        state6 = RESULT;  
      }
      break;
    case TRAIN:
      if(up){
        state6 = SM6_INIT;
        trainMenu = false;
      } else if(right){
        state6 = STATS;
        trainMenu = false;
      } else {
        state6 = TRAIN;  
      }

      if(pressed){
        train = true;
        state6 = RESULT;  
      }
      break;
    case STATS:
      if(up){
        state6 = PLAY;
      } else if(left){
        state6 = TRAIN;  
      } else {
        state6 = STATS;  
      }

      if(pressed){
        state6 = SHOW;  
      }
      break;
    case RESULT:
      if(pressed){
        state6 = RESULT;  
      } else {
        feedMenu = false;
        playMenu = false;
        trainMenu = false;
        rare = false;
        fail = false;
        turns++;
        if(turns % 5 == 0){
          battle = true;  
        }
        state6 = SM6_INIT;
      }
      break;
    case SHOW:
      if(pressed){
        state6 = SHOW;  
      } else {
        state6 = STATS;
      }
      break;
  }
  switch (state6) { // State Action
    case SM6_INIT:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("->FEED   PLAY  ");
      lcd.setCursor(0,1);
      lcd.print("  TRAIN  STATS  ");
      feedMenu = true;
      break;
    case PLAY:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  FEED ->PLAY  ");
      lcd.setCursor(0,1);
      lcd.print("  TRAIN  STATS  ");
      playMenu = true;
      break;
    case TRAIN:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  FEED   PLAY  ");
      lcd.setCursor(0,1);
      lcd.print("->TRAIN  STATS  ");
      trainMenu = true;
      break;
    case STATS: 
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  FEED   PLAY  ");
      lcd.setCursor(0,1);
      lcd.print("  TRAIN->STATS  ");
      break;
    case RESULT:
      if(trainMenu){
          if(rare){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("BIG SUCCESS!!");
            lcd.setCursor(0,1);
            lcd.print("+30HP and +10ATK");
          } else if(fail){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("FAILED....");
            lcd.setCursor(0,1);
            lcd.print("+10HP only");
          } else {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("SUCCESS!!");
            lcd.setCursor(0,1);
            lcd.print("+20HP and +5ATK");
          }
      }

      if(feedMenu){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Gained 50 energy");  
      }

      
      if(playMenu){
        if(rare){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("+10 energy");
          lcd.setCursor(0,1);
          lcd.print("+5HP/ATK");
        } else {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("+10 energy");
          lcd.setCursor(0,1);
          lcd.print("+5HP/ATK"); 
        }
      }

      
      break;
    case SHOW:
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("HP:");
            lcd.print(hp);
            lcd.print(" ATK:");
            lcd.print(atk);
            lcd.setCursor(0,1);
            lcd.print("ENERGY:");
            lcd.print(energy);
            lcd.print("Rank:");
            lcd.print(rank[j]);
     break;
  }
}
  return state6;
}

enum SM7_SWITCH { OFFR, ONP, ONR, OFFP};
int SM7_Tick(int state7) {
int j_btn = digitalRead(JS_BTN);
  switch (state7) { // State transitions
    case OFFR:
      if(j_btn == 0){
        pressed = true;
        state7 = ONP;
      } else {
        state7 = OFFR;  
      }
      break;
    case ONP:
      if(j_btn == 1){
        state7 = ONR;
      } else {
        state7 = ONP;  
      }
      break;
    case ONR:
      if(j_btn == 0){
        pressed = false;
        state7 = OFFP;
      } else {
        state7 = ONR;  
      }
      break;
    case OFFP:
      if(j_btn == 1){
        state7 = OFFR;
      } else {
        state7 = OFFP;  
      }
      break;
  }
  switch (state7) { // State Action
    case OFFR:
      break;
    case ONP:
      break;
    case ONR:
      break;
    case OFFP:
      break;
  }
  return state7;
}

  
void setup() {
  // put your setup code here, to run once:
  //InitDisplay();
  Serial.begin(9600);
  pinMode(servo, OUTPUT);
  pinMode(DC, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(JS_BTN, INPUT_PULLUP);
  SPI.begin();
  lcd.begin(16,2);
  initialize();

  unsigned char i = 0;
  
  tasks[i].state = SM1_INIT;
  tasks[i].period = 20;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
  i++;
  tasks[i].state = SM2_INIT;
  tasks[i].period = 20;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;
  i++;
  tasks[i].state = SM3_INIT;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM3_Tick;
  i++;
  tasks[i].state = SM4_INIT;
  tasks[i].period = 1000;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM4_Tick;
  i++;
  tasks[i].state = SM5_INIT;
  tasks[i].period = 300;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM5_Tick;
  i++;
  tasks[i].state = SM6_INIT;
  tasks[i].period = 5;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM6_Tick;
  i++;
  tasks[i].state = OFFR;
  tasks[i].period = 300;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM7_Tick;

  delay_gcd = 5;
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
      tasks[i].state = tasks[i].TickFct(tasks[i].state);
      tasks[i].elapsedTime = millis(); // Last time this task was ran
    }
  }

  
  //int testNum = randomInt() % 100;
  //Serial.println(abs(testNum));
  /*
  turnRight();

  delay(300);

  moveCenter();

  delay(300);

  turnLeft();

  delay(300);

  moveCenter();

  delay(300);
  */

  //printSprite();
  
  
}
