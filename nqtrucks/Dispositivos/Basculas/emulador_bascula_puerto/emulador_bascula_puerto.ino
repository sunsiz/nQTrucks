/* 
Emulacion de Bascula 
7BITS
SERIAL_7N1
9600


    SERIAL_5N1
    SERIAL_6N1
    SERIAL_7N1
    SERIAL_8N1 (the default)
    SERIAL_5N2
    SERIAL_6N2
    SERIAL_7N2
    SERIAL_8N2
    SERIAL_5E1
    SERIAL_6E1
    SERIAL_7E1
    SERIAL_8E1
    SERIAL_5E2
    SERIAL_6E2
    SERIAL_7E2
    SERIAL_8E2
    SERIAL_5O1
    SERIAL_6O1
    SERIAL_7O1
    SERIAL_8O1
    SERIAL_5O2
    SERIAL_6O2
    SERIAL_7O2
    SERIAL_8O2 


Estuctura: 36 bits
  
  bit 0-36 DATOS:
                                 DEC       |      HEX      |        ASCII            |        FUNCION 
                bit 0     (     0x02              02h                 STX                  inicio de texto       )        
                bit 1     (   0x68|0x69         44h|45h               D|E              ESTADO(Estable/Dinamico)  )
  
                bit 2-3   (   0x32&0x66         20h|42h            Espacio&B               ID PESO BRUTO         )
                bit 4-11  ( [8]0x32..0x126   [8]20h..7Eh   [8]Caracteres Imprimibles         PESO BRUTO          )                
  
                bit 12-13 (   0x32&0x84         20h|54h            Espacio&T               ID PESO TARA          )
                bit 14-21 ( [8]0x32..0x126   [8]20h..7Eh    [8]Caracteres Imprimibles        PESO TARA           )
  
                bit 22-23 (   0x32&0x78         20h|4Eh            Espacio&N               ID PESO TARA          )
                bit 24-32 ( [8]0x32..0x126   [8]20h..7Eh    [8]Caracteres Imprimibles    SIGNO + PESO NETO       )
                bit 33    (     0x03              03h                 ETX                  fin de texto          )        
                bit 34    (     0x13              0Dh                 CR                  retorno de carro       )
                bit 35    (     0x10              0Ah                 LF                  retorno de linea       )                        

bucle

*/

const int defwordlength = 7;

void setup()
{
  //Configurar a 7BITS
 Serial.begin(9600,SERIAL_7N1); 
}

void loop()
{
 if (Serial.available() > 0) 
 {}
 for(int i=0;i<=10; i++){
  azero();
 }
 subiendo();
 

 for(int i=0;i<=10; i++){
  estable();
 }

  bajando();

}



void azero(){
  for(int i=0; i <=10; i++)
  {
   Serial.write(2);
   Serial.print(" E");
   Serial.print(" B");
   Serial.print("       0");
   Serial.print(" T");
   Serial.print("       0");
   Serial.print(" N");
   Serial.print("      +0");   
   Serial.write(3);
   Serial.write(13);
   Serial.write(10);
  }    
}

void estable(){
  for(int i=0; i <=10; i++)
  {
   Serial.write(2);
   Serial.print(" E");
   Serial.print(" B");
   Serial.print("    1897");
   Serial.print(" T");
   Serial.print("       0");
   Serial.print(" N");
   Serial.print("   +1897");   
   Serial.write(3);
   Serial.write(13);
   Serial.write(10);    
  } 
}




void subiendo(){
  for(int i=0; i <=6; i++)
  {
   Serial.write(2);
   Serial.print(" D");
   Serial.print(" B");
   Serial.print("       0");
   Serial.print(" T");
   Serial.print("       0");
   Serial.print(" N");
   Serial.print("      +0");   
   Serial.write(3);
   Serial.write(13);
   Serial.write(10);    
  }

  for(int i=0; i <=6; i++)
  {  
   Serial.write(2);
   Serial.print(" D");
   Serial.print(" B");
   Serial.print("     300");
   Serial.print(" T");
   Serial.print("       0");
   Serial.print(" N");
   Serial.print("    +300");   
   Serial.write(3);
   Serial.write(13);
   Serial.write(10);    
  }
  
  for(int i=0; i <=9; i++)
  {
   Serial.write(2);
   Serial.print(" D");
   Serial.print(" B");
   Serial.print("     600");
   Serial.print(" T");
   Serial.print("       0");
   Serial.print(" N");
   Serial.print("    +600");   
   Serial.write(3);
   Serial.write(13);
   Serial.write(10);    
  }

  for(int i=0; i <=4; i++)
  {
   Serial.write(2);
   Serial.print(" D");
   Serial.print(" B");
   Serial.print("     800");
   Serial.print(" T");
   Serial.print("       0");
   Serial.print(" N");
   Serial.print("    +800");   
   Serial.write(3);
   Serial.write(13);
   Serial.write(10);    
  }

  for(int i=0; i <=3; i++)
  {  
   Serial.write(2);
   Serial.print(" D");
   Serial.print(" B");
   Serial.print("     900");
   Serial.print(" T");
   Serial.print("       0");
   Serial.print(" N");
   Serial.print("    +900");   
   Serial.write(3);
   Serial.write(13);
   Serial.write(10);    
  }

  for(int i=0; i <=6; i++)
  {
   Serial.write(2);
   Serial.print(" D");
   Serial.print(" B");
   Serial.print("    1200");
   Serial.print(" T");
   Serial.print("       0");
   Serial.print(" N");
   Serial.print("   +1200");   
   Serial.write(3);
   Serial.write(13);
   Serial.write(10);    
  }

  for(int i=0; i <=4; i++)
  {
   Serial.write(2);
   Serial.print(" D");
   Serial.print(" B");
   Serial.print("    1890");
   Serial.print(" T");
   Serial.print("       0");
   Serial.print(" N");
   Serial.print("   +1890");   
   Serial.write(3);
   Serial.write(13);
   Serial.write(10);    
  }
  
}


void bajando(){
  for(int i=0; i <=4; i++)
  {
   Serial.write(2);
   Serial.print(" D");
   Serial.print(" B");
   Serial.print("    1890");
   Serial.print(" T");
   Serial.print("       0");
   Serial.print(" N");
   Serial.print("   +1890");   
   Serial.write(3);
   Serial.write(13);
   Serial.write(10);    
  }

  for(int i=0; i <=6; i++)
  {
   Serial.write(2);
   Serial.print(" D");
   Serial.print(" B");
   Serial.print("    1200");
   Serial.print(" T");
   Serial.print("       0");
   Serial.print(" N");
   Serial.print("   +1200");   
   Serial.write(3);
   Serial.write(13);
   Serial.write(10);    
  }

  for(int i=0; i <=3; i++)
  {  
   Serial.write(2);
   Serial.print(" D");
   Serial.print(" B");
   Serial.print("     900");
   Serial.print(" T");
   Serial.print("       0");
   Serial.print(" N");
   Serial.print("    +900");   
   Serial.write(3);
   Serial.write(13);
   Serial.write(10);    
  }

  for(int i=0; i <=4; i++)
  {
   Serial.write(2);
   Serial.print(" D");
   Serial.print(" B");
   Serial.print("     800");
   Serial.print(" T");
   Serial.print("       0");
   Serial.print(" N");
   Serial.print("    +800");   
   Serial.write(3);
   Serial.write(13);
   Serial.write(10);    
  }

  for(int i=0; i <=9; i++)
  {
   Serial.write(2);
   Serial.print(" D");
   Serial.print(" B");
   Serial.print("     600");
   Serial.print(" T");
   Serial.print("       0");
   Serial.print(" N");
   Serial.print("    +600");   
   Serial.write(3);
   Serial.write(13);
   Serial.write(10);    
  }

  for(int i=0; i <=6; i++)
  {  
   Serial.write(2);
   Serial.print(" D");
   Serial.print(" B");
   Serial.print("     300");
   Serial.print(" T");
   Serial.print("       0");
   Serial.print(" N");
   Serial.print("    +300");   
   Serial.write(3);
   Serial.write(13);
   Serial.write(10);    
  }

  for(int i=0; i <=6; i++)
  {
   Serial.write(2);
   Serial.print(" D");
   Serial.print(" B");
   Serial.print("       0");
   Serial.print(" T");
   Serial.print("       0");
   Serial.print(" N");
   Serial.print("      +0");   
   Serial.write(3);
   Serial.write(13);
   Serial.write(10);    
  }

}



