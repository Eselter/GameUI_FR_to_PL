// Many thanks to BLUEPIXY for his function for binary replace
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


size_t replace(FILE *fi, FILE *fo, uint8_t *what, uint8_t *repl, size_t size){
    size_t i, index = 0, count = 0;
    int ch;
    
    while(EOF!=(ch=fgetc(fi))){
        if(ch == what[index]){
            if(++index == size){
                for(i = 0; i < size ; ++i){
                    fputc(repl[i], fo);
                }
                index = 0;
                ++count;
            }
        } else {
            for(i = 0; i < index ; ++i){
                fputc(what[i], fo);
            }
            index =0;
            fputc(ch, fo);
        }
    }
    for(i = 0; i < index ; ++i){
        fputc(what[i], fo);
    }

    return count;
}

void main(){
    FILE *file,*fileout;
    uint8_t what[] = {0x46, 0x72, 0x65, 0x6E, 0x63, 0x68, 0x00, 0x00, 0x66, 0x72, 0x65, 0x6E, 0x63};
    uint8_t repl[] = {0x50, 0x6F, 0x6C, 0x69, 0x73, 0x68, 0x00, 0x00, 0x70, 0x6F, 0x6C, 0x69, 0x73};
    
    uint8_t what2[] = {0x46, 0x72, 0x65, 0x6E, 0x63, 0x68, 0x00, 0x66, 0x72, 0x5F, 0x46, 0x52};
    uint8_t repl2[] = {0x50, 0x6F, 0x6C, 0x69, 0x73, 0x68, 0x00, 0x70, 0x6C, 0x5F, 0x50, 0x4C}; 
    size_t count;

    file=fopen("GameUI.dll","rb");
    fileout=fopen("GameUI.dll.step1","wb");
    
    count = replace(file, fileout, what, repl, sizeof(what));
    fclose(fileout);
    fclose(file);
		
    file=fopen("GameUI.dll.step1","rb");
    fileout=fopen("GameUI.dll.step2","wb");
    count += replace(file, fileout, what2, repl2, sizeof(what2));
    fclose(fileout);
    fclose(file);
    
    if (remove("GameUI.dll.step1") != 0){
    	printf("Blad usuwania pliku GameUI.dll.step1 jest to plik tymczasowy mozna go usunac recznie.\n");
	}
    
    if (count == 2){
    	if (rename("GameUI.dll", "GameUI.dll.bak") == 0){
    		if (rename("GameUI.dll.step2", "GameUI.dll") == 0){
    			printf("Podmieniono jezyk francuski na polski w menu gry.\n");
			} else {
				printf("Blad zmiany nazwy pliku GameUI.dll.step2, jest to plik finaly i poprawnie utowrzony. Mozna recznie usunac .step2 z nazwy.\n");
			}
		} else {
			printf("Blad tworzenia kopii zapasowej. Mozna recznie przeniesc plik GameUI.dll badz zmienic jego nazwe, a z pliku GameUI.dll.step2 usunac .step2 (jest to plik finalny poprawnie utworzony).\n");
		}
	} else if (count == 0) {
		printf("Nic nie podmieniono.\nPlik albo zostal wczesniej podmieniony i w menu powinien byc juz dostepny jezyk polski albo plik GameUI.dll sie bardzo zmienil i porgram juz nie jest z nim kompatyblilny.\n", count);
		remove("GameUI.dll.step2");
	} else {
		printf("Podmieniono %o ciagow znakow. Program powinien podmienic 2 ciagi, cos poszlo nie tak.\n", count);
		remove("GameUI.dll.step2");
	}
		
    system("pause");
}
