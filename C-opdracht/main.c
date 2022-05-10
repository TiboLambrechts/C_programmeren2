#include <stdio.h>
#include <stdlib.h>

#define BMPINPUT "test.bmp"


int main(int argc, char const *argv[])
{
    FILE * inputBMP = fopen(BMPINPUT, "rb");
    unsigned char header[54] = {0};
    signed int hoogte = 0;
    signed int breedte = 0;
    unsigned char * pixels = NULL;
    int totaalAantalPixels = 0;

    if(inputBMP == NULL)
    {
        printf("%s\n", "ERROR: can't open the file");
        return -1;
    }

    fread(header, 1, 54, inputBMP);

    breedte = header[21] << 24 | header[20] << 16 | header[19] << 8 | header[18];
    printf("De breedte van mijn afbeelding is = %d\n", breedte);
    hoogte = header[25] << 24 | header[24] << 16 | header[23] << 8 | header[22];
    printf("De hoogte van mijn afbeelding is = %d\n", hoogte);

    totaalAantalPixels = breedte * hoogte;
    pixels = (unsigned char *) malloc(totaalAantalPixels*3);
    if(pixels == NULL)
    {
        printf("ERROR: memory allocation Failed\n");
        return -2;
    }

    fread(pixels, 1, totaalAantalPixels*3, inputBMP);
    printf("INFO: Heap memory allocated = %d (bytes)\n", totaalAantalPixels*3);

    //----------------------------------------

    int imageSize = breedte * hoogte * 3;
    for(int i =0; i < imageSize-2; i+=3)
        {
            printf("pixel %d: B= %d, G=%d, R=%d\n", i, pixels[i], pixels[i+1], pixels[i+2]);
        }

     //------------test smoothing------------

    signed int Red_gesmooth = 0;
    signed int Blue_gesmooth = 0;
    signed int Green_gesmooth = 0;

    Blue_gesmooth = (pixels[36] + pixels[39] + pixels[42] + pixels[24] + pixels[27] + pixels[30] + pixels[12] + pixels[15] + pixels[18]) / 9;
    printf("De blauw waarde van de gesmoothe pixel = %d\n", Blue_gesmooth);

    Green_gesmooth = (pixels[37] + pixels[40] + pixels[43] + pixels[25] + pixels[28] + pixels[31] + pixels[13] + pixels[16] + pixels[19]) / 9;
    printf("De groen waarde van de gesmoothe pixel = %d\n", Green_gesmooth);

    Red_gesmooth = (pixels[39] + pixels[41] + pixels[44] + pixels[26] + pixels[29] + pixels[32] + pixels[14] + pixels[15] + pixels[20]) / 9;
    printf("De groen waarde van de gesmoothe pixel = %d\n", Red_gesmooth);

    pixels[27] = Blue_gesmooth;
    pixels[28] = Green_gesmooth;
    pixels[29] = Red_gesmooth;


    fwrite(pixels, sizeof(pixels), 1, inputBMP);


    //----------einde teste smooting---------

    //----------------------------------------

    fclose(inputBMP);
    printf("INFO: File %s CLOSED\n", BMPINPUT);

    free(pixels);
    printf("INFO: Heap memory Freed = %d (bytes)\n", totaalAantalPixels*3);
    return 0;
}