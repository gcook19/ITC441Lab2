#include <cQueue.h>
#include <ESP8266WiFi.h>


int redLED = D6;
int yellowLED = D7;
int greenLED = D8;
const int offState = 0;
const int redState = 1;
const int greenState = 2;
const int yellowState = 3;
bool automode = false;
const char* ssid = "WIFI SSID";
const char* password = "WIFIPASSWORD";
unsigned long currentMillis;
unsigned long nextStage;
WiFiServer server(80);
int curState;
Queue_t stateQueue;


void greenLight() {
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, HIGH);
}
void yellowLight() {
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, HIGH);
}

void redLight() {
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, HIGH);
}

void offLight() {
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(10);
  Serial.println("Serial Output Beginning.");
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
  Serial.println("Done Initializing LEDs.");
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Server Started...");
  curState = offState;
  q_init(&stateQueue, sizeof(int), 20, FIFO, false);
  Serial.println("Queue created...");
  currentMillis = millis();
  nextStage = 0;

}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("Red");
 
  WiFiClient client = server.available();
  if(client) {
    while (!client.available()) {
      delay(10);
    }
  }
  client.setTimeout(100);
  String request = client.readStringUntil('\r');
  
  //Serial.println(request);
  client.flush();
  //Parse and deal with input.
  currentMillis = millis();
  if(request.indexOf("/off") != -1) {
    if(automode) {
      nextStage = currentMillis;
      automode = false;
    }
    
    //Queuing up this request
    q_push(&stateQueue, &offState);
    Serial.println("Queued Off State.");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
  }
  
  if(request.indexOf("/green") != -1) {
    if(automode) {
      nextStage = currentMillis;
      automode = false;
    }

    //Queuing up this request
    q_push(&stateQueue, &greenState);
    Serial.println("Queued Green State.");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    nextStage = currentMillis;
  }
  else if(request.indexOf("/red") != -1) {
     if(automode) {
      nextStage = currentMillis;
      automode = false;
    }
    
    //Queuing up this request
    q_push(&stateQueue, &redState);
    Serial.println("Queued Red State.");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
  }
  else if(request.indexOf("/yellow") != -1) {
    if(automode) {
      nextStage = currentMillis;
      automode = false;
    }
    
    //Queuing up this request
    q_push(&stateQueue, &yellowState);
    Serial.println("Queued Yellow State.");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
  }
  else if(request.indexOf("/Status") != -1) {
    //We need to print out the status here.
    Serial.println("Request for Status.");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.print("{\"mode\":\"");
    if(automode) {
      client.print("auto");
    } else {
      client.print("manual");  
    }
    client.print("\",\"light\":\"");
    if(curState == redState) {
      client.print("red");
    } else if (curState == greenState) {
      client.print("green");
    } else if (curState == yellowState) {
      client.print("yellow");
    } else {
      client.print("off");
    }
    client.println("\"}");
  }
  else if(request.indexOf("/auto") != -1) {
    automode = true;
    nextStage = currentMillis;
    //Clearing the Queue
    q_clean(&stateQueue);
    Serial.println("Auto Mode.");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    
  } else {
    //I need to output a web page here.
    client.println("<HTML>");
    client.println("<HEAD>");
    client.println("<style>");
    client.println("table, th, td {");
    client.println("  border: 1px solid black;");
    client.println("}");
    client.println("");
    client.println(".red{background-color: red;}");
    client.println(".yellow{background-color: yellow;}");
    client.println(".green{background-color: green;}");
    client.println(".white{background-color: white;}");
    client.println("</style>");
    client.println("<script>");
    client.println("window.onload = function () {");
    client.println("  var b1 = document.getElementById(\"Off\");");
    client.println("  var b2 = document.getElementById(\"Auto\");");
    client.println("  var b3 = document.getElementById(\"Red\");");
    client.println("  var b4 = document.getElementById(\"Yellow\");");
    client.println("  var b5 = document.getElementById(\"Green\");");
    client.println("  ");
    client.println("  b1.onclick = handler;");
    client.println("  b2.onclick = handler;");
    client.println("  b3.onclick = handler;");
    client.println("  b4.onclick = handler;");
    client.println("  b5.onclick = handler;");
    client.println("}");
    client.println("");
    client.println("function handler() {");
    client.println("  var radios = document.getElementsByName('Traffic');");
    client.println("  var radioval = \"\";");
    client.println("  for (var i = 0, length = radios.length; i < length; i++)");
    client.println("  {");
    client.println("    if (radios[i].checked)");
    client.println("    {");
    client.println("      radioval = radios[i].value.toLowerCase();");
    client.println("");
    client.println("    // only one radio can be logically checked, don't check the rest");
    client.println("    break;");
    client.println("    }");
    client.println("  }");
    client.println("  var colorchangeurl = \"/\" + radioval;");
    client.println("  var newhttp = new XMLHttpRequest();");
    client.println("  newhttp.open(\"GET\", colorchangeurl);");
    client.println("  newhttp.send();");
    client.println("  ");
    client.println("}");
    client.println("");
    client.println("setInterval( function() {");
    client.println("  var xmlhttp = new XMLHttpRequest();");
    client.println("  xmlhttp.onreadystatechange = function() {");
    client.println("    if (this.readyState == 4 && this.status == 200) {");
    client.println("      var myObj = JSON.parse(this.responseText);");
    client.println("      document.getElementById(\"Mode\").innerHTML = myObj.mode;");
    client.println("      document.getElementById(\"Light\").innerHTML = myObj.light;");
    client.println("      var cell = document.getElementById(\"Light\");");
    client.println("      if(myObj.light == \"green\") {");
    client.println("        cell.className = \"green\";");
    client.println("      } else if (myObj.light == \"yellow\") {");
    client.println("        cell.className = \"yellow\";");
    client.println("      } else if (myObj.light == \"red\") {");
    client.println("        cell.className = \"red\";");
    client.println("      } else {");
    client.println("        cell.className = \"white\";");
    client.println("      }");
    client.println("    }");
    client.println("  };");
    client.println("  xmlhttp.open(\"GET\", \"/Status\", true);");
    client.println("  xmlhttp.send();");
    client.println("}, 1000);");
    client.println("</script>");
    client.println("</HEAD>");
    client.println("<BODY>");
    client.println("<table>");
    client.println("<tr>");
    client.println("  <td><font size=7>Mode:</td>");
    client.println("    <td><font size=7 id=Mode>&nbsp;</font></td>");
    client.println("</tr>");
    client.println("<tr>");
    client.println("  <td><font size=7>State:</td>");
    client.println("  <td><font size=7 ID=Light>&nbsp;</td>");
    client.println("</tr>");
    client.println("</table>");
    client.println("");
    client.println("<input type=\"radio\" name=\"Traffic\" id=\"Off\" value=\"Off\" />");
    client.println("<label for=\"Off\">Off</label>");
    client.println("<input type=\"radio\" name=\"Traffic\" id=\"Auto\" value=\"Auto\" />");
    client.println("<label for=\"Auto\">Auto</label>");
    client.println("<input type=\"radio\" name=\"Traffic\" id=\"Red\" value=\"Red\" />");
    client.println("<label for=\"Red\">Red</label>");
    client.println("<input type=\"radio\" name=\"Traffic\" id=\"Yellow\" value=\"Yellow\" />");
    client.println("<label for=\"Yellow\">Yellow</label>");
    client.println("<input type=\"radio\" name=\"Traffic\" id=\"Green\" value=\"Green\" />");
    client.println("<label for=\"Green\">Green</label>");
    client.println("</body>");
    client.println("</html>");
  }
  currentMillis = millis();

  ///Light control is down here.
  if(automode && nextStage <= currentMillis) {
    //We are in Auto mode.
    if( curState == offState) {
        //Go to red.
        redLight();
        curState = redState;
        nextStage = currentMillis + 15000;
        Serial.println("Current State is Red.");
    }else if ( curState == redState) {
        //Go to green.
        greenLight();
        curState = greenState;
        nextStage = currentMillis + 15000;
        Serial.println("Current State is Green.");
    } else if( curState == greenState ) {
      //Go to yellow
       yellowLight();
       curState = yellowState;
       nextStage = currentMillis + 4000;
       Serial.println("Current State is Yellow.");
    } else {
      //The only state left is the Yellow State.
      //Go to Red State
      redLight();
      curState = redState;
      nextStage = currentMillis + 15000;
      Serial.println("Current State is Red.");
    }
    Serial.print("CurMillis is: ");
    Serial.println(currentMillis);
    Serial.print("Next Stage is: ");
    Serial.println(nextStage);
  } else if (automode == false && nextStage <= currentMillis) {
    //Manual Mode. First we check if something is the Queue before we try to do anything.
    if(q_getCount(&stateQueue) > 0) {
      //There is stuff in the Queue. We need to go to work.
      int nextState;
      //Peeking what hte next command is
      q_peek(&stateQueue,&nextState);
      if(curState == offState) {
        //We skip checking the off state. We are already off, so we don't have to do anything.
        if(nextState == greenState) {
          greenLight();
          curState = greenState;
          Serial.println("Green");
        } else if (nextState == redState) {
          redLight();
          curState = redState;
          Serial.println("Red");
        } else if (nextState == yellowState) {
          yellowLight();
          curState = yellowState;
          Serial.println("Yellow");
        }
      } else if (curState == greenState) {
        //A lit light can only go to two places. To one other lit light or to off.
        if(nextState == yellowState || nextState == redState) {
          yellowLight();
          curState = yellowState;
          Serial.println("Yellow");
        } else if (nextState == offState) {
          offLight();
          curState = offState;
          Serial.println("Off");
        }
      } else if (curState == yellowState) {
        if(nextState == redState || nextState == greenState) {
          redLight();
          curState = redState;
          Serial.println("Red");
        } else if (nextState == offState) {
          offLight();
          curState = offState;
          Serial.println("Off");
        }
      } else {
        //This should mean our current state is the redState. The next command is just in case something goes wrong.
        curState = redState;
        if(nextState == greenState || nextState == yellowState) {
          greenLight();
          curState = greenState;
          Serial.println("Green");
        } else if(nextState == offState) {
          offLight();
          curState = offState;
          Serial.println("Off");
        }
      }
      if(curState == nextState) {
        //We've reached the state we need to. We should pop it off the Queue so we can work on getting to the next requested state.
        Serial.print("Popping ");
        if(curState == redState) {
          Serial.print("red");
        } else if (curState == yellowState) {
          Serial.print("yellow");
        } else if (curState == greenState) {
          Serial.print("green");
        } else {
          Serial.print("off");
        }
        Serial.println(" from Queue.");
        q_pop(&stateQueue, &nextState);
      }
      //Programming our next wait.
      nextStage = currentMillis + 1000;
    }
  }

}
