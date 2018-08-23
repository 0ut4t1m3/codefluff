void button1_shrt() {
  if (mask1 == 0 && lock == 0) {
    screen++;
    refresh = 1;
    v_menu  = 0;
    m_set   = 8;
  }
  mask1 = 0;
}

void button1_long() {
  mask1   = 1;
  if (digitalRead(PB13) == 1) {
    lock = !lock;
  } else {
    if (lock == 0) {
      button  = 3;
      refresh = 1;
    }
  }
}

void button2_shrt() {
  if (mask2 == 0 && lock == 0) {
    refresh = 1;
    button  = 1;
  }
  mask2 = 0;
}

void button2_long() {
  mask2   = 1;
  if (lock == 0 && digitalRead(PB14) == 0) {
    refresh = 1;
    button  = 2;
  }
}
