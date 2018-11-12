
void setup()
{
  Serial.begin(9600);
  Serial.println("\n---------\n  Setup\n---------");
  Serial.setTimeout(5);
}

void loop()
{
  if ( Serial.available()) {
    //String serialResponse = Serial.readStringUntil('\r\n');
    String serialResponse = Serial.readString();

    split(serialResponse, ";");
    Serial.println("GSUS ;: " + getStringUntilSeparator(serialResponse, ';'));
    Serial.println("GSUS ,: " + getStringUntilSeparator(serialResponse, ','));
    
    
  }
}

void split(String s, const char* separator){
  // Convert from String Object to String.
    char buf[s.length() * sizeof(s)];
    s.toCharArray(buf, sizeof(buf));
    char *p = buf;
    char *str;
    while ((str = strtok_r(p, separator, &p)) != NULL)
      Serial.println(str);
}

String getStringUntilSeparator(String s, char separator){
  int i = 0;
  while(s.charAt(i) != separator){
    i++;
  }
  return s.substring(0, i);
}
