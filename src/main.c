#include "sprec/sprec.h"

#define API_KEY "AIzaSyA4e_DmX-kxzXvGiUtrkrRSTxM4Pa5zIEA"
#define LANG    "fr-FR"
#define FILE_NAME    "src/assets/untitled.wav"

int main() {
//  char *txt = sprec_recognize_base64(API_KEY, LANG,
//                                   FILE_NAME, 16000);
  char *txt = sprec_recognize_wav(API_KEY, LANG,
                                   FILE_NAME, 16000);
  printf("%s", txt);
  return 0;
}
