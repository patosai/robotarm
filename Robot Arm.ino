int CLAW_1 = 0;
int TELESCOPE_1 = 1;
int ELBOW_1 = 2;
int LAZY_SUSAN_1 = 3;

int CLAW_2 =  4;
int TELESCOPE_2 = 5;
int ELBOW_2 = 6;
int LAZY_SUSAN_2 = 7;

int HJOYIDLE = 524;
int VJOYIDLE = 514;

int transistor;
int DPDT;
int SPST;
int DP3T;
int POT;
int ENCODER;
int JOYSTICK;
int JOYIDLE;
int JPOWER;
int difference;
int range;
int variable_power;
int Negate_Pick_Process;
int BREAK_LAZY_SUSAN_2;
int BREAK_ELBOW_2;
int BREAK_TELESCOPE_2;
int INITIAL_LAZY_SUSAN_POSITION;
int INITIAL_ELBOW_POSITION;

int reset;
int reset2;
int last_count;

int i;

int count;
boolean last = LOW;

int CLOSE = 0;
int OPEN = 1;

int ENCODER_1 = 36;
int ENCODER_2 = 37;
int ELBOW_1_POT = 10;
int ELBOW_2_POT = 11;
int LAZY_SUSAN_1_POT = 12;
int LAZY_SUSAN_2_POT = 13;

int Stop_Auton_Mode = 0;

void Assign_Pins(int MOTOR)
{
  transistor = (11 - MOTOR);
  DPDT = (38 + (MOTOR * 2));
  SPST = (39 + (MOTOR * 2));
  DP3T = MOTOR;
  if(MOTOR == ELBOW_1)
  {POT = 10;}
  if(MOTOR == ELBOW_2)
  {POT = 11;}
  if(MOTOR == LAZY_SUSAN_1)
  {POT = 12;}
  if(MOTOR == LAZY_SUSAN_2)
  {POT = 13;}
  if(DP3T > 2)
  {DP3T = (DP3T + 1);}
  
}

void Stop_Auton()
{
  if(analogRead(6) > 300)
  {Stop_Auton_Mode = 1;}
}
  
void Control_Joystick(int MOTOR, int base, int limit)
{
  Assign_Pins(MOTOR);
  if(MOTOR == LAZY_SUSAN_1) {JOYSTICK = 2; JOYIDLE = 524;}
  if(MOTOR == ELBOW_1) {JOYSTICK = 3; JOYIDLE = 515;}
  if(MOTOR == LAZY_SUSAN_2) {JOYSTICK = 7; JOYIDLE = 502;}
  if(MOTOR == ELBOW_2) {JOYSTICK = 8; JOYIDLE = 520;}
  if(analogRead(JOYSTICK) > JOYIDLE + 10)
  {
    difference = analogRead(JOYSTICK) - JOYIDLE;
    range = (1023 - JOYIDLE);
    variable_power = limit - base;
    JPOWER = base + (variable_power * (difference / range));
    digitalWrite(SPST, LOW);
    digitalWrite(DPDT, LOW);
    analogWrite(transistor, JPOWER);
  }
  else if(analogRead(JOYSTICK) < JOYIDLE - 10)
  {
    difference = JOYIDLE - analogRead(JOYSTICK);
    range = JOYIDLE;
    variable_power = limit - base;
    JPOWER = base + (variable_power * (difference / range));
    digitalWrite(SPST, LOW);
    digitalWrite(DPDT, HIGH);
    analogWrite(transistor, JPOWER);
  }
  else
  {
    digitalWrite(SPST, HIGH);
    analogWrite(transistor, 0);
  }
}

void Control_DP3T(int MOTOR, int SPEED)
{
  Assign_Pins(MOTOR);
  if(analogRead(DP3T) > 700)
  {
    digitalWrite(SPST, LOW);
    digitalWrite(DPDT, LOW);
    analogWrite(transistor, SPEED);
  }
  else if(analogRead(DP3T) > 300)
  {
    digitalWrite(SPST, LOW);
    digitalWrite(DPDT, HIGH);
    analogWrite(transistor, SPEED);
  }
  else
  {
    digitalWrite(SPST, HIGH);
    analogWrite(transistor, 0);
  }
}

void Move_Pot(int MOTOR, int POSITION, int SPEED)
{
  Assign_Pins(MOTOR);
  if(analogRead(POT) < POSITION)
  {
    while(analogRead(POT) < POSITION)
    {
      Stop_Auton();
      if(Stop_Auton_Mode == 1)
      {break;}
      digitalWrite(SPST, LOW);
      digitalWrite(DPDT, LOW);
      analogWrite(transistor, SPEED);
    }
  }
  else
  {
    while(analogRead(POT) > POSITION)
    {
      Stop_Auton();
      if(Stop_Auton_Mode == 1)
      {break;}
      digitalWrite(SPST, LOW);
      digitalWrite(DPDT, HIGH);
      analogWrite(transistor, SPEED);
    }
  }
  digitalWrite(SPST, HIGH);
  analogWrite(transistor, 0);
}  

void Move_Telescope(int ARM, int TEETH, int SPEED)
{
  if(ARM == 1)
  {
    Assign_Pins(TELESCOPE_1);
    ENCODER = ENCODER_1;
  }
  if(ARM == 2)
  {
    Assign_Pins(TELESCOPE_2);
    ENCODER = ENCODER_2;
  }
  count = 0;
  while(count < abs(TEETH))
  {
    Stop_Auton();
    if(Stop_Auton_Mode == 1)
    {break;}
    if(digitalRead(ENCODER) != last)
    {count = count + 1;}
    last = digitalRead(ENCODER);
    delay(5);
    digitalWrite(SPST, LOW);
    if(TEETH < 0)
    {digitalWrite(DPDT, LOW);}
    else
    {digitalWrite(DPDT, HIGH);}
    analogWrite(transistor, SPEED);
  }
  analogWrite(transistor, 0);
  digitalWrite(SPST, HIGH);
}

void Negate_Pick(int ARM)
{
  if(ARM == 1)
  {Assign_Pins(CLAW_1);}
  if(ARM == 2)
  {Assign_Pins(CLAW_2);}
  digitalWrite(SPST, LOW);
  digitalWrite(DPDT, LOW);
  analogWrite(transistor, 200);
  if(ARM == 1)
  {Assign_Pins(TELESCOPE_1);}
  if(ARM == 2)
  {Assign_Pins(TELESCOPE_2);}
  digitalWrite(SPST, LOW);
  digitalWrite(DPDT, HIGH);
  analogWrite(transistor, 130);
  
  delay(300);
  
  digitalWrite(SPST, HIGH);
  analogWrite(transistor, 0);
  
  delay(300);
  
  if(ARM == 1)
  {Assign_Pins(CLAW_1);}
  if(ARM == 2)
  {Assign_Pins(CLAW_2);}
  digitalWrite(SPST, HIGH);
  analogWrite(transistor, 0);
}
  
void Edmond(int claw)
{
  Assign_Pins(claw);
  digitalWrite(SPST, LOW);
  digitalWrite(DPDT, LOW);
  analogWrite(transistor, 250);
}
  
void Move_Claw_2(int GRIP)
{
  Stop_Auton();
  Assign_Pins(CLAW_2);
  if(GRIP == 0 && Stop_Auton_Mode == 0)  
  {
    digitalWrite(SPST, LOW);
    digitalWrite(DPDT, LOW);
    analogWrite(transistor, 255);
    delay(1000);
  }
  if(GRIP == 1 && Stop_Auton_Mode == 0)
  {
    digitalWrite(SPST, LOW);
    digitalWrite(DPDT, HIGH);
    analogWrite(transistor, 255);
    delay(1000);
  }
  digitalWrite(SPST, HIGH);
  analogWrite(transistor, 0);
}

void Robot_Arm_2(int LSPOSITION, int LSSPEED, int EPOSITION, int ESPEED, int TTEETH, int TSPEED, int CGRIP)
{
  BREAK_LAZY_SUSAN_2 = 0;
  BREAK_ELBOW_2 = 0;
  BREAK_TELESCOPE_2 = 0;
  INITIAL_LAZY_SUSAN_POSITION = analogRead(LAZY_SUSAN_2_POT);
  INITIAL_ELBOW_POSITION = analogRead(ELBOW_2_POT);
  count = 0;
  while(0 == 0) 
  {
    Stop_Auton();
    if(Stop_Auton_Mode == 1)
    {break;}
    if(BREAK_LAZY_SUSAN_2 != 0 && BREAK_ELBOW_2 != 0 && BREAK_TELESCOPE_2 != 0)
    {break;}
    if(LSPOSITION == -1)
    {BREAK_LAZY_SUSAN_2 = 1;}
    if(EPOSITION == -1)
    {BREAK_ELBOW_2 = 1;}
    Assign_Pins(LAZY_SUSAN_2);
    if(LSPOSITION == analogRead(LAZY_SUSAN_2_POT))
    {BREAK_LAZY_SUSAN_2 = 1;}
 /*   if(INITIAL_LAZY_SUSAN_POSITION < LSPOSITION && analogRead(LAZY_SUSAN_2_POT) > LSPOSITION)
    {BREAK_LAZY_SUSAN_2 = 1;}
    if(INITIAL_LAZY_SUSAN_POSITION > LSPOSITION && analogRead(LAZY_SUSAN_2_POT) < LSPOSITION)
    {BREAK_LAZY_SUSAN_2 = 1;} */
    if(BREAK_LAZY_SUSAN_2 == 0)
    {
      if(analogRead(LAZY_SUSAN_2_POT) < LSPOSITION)
      {
        digitalWrite(DPDT, LOW);
        digitalWrite(SPST, LOW);
        analogWrite(transistor, LSSPEED);
      }
      else
      {
        digitalWrite(DPDT, HIGH);
        digitalWrite(SPST, LOW);
        analogWrite(transistor, LSSPEED);
      }
    }
    else
    {
      analogWrite(transistor, 0);
      digitalWrite(SPST, HIGH);
    }
    Assign_Pins(ELBOW_2);
    
    if(EPOSITION == analogRead(ELBOW_2_POT))
    {BREAK_ELBOW_2 = 1;}
  /*  if(INITIAL_ELBOW_POSITION < EPOSITION && analogRead(ELBOW_2_POT) > EPOSITION)
    {BREAK_ELBOW_2 = 1;}
    if(INITIAL_ELBOW_POSITION > EPOSITION && analogRead(ELBOW_2_POT) < EPOSITION)
    {BREAK_ELBOW_2 = 1;} */
    if(BREAK_ELBOW_2 == 0)
    {
      if(analogRead(ELBOW_2_POT) < EPOSITION)
      {
        digitalWrite(DPDT, HIGH);
        digitalWrite(SPST, LOW);
        analogWrite(transistor, ESPEED);
      }
      else
      {
        digitalWrite(DPDT, LOW);
        digitalWrite(SPST, LOW);
        analogWrite(transistor, ESPEED);
      }
    }
    else
    {
      analogWrite(transistor, 0);
      digitalWrite(SPST, HIGH);
    }
    Assign_Pins(TELESCOPE_2);
    if(count > abs(TTEETH))
    {BREAK_TELESCOPE_2 = 1;}
    if(BREAK_TELESCOPE_2 == 0)
    {
      if(digitalRead(ENCODER_2) != last)
      {count = count + 1;}
      last = digitalRead(ENCODER_2);
      delay(10);
      digitalWrite(SPST, LOW);
      if(TTEETH < 0)
      {digitalWrite(DPDT, LOW);}
      else
      {digitalWrite(DPDT, HIGH);}
      analogWrite(transistor, TSPEED);
    }
    else
    {
      analogWrite(transistor, 0);
      digitalWrite(SPST, HIGH);
    }
    Assign_Pins(CLAW_2);
    if(CGRIP == 1)
    {
      digitalWrite(SPST, LOW);
      analogWrite(transistor, 110);
      digitalWrite(DPDT, LOW);
    }
  }
  Assign_Pins(LAZY_SUSAN_2);
  analogWrite(transistor, 0);
  digitalWrite(SPST, HIGH);
  Assign_Pins(ELBOW_2);
  analogWrite(transistor, 0);
  digitalWrite(SPST, HIGH);
  Assign_Pins(TELESCOPE_2);
  analogWrite(transistor, 0);
  digitalWrite(SPST, HIGH);
  Assign_Pins(CLAW_2);
  analogWrite(transistor, 0);
  digitalWrite(SPST, HIGH);
}
    
    
void Reset_Telescope_2()
{
  Assign_Pins(TELESCOPE_2);
  reset = 0;
  reset2 = 0;
  count = 0;
  while(0 == 0)
  {
    Stop_Auton();
    if(Stop_Auton_Mode == 1)
    {break;}
    if(reset2 == 0)
    {
      last_count = count;
      reset2 = 1;
    }
    if(digitalRead(ENCODER_2) != last)
    {count = count + 1;}
    last = digitalRead(ENCODER_2);
    delay(5);
    reset = reset + 1;
    digitalWrite(SPST, LOW);
    digitalWrite(DPDT, HIGH);
    analogWrite(transistor, 180);
    if(reset > 20)
    {
      if(last_count == count)
      {break;}
      reset = 0;
      reset2 = 0;
    }
  }
  analogWrite(transistor, 0);
  digitalWrite(SPST, HIGH);
}
 
 
void Score_Two_Southmost_Pencils()
{
  Move_Claw_2(OPEN);
  Robot_Arm_2(910, 200, 840, 200, -23, 180, 0);
  Robot_Arm_2(-1, 200, -1, 200, -15, 180, 0);
  Negate_Pick(1);
  delay(50);
  Robot_Arm_2(-1, 160, 800, 180, 20, 180, 1);
  Robot_Arm_2(490, 130, 570, 200, -55, 180, 1);
  Robot_Arm_2(-1, 130, 620, 100, 0, 180, 1);
  delay(2000);
  Move_Claw_2(OPEN);
}
    
void setup()
{
  Stop_Auton_Mode = 0;
  for(i = 4; i < 13; i++)
  {pinMode(i, OUTPUT);}
  for(i = 34; i < 38; i++)
  {pinMode(i, INPUT);}
  for(i = 38; i < 54; i++)
  {pinMode(i, OUTPUT);}
}

void Manual_Arm_1()
{
  if(digitalRead(34) == LOW)
  {
    if(analogRead(4) > 800 && analogRead(4) < 900)
    {Edmond(CLAW_1);}
    else
    {Control_DP3T(CLAW_1, 255);}
    Control_DP3T(TELESCOPE_1, 255);
    Control_Joystick(ELBOW_1, 140, 255);
    Control_Joystick(LAZY_SUSAN_1, 100, 255);
  }
  else
  {
    if(analogRead(4) > 800 && analogRead(4) < 900)
    {Edmond(CLAW_1);} 
    else
    {Control_DP3T(CLAW_1, 200);}
    Control_DP3T(TELESCOPE_1, 140);
    Control_Joystick(ELBOW_1, 140, 200);
    Control_Joystick(LAZY_SUSAN_1, 100, 200);
  }
}

void Manual_Arm_2()
{
  if(digitalRead(35) == LOW)
  {
    if(analogRead(9) > 800 && analogRead(4) < 900)
    {Edmond(CLAW_2);}
    else
    {Control_DP3T(CLAW_2, 255);}
    Control_DP3T(TELESCOPE_2, 255);
    Control_Joystick(ELBOW_2, 140, 255);
    Control_Joystick(LAZY_SUSAN_2, 100, 255);
  }
  else
  {
    if(analogRead(4) > 800 && analogRead(4) < 900)
    {Edmond(CLAW_2);}
    else
    {Control_DP3T(CLAW_2, 200);}
    Control_DP3T(TELESCOPE_2, 140);
    Control_Joystick(ELBOW_2, 140, 200);
    Control_Joystick(LAZY_SUSAN_2, 100, 200);
  }
}

void loop()
{
  Manual_Arm_1();
  Manual_Arm_2();  
}
