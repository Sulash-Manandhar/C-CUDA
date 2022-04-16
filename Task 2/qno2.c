#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>


int password_size = 4;

char *encrypted_pswd[] = {
  "$6$AS$9IwGTn5WbHSalUs4ba3JbOfOUX/v1yD71Z4M2F6Yusz5k2WQEOFxqLIY80tudGtcFttqr/Zq6RIPjHkl/t2Pp1",
  "$6$AS$Qqv93HuzqjoyCeHoo2XSa0fDr/97Qxu3a8o1PdhI.ac6BmqXHhDrcA4VEvRhP0UoV.5lHzUHVwPTRpK4I2z7w1",
  "$6$AS$H1YldkeINMgVdSl4Oo.zGbtdf8QPSfSiClnkfw1xm0V3GvGmn40ET.Vlmkctv60Q7/1mGlC6/gjVeg6Ea4KeR/",
  "$6$AS$0TNsMtABiPtdhV1UK6ojNQX75f5O5GOER74CewhOE3TJEmAMpr52JpSYDYlQFMYWLPgEcDW25V/D2aQJStaw4."
};

void substring(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

void pw_crack_fn(char *salt_and_encrypted){
  int i, j, k;     
  char salt[7];    
  char plain[7];  
  char *enc;       
  int count = 0;   

  substring(salt, salt_and_encrypted, 0, 6);

  for(i='A'; i<='Z'; i++){
    for(j='A'; j<='Z'; j++){
      for(k=0; k<=99; k++){
        sprintf(plain, "%c%c%02d", i, j, k); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } 
      }
    }
  }
  printf("%d Outcomes Count \n", count);
}


int main(int argc, char *argv[]){
	int k;


 	for(k=0;k<password_size;k<k++) {
    		pw_crack_fn(encrypted_pswd[k]);
  	}
  return 0;
}
