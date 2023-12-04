int sound_analog = A0;

void setup() {
  Serial.begin(115200);
  pinMode(sound_analog, INPUT); 
}

long get_sound_value() {
  long sum = 0;
  int i = 0;
  const int max = 500;
  while(i < max) {
    sum += analogRead(sound_analog);
    i++;
  }

  return sum / max;
}

void loop() {
  int val_analog = get_sound_value();
  Serial.println(val_analog);
} 