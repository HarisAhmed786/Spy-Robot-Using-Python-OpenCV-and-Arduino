String name; // to store user input
void setup() {
Serial.begin(9600); // Start Serial Monitor
Serial.println("Enter your name:");
}
void loop() {
if (Serial.available() > 0) { // check if data received
name = Serial.readString(); // read full string input
name.trim(); // remove extra spaces or newline
Serial.print("Hello ");
Serial.print(name);
Serial.println(", welcome to Arduino!");
delay(2000); // wait before asking again
Serial.println("Enter your name:");
}
}