int ledpin = 13;

void setup() 
{
  pinMode(ledpin, OUTPUT);
}

/*void genvector(char letter, unsigned short* vector)
{
  for(int x = 0; x < 15; ++x)
    vector[x] = (letter & (1 << 14-x)) >> x;
}*/

void loop() 
{
  unsigned short message[10][7];
  for(short i = 0;i <10;i++)
  {
    if(i%2==0)
    for (short j=0;j<10;j++)
    {
      if(i%2==0)
      {
        message[i][0]=0;
        message[i][1]=0;
        message[i][2]=1;
        message[i][4]=0;
      }
      else if(i==1)
      {
        message[i][0]=1;
        message[i][1]=0;
        message[i][2]=0;
        message[i][4]=0;
      }
      else if(i==3)
      {
        message[i][0]=1;
        message[i][1]=1;
        message[i][2]=1;
        message[i][4]=0;
      }
      else if(i==5)
      {
        message[i][0]=1;
        message[i][1]=1;
        message[i][2]=1;
        message[i][4]=0;
      }
      else if(i==7)
      {
        message[i][0]=1;
        message[i][1]=0;
        message[i][2]=0;
        message[i][4]=1;
      }
      else if(i==9)
      {
        message[i][0]=0;
        message[i][1]=1;
        message[i][2]=0;
        message[i][4]=1;
      }     
    }

    for (short i=0;i<10;i++)
    {
      message[i][3] = message[i][0]^message[i][1]^message[i][2];
      message[i][5] = message[i][0]^message[i][1]^message[i][4];
      message[i][6] = message[i][0]^message[i][2]^message[i][4];
    }

    for(short i=0;i<10;i++)
    {
      for(short j=0;j<7;j++)
      {
        digitalWrite(ledpin,message[i][j]);
        delay(250);
        digitalWrite(ledpin, LOW);
        delay(250);
      }
    }
    
  }
}
